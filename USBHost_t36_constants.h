#ifndef _USBHost_t36_constants_h_
#define _USBHost_t36_constants_h_
#include <usb_ch9.h>
#define claim_type_device   0
#define claim_type_interface    1
#define claim_type_IAD_descriptors  2

#define enum_got_DEV8B_setMaxPkt_setAddr    0   // read 8 bytes of device desc, set max packet, and send set address
#define enum_addrSet_req_DEV18B             1   // request all 18 bytes of device descriptor
#define enum_got_DEV18B_parse_dev           2   // parse 18 device desc bytes into the dev from the Transfer_t
#define enum_req_langID                     3   // request Language ID
#define enum_got_langID_parsing             4   // parse Language ID, not sure how it is doing it ATM
#define enum_req_str_manuf                  5   // request Manufacturer string
#define enum_got_str_manuf_parsing          6   // parse Manufacturer string
#define enum_req_str_prod                   7   // request product string
#define enum_got_str_prod_parsing           8   // parse Product string
#define enum_req_str_ser                    9   // request serial number string
#define enum_got_str_ser_parsing            10  // parse serial number string
#define enum_req_CONF9B                     11  // request first 9 bytes of config desc
#define enum_got_CONF9B_req_CONF_all        12  // read 9 bytes, request all of config desc
#define enum_got_CONF_all_set_conf          13  // read all config desc, update dev, send set config
#define enum_dev_configured_claim           14  // device is now configured, call unused drivers to claim the device
#define enum_nothing_to_do                  15  // control transfers for other stuff?

#define HID_SET_IDLE(setup) (mk_setup(setup,USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE,HID_REQUEST_SET_IDLE,0,0,0))
#define HID_SET_PROTOCOL(setup) (mk_setup(setup,USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE,HID_REQUEST_SET_PROTOCOL,BOOT_PROTOCOL,0,0))
#endif