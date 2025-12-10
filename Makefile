default: log

env:
	docker run --privileged -v ~/edu/domotica:/project/zephyr/project -it smvd-zephyr

build:
	west build -b nucleo_f091rc . --runner=openocd

flash: build
	west flash --runner=openocd

log: flash
	stty -F /dev/ttyACM0 115200 raw -echo
	cat /dev/ttyACM0
