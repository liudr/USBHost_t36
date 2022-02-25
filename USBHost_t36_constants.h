#ifndef _USBHost_t36_constants_h_
#define _USBHost_t36_constants_h_
#include <usb_ch9.h>
#define claim_type_device   0
#define claim_type_interface    1
#define claim_type_IAD_descriptors  2

#define HID_SET_IDLE(setup) (mk_setup(setup,USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE,HID_REQUEST_SET_IDLE,0,0,0))
#define HID_SET_PROTOCOL(setup) (mk_setup(setup,USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE,HID_REQUEST_SET_PROTOCOL,BOOT_PROTOCOL,0,0))
#endif