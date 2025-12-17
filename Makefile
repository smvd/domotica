BOARD=nrf52840dongle/nrf52840/bare
BLOCK_DEVICE=/dev/ttyACM0
BAUD_RATE=115200

default: flash

build:
	west build -b $(BOARD) --build-dir build 


# DO NOT FLASH WHILE MONITORING THE SERIAL INTERFACE IT WILL CRASH THE USB CONTROLLER
flash: build
	west flash

clean:
	nrfjprog --eraseall -f NRF52
	rm build -rf
	
log: flash
	stty -F $(BLOCK_DEVICE) $(BAUD_RATE) raw -echo
	cat $(BLOCK_DEVICE)

env:
	docker run --privileged -v ~/edu/domotica:/project/zephyr/project -it smvd-zephyr

env-build:
	docker build -t smvd-zephyr .
