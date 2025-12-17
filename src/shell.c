#include "shell.h"

LOG_MODULE_REGISTER(SHELL, LOG_LEVEL_INF);

const struct device * UART_DEVICE = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart);

uint8_t SHELL_rxThrottled = 0;
char SHELL_uartBufferData[SHELL_UART_RING_BUFFER_SIZE];
struct ring_buf SHELL_uartBuffer;

struct usbd_context * SHELL_usbDevice;
K_SEM_DEFINE(SHELL_uartReady, 0, 1);

uint8_t SHELL_Init() {
    if (!device_is_ready(UART_DEVICE)) {
    	LOG_ERR("UART device not ready");
    	return 0;
    }

    if (SHELL_EnableUSB()) {
    	LOG_ERR("Failed to enable USB device support");
    	return 0;
    }

    ring_buf_init(&SHELL_uartBuffer, sizeof(SHELL_uartBufferData), SHELL_uartBufferData);
    LOG_INF("Waiting for UART ready signal");
    k_sem_take(&SHELL_uartReady, K_FOREVER);
    LOG_INF("UART ready");

    k_msleep(100);
    
    uart_irq_callback_set(UART_DEVICE, SHELL_UartInteruptHandler);
    uart_irq_rx_enable(UART_DEVICE);

    return 0;
}

void SHELL_MessageCallback(struct usbd_context * const ctx, const struct usbd_msg * msg) {
    if (usbd_can_detect_vbus(ctx)) {
        if (msg->type == USBD_MSG_VBUS_READY && usbd_enable(ctx)) {
            LOG_ERR("WEIRD 1, Failed to enable device support");
        }

        if (msg->type == USBD_MSG_VBUS_REMOVED && usbd_disable(ctx)) {
            LOG_ERR("WEIRD 2, Failed to disable device support");
    	}
    }

    if (msg->type == USBD_MSG_CDC_ACM_CONTROL_LINE_STATE) {
        uint32_t dtr = 0U;

        uart_line_ctrl_get(UART_DEVICE, UART_LINE_CTRL_DTR, &dtr);
        if (dtr) {
            k_sem_give(&SHELL_uartReady);
        }
    }
}

uint8_t SHELL_EnableUSB() {
    SHELL_usbDevice = USB_Init(SHELL_MessageCallback);
    if (SHELL_usbDevice == NULL) {
    	LOG_ERR("Failed to initialize USB device");
    	return 1;
    }

    if (!usbd_can_detect_vbus(SHELL_usbDevice) && usbd_enable(SHELL_usbDevice)) {
		LOG_ERR("Failed to enable device support");
		return 1;
    }

    return 0;
}

void SHELL_UartInteruptHandler(const struct device * dev, void * user_data)
{
	while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
        if (SHELL_rxThrottled == 0 && uart_irq_rx_ready(dev)) {
            char buffer[SHELL_UART_BUFFER_SIZE];
            size_t ringBufferSpace = MIN(ring_buf_space_get(&SHELL_uartBuffer), SHELL_UART_BUFFER_SIZE);

            if (ringBufferSpace == 0) {
                uart_irq_rx_disable(dev);
                SHELL_rxThrottled = 1;
                continue;
            }

            size_t recievedCharacterCount = uart_fifo_read(dev, buffer, ringBufferSpace);
            size_t bufferCharacterCount = ring_buf_put(&SHELL_uartBuffer, buffer, recievedCharacterCount);

            if (bufferCharacterCount != 0) {
                uart_irq_tx_enable(dev);
            }
		}

		if (uart_irq_tx_ready(dev)) {
			char buffer[SHELL_UART_BUFFER_SIZE];
            size_t sendCharacterCount = ring_buf_get(&SHELL_uartBuffer, buffer, SHELL_UART_BUFFER_SIZE);

            if (sendCharacterCount == 0) {
                uart_irq_tx_disable(dev);
                continue;
            }

            if (SHELL_rxThrottled == 1) {
                uart_irq_rx_enable(dev);
                SHELL_rxThrottled = 0;
            }

            uart_fifo_fill(dev, buffer, sendCharacterCount);
        }
	}
}
