#include "gpio.h"

LOG_MODULE_REGISTER(GPIO, LOG_LEVEL_INF);
const struct gpio_dt_spec GPIO_LED = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
const struct gpio_dt_spec GPIO_BUTTON = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
struct gpio_callback GPIO_buttonCallbackData;

otCoapResource GPIO_rsc = {
    .mUriPath = "led",
    .mHandler = GPIO_Handler,
    .mContext = NULL,
    .mNext = NULL
};

uint8_t GPIO_Init() {
	if (!gpio_is_ready_dt(&GPIO_LED)) {
        LOG_ERR("led device not ready");
		return 1;
	}
	
	if (gpio_pin_configure_dt(&GPIO_LED, GPIO_OUTPUT_HIGH) < 0) {
        LOG_ERR("led configuration failed");
		return 1;
	}
	

	if (!gpio_is_ready_dt(&GPIO_BUTTON)) {
        LOG_ERR("button device not ready");
        return 1;
	}

	if (gpio_pin_configure_dt(&GPIO_BUTTON, GPIO_INPUT) != 0) {
        LOG_ERR("failed to configure button");
        return 1;
	}

	if (gpio_pin_interrupt_configure_dt(&GPIO_BUTTON, GPIO_INT_EDGE_TO_ACTIVE) != 0) {
        LOG_ERR("failed to configure button interupt");
        return 1;
	}

	gpio_init_callback(&GPIO_buttonCallbackData, GPIO_ButtonCallback, BIT(GPIO_BUTTON.pin));
	gpio_add_callback_dt(&GPIO_BUTTON, &GPIO_buttonCallbackData);

	otCoapAddResource(COAP_openThread, &GPIO_rsc);

	LOG_INF("GPIO ready");

    return 0;
}

void GPIO_ButtonCallback(const struct device * dev, struct gpio_callback * cb, uint32_t pins) {
    COAP_SendRequest("ff03::1", "led", OT_COAP_CODE_PUT, NULL, 0);
}

void GPIO_Handler(void * ctx, otMessage * message, const otMessageInfo * messageInfo) {
    COAP_RequestHandler(ctx, message, messageInfo, GPIO_PutHandler, GPIO_GetHandler);
}

uint8_t GPIO_PutHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo) {
    gpio_pin_toggle_dt(&GPIO_LED);
}

uint8_t GPIO_GetHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo) {
    gpio_pin_toggle_dt(&GPIO_LED);
}
