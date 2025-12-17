#ifndef INCLUDED_USB
#define INCLUDED_USB

#include <zephyr/device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/bos.h>

#include <stdint.h>

#define USB_VENDOR_ID       0x2fe3
#define USB_PRODUCT_ID      0x0001
#define USB_MANUFACTURER    "Domotica"
#define USB_PRODUCT         "THREAD node interface"
#define USB_MAX_POWER       125

struct usbd_context * USB_Init(usbd_msg_cb_t messageCallback);
struct usbd_context * USB_SetupDevice(usbd_msg_cb_t messageCallback);
void USB_FixCodeTripplePoint(struct usbd_context * uds_ctx,  const enum usbd_speed speed);

#endif
