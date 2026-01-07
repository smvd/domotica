#include "uart.h"

LOG_MODULE_REGISTER(UART, LOG_LEVEL_INF);

const struct device * UART_DEVICE = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart);

uint8_t UART_commandReady = 0;
char UART_commandBuffer[UART_BUFFER_SIZE];
size_t UART_commandBufferIndex = 0;
char UART_bufferData[UART_BUFFER_SIZE];
struct ring_buf UART_buffer;

struct usbd_context * UART_usbDevice;

uint8_t UART_Init() {
    if (!device_is_ready(UART_DEVICE)) {
    	LOG_ERR("UART device not ready");
    	return 0;
    }

    if (UART_EnableUSB()) {
    	LOG_ERR("Failed to enable USB device support");
    	return 0;
    }

    ring_buf_init(&UART_buffer, sizeof(UART_bufferData), UART_bufferData);
    
    uart_irq_callback_set(UART_DEVICE, UART_InteruptHandler);
    uart_irq_rx_enable(UART_DEVICE);

    return 0;
}

void UART_MessageCallback(struct usbd_context * const ctx, const struct usbd_msg * msg) {
    if (usbd_can_detect_vbus(ctx)) {
        if (msg->type == USBD_MSG_VBUS_READY && usbd_enable(ctx)) {
            LOG_ERR("Failed to enable device support");
        }

        if (msg->type == USBD_MSG_VBUS_REMOVED && usbd_disable(ctx)) {
            LOG_ERR("Failed to disable device support");
    	}
    }
}

uint8_t UART_EnableUSB() {
    UART_usbDevice = USB_Init(UART_MessageCallback);
    if (UART_usbDevice == NULL) {
    	LOG_ERR("Failed to initialize USB device");
    	return 1;
    }

    if (!usbd_can_detect_vbus(UART_usbDevice) && usbd_enable(UART_usbDevice)) {
		LOG_ERR("Failed to enable device support");
		return 1;
    }

    return 0;
}

void UART_InteruptHandler(const struct device * dev, void * user_data) {
	while (uart_irq_update(UART_DEVICE) && uart_irq_is_pending(UART_DEVICE)) {
        if (UART_commandReady == 0 && uart_irq_rx_ready(UART_DEVICE)) {
            char c;
            uart_fifo_read(UART_DEVICE, &c, 1);

            if (c == '\r') {
                char * temp = "\r\n";
                UART_Write(temp, 2);
                UART_commandBuffer[UART_commandBufferIndex] = '\0';
                UART_DisableInput();
            } else if (c == '\b' || c == 0x7F) {
                char * temp = "\b \b";
                UART_Write(temp, 3);
                UART_commandBufferIndex -= 1;
            } else {
                UART_Write(&c, 1);
                UART_commandBuffer[UART_commandBufferIndex++] = c;
            }
		}

		if (uart_irq_tx_ready(dev)) {		
			char buffer[UART_BUFFER_SIZE];

			// Read the data from the ringbuffer
            size_t sendCharacterCount = ring_buf_get(&UART_buffer, buffer, UART_BUFFER_SIZE);

            // If there is nothing in the ringbuffer disable output
            if (sendCharacterCount == 0) {
                uart_irq_tx_disable(dev);
            }

            uart_fifo_fill(dev, buffer, sendCharacterCount);
        }
	}
}

void UART_EnableInput() {
    UART_commandReady = 0;
    uart_irq_rx_enable(UART_DEVICE);
}

void UART_DisableInput() {
    UART_commandReady = 1;
    uart_irq_rx_disable(UART_DEVICE);
}

void UART_Write(char * string, size_t length) {
    ring_buf_put(&UART_buffer, string, length);
    uart_irq_tx_enable(UART_DEVICE);
}
