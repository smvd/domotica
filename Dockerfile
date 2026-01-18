FROM ubuntu:latest

# Ready the container for installing programs
ENV DEBIAN_FRONTEND=noninteractive
SHELL ["/bin/bash", "-eo", "pipefail", "-c"]

# Install all needed dependencies and programs available in apt
RUN --mount=type=cache,target=/var/cache/apt --mount=type=cache,target=/var/lib/apt                                                                                                                 \
       apt-get update                                                                                                                                                                               \
    && apt-get install -y --no-install-recommends                                                                                                                                                   \
        git cmake ninja-build gperf ccache dfu-util device-tree-compiler wget python3-dev unzip python3-venv python3-tk xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1 curl \
        openocd stlink-tools libstlink-dev tio                                                                                                                                                      \
    && apt-get clean                                                                                                                                                                                \
    && rm -rf /var/lib/apt/lists/*

# Install west
RUN --mount=type=cache,target=/root/.cache/pip --mount=type=cache,target=/root/.cache/west  \
       python3 -m venv /project/.venv                                                       \
    && . /project/.venv/bin/activate                                                        \
    && pip3 install --no-cache-dir west pyelftools                                          \
    && west init -m https://github.com/zephyrproject-rtos/zephyr /project                   \
    && cd /project                                                                          \
    && west update                                                                          \
    && west zephyr-export                                                                   \
    && pip3 install -r /project/zephyr/scripts/requirements.txt

# Install the zephyr sdk
RUN --mount=type=cache,target=/root/.cache/sdk                                                                              \
       wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.17.4/zephyr-sdk-0.17.4_linux-x86_64.tar.xz    \
    && tar xvf zephyr-sdk-0.17.4_linux-x86_64.tar.xz                                                                        \
    && cd zephyr-sdk-0.17.4                                                                                                 \
    && ./setup.sh -t arm-zephyr-eabi -h -c

# Make the venv globally available
ENV PATH="/project/.venv/bin:${PATH}"

# Install nrfutils
RUN    mkdir -p /project/.venv/bin                                                                                                                          \
    && curl -L https://files.nordicsemi.com/artifactory/swtools/external/nrfutil/executables/x86_64-unknown-linux-gnu/nrfutil -o /project/.venv/bin/nrfutil \
    && chmod +x /project/.venv/bin/nrfutil                                                                                                                  \
    && /project/.venv/bin/nrfutil install device

# Install jlink
# The .deb should be manually downloaded from https://www.segger.com/downloads/jlink/ (version may differ so change the name accordingly)
COPY    JLink_Linux_V894_x86_64.deb /tmp/jlink.deb
RUN     dpkg-deb -x /tmp/jlink.deb /

# Install jprog
# The .deb should be manually downloaded from https://www.nordicsemi.com/Products/Development-tools/nRF-Command-Line-Tools/Download (version may differ so change the name accordingly)
COPY    nrf-command-line-tools_10.24.2_amd64.deb /tmp/nrf-cli.deb
RUN     dpkg-deb -x /tmp/nrf-cli.deb /
ENV PATH="/opt/nrf-command-line-tools/bin:${PATH}"

# Setup the project folder
RUN     mkdir /project/zephyr/project

# Ready the container for development
WORKDIR /project/zephyr/project
CMD     ["bash"]

# The container should be run in priviliged mode or with the correct usb device passed through
# The project folder folder should my linked to /project/zephyr/project
# Example:
#   docker run --privileged -v /c/Users/bbels/Desktop/school/domotica/domotica:/project/zephyr/project -it smvd-zephyr
