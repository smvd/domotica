#include "usb.h"

LOG_MODULE_REGISTER(USB, LOG_LEVEL_INF);

const char * USB_BLOCKLIST[] = {
	"dfu_dfu",
	NULL
};

USBD_DEVICE_DEFINE(USB_device, DEVICE_DT_GET(DT_NODELABEL(zephyr_udc0)), USB_VENDOR_ID, USB_PRODUCT_ID);
USBD_DESC_LANG_DEFINE(USB_language);
USBD_DESC_MANUFACTURER_DEFINE(USB_manufacturer, USB_MANUFACTURER);
USBD_DESC_PRODUCT_DEFINE(USB_product, USB_PRODUCT);

const uint8_t USB_attributes = USB_SCD_SELF_POWERED;

USBD_DESC_CONFIG_DEFINE(USB_fullSpeedConfigurationDescriptor, "FS Configuration");
USBD_CONFIGURATION_DEFINE(USB_fullSpeedConfiguration, USB_attributes, USB_MAX_POWER, &USB_fullSpeedConfigurationDescriptor);

struct usbd_context * USB_Init(usbd_msg_cb_t messageCallback) {
	if (USB_SetupDevice(messageCallback) == NULL) {
		return NULL;
	}

	if (usbd_init(&USB_device)) {
		return NULL;
	}

	return &USB_device;
}

struct usbd_context * USB_SetupDevice(usbd_msg_cb_t messageCallback) {
	if (usbd_add_descriptor(&USB_device, &USB_language)) {
		LOG_ERR("Failed to initialize language descriptor");
		return NULL;
	}

	if (usbd_add_descriptor(&USB_device, &USB_manufacturer)) {
		LOG_ERR("Failed to initialize manufacturer descriptor");
		return NULL;
	}

	if (usbd_add_descriptor(&USB_device, &USB_product)) {
		LOG_ERR("Failed to initialize product descriptor");
		return NULL;
	}

	if (usbd_add_configuration(&USB_device, USBD_SPEED_FS, &USB_fullSpeedConfiguration)) {
		LOG_ERR("Failed to add Full-Speed configuration");
		return NULL;
	}
	
	if (usbd_register_all_classes(&USB_device, USBD_SPEED_FS, 1, USB_BLOCKLIST)) {
		LOG_ERR("Failed to add register classes");
		return NULL;
	}

	USB_FixCodeTripplePoint(&USB_device, USBD_SPEED_FS);
	usbd_self_powered(&USB_device, USB_attributes & USB_SCD_SELF_POWERED);

	if (messageCallback != NULL) {
		if (usbd_msg_register_cb(&USB_device, messageCallback)) {
			LOG_ERR("Failed to register message callback");
			return NULL;
		}
	}

	return &USB_device;
}

void USB_FixCodeTripplePoint(struct usbd_context * uds_ctx,  const enum usbd_speed speed) {
	if (IS_ENABLED(CONFIG_USBD_CDC_ACM_CLASS) || IS_ENABLED(CONFIG_USBD_CDC_ECM_CLASS) || IS_ENABLED(CONFIG_USBD_CDC_NCM_CLASS) || IS_ENABLED(CONFIG_USBD_MIDI2_CLASS) || IS_ENABLED(CONFIG_USBD_AUDIO2_CLASS) || IS_ENABLED(CONFIG_USBD_VIDEO_CLASS)) {
		usbd_device_set_code_triple(uds_ctx, speed, USB_BCC_MISCELLANEOUS, 0x02, 0x01);
	} else {
		usbd_device_set_code_triple(uds_ctx, speed, 0, 0, 0);
	}
}
