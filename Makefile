BOARD=nrf52840dongle/nrf52840/bare
BLOCK_DEVICE=/dev/ttyACM0
BAUD_RATE=115200

default: flash

build:
	west build -b $(BOARD) --build-dir build 

build_router:
	west build -b $(BOARD) --build-dir build_router -- -DCONFIG_MESH_ROUTER_NODE=y

# DO NOT FLASH WHILE MONITORING THE SERIAL INTERFACE IT WILL CRASH THE USB CONTROLLER
flash: build
	west flash --build-dir build

router: build_router
	west flash --build-dir build_router

# ON WINDOS clean before rebuilding else it'll fail.

clean:
	nrfjprog --eraseall -f NRF52
	rm build -rf
	rm build_router -rf
	
log: flash
	stty -F $(BLOCK_DEVICE) $(BAUD_RATE) raw -echo
	cat $(BLOCK_DEVICE)

env:
	docker run --privileged -v /c/Users/bbels/Desktop/school/domotica/domotica:/project/zephyr/project -v /dev:/dev -it smvd-zephyr

env-build:
	docker build -t smvd-zephyr .
