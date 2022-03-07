#ifndef _USBHost_t36_constants_h_
#define _USBHost_t36_constants_h_
#include <usb_ch9.h>

// IMXRT constants
#define bmTDToken_DataToogle                (1<<31) // 0x8000 0000
#define bmTDToken_InterruptOnComplete		(1<<15) // 0x8000
#define bmTDToken_PID   					(3<<8)  // 0x0300 00 OUT Token generates token, 01 IN Token generates token, 10 SETUP Token generates token
#define bmTDToken_Active					(1<<7)  // 0x80
#define bmTDToken_Halted					(1<<6)  // 0x40
#define bmTDToken_DataBufferError			(1<<5)  // 0x20
#define bmTDToken_BabbleDetected			(1<<4)  // 0x10
#define bmTDToken_TransactionError			(1<<3)  // 0x08
#define bmTDToken_MissedMicroFrame			(1<<2)  // 0x04
#define bmTDToken_SplitTransactionState		(1<<1)  // 0x02
#define bmTDToken_PingState_ERR				1

// Library states and status constants
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

// HID class control requests and messages
#define mk_HID_SET_IDLE(setup) (mk_setup(setup,bmREQ_TYPE_SET_IDLE,HID_REQUEST_SET_IDLE,0,0,0))
#define mk_HID_SET_PROTOCOL(setup) (mk_setup(setup,bmREQ_TYPE_SET_PROTOCOL,HID_REQUEST_SET_PROTOCOL,BOOT_PROTOCOL,0,0))
#define mk_HID_GET_REPORT_DESC(setup,interface,len) (mk_setup(setup,bmREQ_TYPE_GET_HIDR_DESC,HID_REQUEST_GET_HIDR_DESC,HID_DESCRIPTOR_REPORT<<8,interface,len))

#define HID_SET_IDLE_MSG (bmREQ_TYPE_SET_IDLE+(HID_REQUEST_SET_IDLE<<8))
#define HID_GET_REPORT_DESC_MSG (bmREQ_TYPE_GET_HIDR_DESC+(HID_REQUEST_GET_HIDR_DESC<<8U)+(HID_DESCRIPTOR_REPORT<<24U))

// HUB class control requests
#define HUB_SET_PORT_FEATURE(setup,wV,wI,wL) (mk_setup(setup,bmREQ_SET_PORT_FEATURE,USB_REQUEST_SET_FEATURE,wV,wI,wL))
#define HUB_SET_PORT_FEATURE_POWER_ON(setup,port) (mk_setup(setup,bmREQ_SET_PORT_FEATURE,USB_REQUEST_SET_FEATURE,HUB_FEATURE_PORT_POWER,port,0))
#define HUB_SET_PORT_FEATURE_POWER_OFF(setup,port) (mk_setup(setup,bmREQ_CLEAR_PORT_FEATURE,USB_REQUEST_CLEAR_FEATURE,HUB_FEATURE_PORT_POWER,port,0))

// Printer class control requests
#define mk_PRN_GET_DEVICE_ID(setup,conf_index,interface,len) (mk_setup(setup,bmREQUEST_TYPE_GET_DEVICE_ID,PRINTER_REQUEST_GET_DEVICE_ID,conf_index, interface<<8, len))    // interface is the high byte of wIndex
#endif