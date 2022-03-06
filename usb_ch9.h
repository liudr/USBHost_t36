#ifndef _ch9_h_
#define _ch9_h_
#include <stdint.h>

// Configuration descriptor
#define DESC_offset_bLength              0
#define DESC_offset_bDescriptorType      1

#define CONF_DESC_offset_wTotalLength         2
#define CONF_DESC_offset_bNumInterfaces       4
#define CONF_DESC_offset_bConfigurationValue  5
#define CONF_DESC_offset_iConfiguration       6
#define CONF_DESC_offset_bmAttributes         7
#define CONF_DESC_offset_bMaxPower            8

// Interface descriptor
#define INTR_DESC_offset_bInterfaceNumber     2
#define INTR_DESC_offset_bAlternateSetting    3
#define INTR_DESC_offset_bNumEndpoints        4
#define INTR_DESC_offset_bInterfaceClass      5
#define INTR_DESC_offset_bInterfaceSubClass   6
#define INTR_DESC_offset_bInterfaceProtocol   7
#define INTR_DESC_offset_iInterface           8

#define HIDR_DESC_offset_wDescriptorLength		7
// Endpoint descriptor
#define ENDP_DESC_offset_bEndpointAddress       2
#define ENDP_DESC_offset_bmAttributes           3
#define ENDP_DESC_offset_wMaxPacketSize         4
#define ENDP_DESC_offset_bInterval              6

/* Misc.USB constants */
#define DEV_DESCR_LEN	18		//device descriptor length
#define CONF_DESCR_LEN	9		//configuration descriptor length
#define INTR_DESCR_LEN	9		//interface descriptor length
#define EP_DESCR_LEN	7		//endpoint descriptor length
#define USB1_1bcdVER    0x0101  // USB version 1.1
#define USB2_0bcdVER    0x0200  // USB version 2.0

/***
 *     $$$$$$\ $$$$$$$$\ $$$$$$$\                                                
 *    $$  __$$\\__$$  __|$$  __$$\                                               
 *    $$ /  \__|  $$ |   $$ |  $$ |       $$$$$$\   $$$$$$\   $$$$$$\   $$$$$$$\ 
 *    \$$$$$$\    $$ |   $$ |  $$ |      $$  __$$\ $$  __$$\ $$  __$$\ $$  _____|
 *     \____$$\   $$ |   $$ |  $$ |      $$ |  \__|$$$$$$$$ |$$ /  $$ |\$$$$$$\  
 *    $$\   $$ |  $$ |   $$ |  $$ |      $$ |      $$   ____|$$ |  $$ | \____$$\ 
 *    \$$$$$$  |  $$ |   $$$$$$$  |      $$ |      \$$$$$$$\ \$$$$$$$ |$$$$$$$  |
 *     \______/   \__|   \_______/       \__|       \_______| \____$$ |\_______/ 
 *                                                                 $$ |          
 *                                                                 $$ |          
 *                                                                 \__|          
 */

#define USB_REQUEST_GET_STATUS					0		// Standard Device Request - GET STATUS
#define USB_REQUEST_CLEAR_FEATURE				1		// Standard Device Request - CLEAR FEATURE
#define USB_REQUEST_SET_FEATURE					3		// Standard Device Request - SET FEATURE
#define USB_REQUEST_SET_ADDRESS					5		// Standard Device Request - SET ADDRESS
#define USB_REQUEST_GET_DESCRIPTOR				6		// Standard Device Request - GET DESCRIPTOR
#define USB_REQUEST_SET_DESCRIPTOR				7		// Standard Device Request - SET DESCRIPTOR
#define USB_REQUEST_GET_CONFIGURATION			8		// Standard Device Request - GET CONFIGURATION
#define USB_REQUEST_SET_CONFIGURATION			9		// Standard Device Request - SET CONFIGURATION
#define USB_REQUEST_GET_INTERFACE				10		// Standard Device Request - GET INTERFACE
#define USB_REQUEST_SET_INTERFACE				11		// Standard Device Request - SET INTERFACE
#define USB_REQUEST_SYNCH_FRAME					12		// Standard Device Request - SYNCH FRAME

#define USB_FEATURE_ENDPOINT_HALT				0		// CLEAR/SET FEATURE - Endpoint Halt
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP		1		// CLEAR/SET FEATURE - Device remote wake-up
#define USB_FEATURE_TEST_MODE					2		// CLEAR/SET FEATURE - Test mode

/***
 *     $$$$$$\  $$$$$$$$\ $$$$$$$$\ $$\   $$\ $$$$$$$\                                                  $$\     
 *    $$  __$$\ $$  _____|\__$$  __|$$ |  $$ |$$  __$$\                                                 $$ |    
 *    $$ /  \__|$$ |         $$ |   $$ |  $$ |$$ |  $$ |       $$$$$$$\  $$$$$$\  $$$$$$$\   $$$$$$$\ $$$$$$\   
 *    \$$$$$$\  $$$$$\       $$ |   $$ |  $$ |$$$$$$$  |      $$  _____|$$  __$$\ $$  __$$\ $$  _____|\_$$  _|  
 *     \____$$\ $$  __|      $$ |   $$ |  $$ |$$  ____/       $$ /      $$ /  $$ |$$ |  $$ |\$$$$$$\    $$ |    
 *    $$\   $$ |$$ |         $$ |   $$ |  $$ |$$ |            $$ |      $$ |  $$ |$$ |  $$ | \____$$\   $$ |$$\ 
 *    \$$$$$$  |$$$$$$$$\    $$ |   \$$$$$$  |$$ |            \$$$$$$$\ \$$$$$$  |$$ |  $$ |$$$$$$$  |  \$$$$  |
 *     \______/ \________|   \__|    \______/ \__|             \_______| \______/ \__|  \__|\_______/    \____/ 
 */

#define USB_SETUP_HOST_TO_DEVICE				0x00	// Device Request bmRequestType transfer direction - host to device transfer
#define USB_SETUP_DEVICE_TO_HOST				0x80	// Device Request bmRequestType transfer direction - device to host transfer
#define USB_SETUP_TYPE_STANDARD					0x00	// Device Request bmRequestType type - standard
#define USB_SETUP_TYPE_CLASS					0x20	// Device Request bmRequestType type - class
#define USB_SETUP_TYPE_VENDOR					0x40	// Device Request bmRequestType type - vendor
#define USB_SETUP_RECIPIENT_DEVICE				0x00	// Device Request bmRequestType recipient - device
#define USB_SETUP_RECIPIENT_INTERFACE			0x01	// Device Request bmRequestType recipient - interface
#define USB_SETUP_RECIPIENT_ENDPOINT			0x02	// Device Request bmRequestType recipient - endpoint
#define USB_SETUP_RECIPIENT_OTHER				0x03	// Device Request bmRequestType recipient - other
#define bmRequestTypeDataPhaseTransferDir		0x10000000
#define bmRequestTypeType						0x01100000
#define bmRequestTypeRecipient					0x00011111

/* Common setup data constant combinations  */
#define bmREQ_GET_DESCR     (USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_STANDARD|USB_SETUP_RECIPIENT_DEVICE)     // get descriptor request type
#define bmREQ_SET           (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_STANDARD|USB_SETUP_RECIPIENT_DEVICE)     // set request type for all but 'set feature' and 'set interface'
#define bmREQ_CL_GET_INTF   (USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE)     // get interface request type

/***
 *    $$\   $$\  $$$$$$\  $$$$$$$\        $$$$$$$\                             $$$$$$\  $$\                                                   
 *    $$ |  $$ |$$  __$$\ $$  __$$\       $$  __$$\                           $$  __$$\ $$ |                                                  
 *    $$ |  $$ |$$ /  \__|$$ |  $$ |      $$ |  $$ | $$$$$$\ $$\    $$\       $$ /  \__|$$ | $$$$$$\   $$$$$$$\  $$$$$$$\  $$$$$$\   $$$$$$$\ 
 *    $$ |  $$ |\$$$$$$\  $$$$$$$\ |      $$ |  $$ |$$  __$$\\$$\  $$  |      $$ |      $$ | \____$$\ $$  _____|$$  _____|$$  __$$\ $$  _____|
 *    $$ |  $$ | \____$$\ $$  __$$\       $$ |  $$ |$$$$$$$$ |\$$\$$  /       $$ |      $$ | $$$$$$$ |\$$$$$$\  \$$$$$$\  $$$$$$$$ |\$$$$$$\  
 *    $$ |  $$ |$$\   $$ |$$ |  $$ |      $$ |  $$ |$$   ____| \$$$  /        $$ |  $$\ $$ |$$  __$$ | \____$$\  \____$$\ $$   ____| \____$$\ 
 *    \$$$$$$  |\$$$$$$  |$$$$$$$  |      $$$$$$$  |\$$$$$$$\   \$  /         \$$$$$$  |$$ |\$$$$$$$ |$$$$$$$  |$$$$$$$  |\$$$$$$$\ $$$$$$$  |
 *     \______/  \______/ \_______/       \_______/  \_______|   \_/           \______/ \__| \_______|\_______/ \_______/  \_______|\_______/ 
 */
// Start- Borrowed from USBCore.h UHS V2.0 and expanded
// USB Device Classes
#define USB_CLASS_USE_CLASS_INFO        0x00    // Use Class Info in the Interface Descriptors
#define USB_CLASS_AUDIO                 0x01    // Audio
#define USB_CLASS_COM_AND_CDC_CTRL      0x02    // Communications and CDC Control
#define USB_CLASS_HID                   0x03    // HID
#define USB_CLASS_PHYSICAL              0x05    // Physical
#define USB_CLASS_IMAGE                 0x06    // Image
#define USB_CLASS_PRINTER               0x07    // Printer
#define USB_CLASS_MASS_STORAGE          0x08    // Mass Storage
#define USB_CLASS_HUB                   0x09    // Hub
#define USB_CLASS_CDC_DATA              0x0a    // CDC-Data
#define USB_CLASS_SMART_CARD            0x0b    // Smart-Card
#define USB_CLASS_CONTENT_SECURITY      0x0d    // Content Security
#define USB_CLASS_VIDEO                 0x0e    // Video
#define USB_CLASS_PERSONAL_HEALTH       0x0f    // Personal Healthcare
#define USB_CLASS_DIAGNOSTIC_DEVICE     0xdc    // Diagnostic Device
#define USB_CLASS_WIRELESS_CTRL         0xe0    // Wireless Controller
#define USB_CLASS_MISC                  0xef    // Miscellaneous
#define USB_CLASS_APP_SPECIFIC          0xfe    // Application Specific
#define USB_CLASS_VENDOR_SPECIFIC       0xff    // Vendor Specific
// Stop- Borrowed from USBCore.h UHS V2.0 and expanded

/***
 *    $$$$$$$\  $$$$$$$$\  $$$$$$\   $$$$$$\          $$\                                             
 *    $$  __$$\ $$  _____|$$  __$$\ $$  __$$\         $$ |                                            
 *    $$ |  $$ |$$ |      $$ /  \__|$$ /  \__|      $$$$$$\   $$\   $$\  $$$$$$\   $$$$$$\   $$$$$$$\ 
 *    $$ |  $$ |$$$$$\    \$$$$$$\  $$ |            \_$$  _|  $$ |  $$ |$$  __$$\ $$  __$$\ $$  _____|
 *    $$ |  $$ |$$  __|    \____$$\ $$ |              $$ |    $$ |  $$ |$$ /  $$ |$$$$$$$$ |\$$$$$$\  
 *    $$ |  $$ |$$ |      $$\   $$ |$$ |  $$\         $$ |$$\ $$ |  $$ |$$ |  $$ |$$   ____| \____$$\ 
 *    $$$$$$$  |$$$$$$$$\ \$$$$$$  |\$$$$$$  |        \$$$$  |\$$$$$$$ |$$$$$$$  |\$$$$$$$\ $$$$$$$  |
 *    \_______/ \________| \______/  \______/          \____/  \____$$ |$$  ____/  \_______|\_______/ 
 *                                                            $$\   $$ |$$ |                          
 *                                                            \$$$$$$  |$$ |                          
 *                                                             \______/ \__|                          
 */
// USB descriptor types
#define USB_DESCRIPTOR_DEVICE			0x01	// bDescriptorType for a Device Descriptor.
#define USB_DESCRIPTOR_CONFIGURATION	0x02	// bDescriptorType for a Configuration Descriptor.
#define USB_DESCRIPTOR_STRING			0x03	// bDescriptorType for a String Descriptor.
#define USB_DESCRIPTOR_INTERFACE		0x04	// bDescriptorType for an Interface Descriptor.
#define USB_DESCRIPTOR_ENDPOINT			0x05	// bDescriptorType for an Endpoint Descriptor.
#define USB_DESCRIPTOR_DEVICE_QUALIFIER 0x06	// bDescriptorType for a Device Qualifier.
#define USB_DESCRIPTOR_OTHER_SPEED		0x07	// bDescriptorType for a Other Speed Configuration.
#define USB_DESCRIPTOR_INTERFACE_POWER	0x08	// bDescriptorType for Interface Power.
#define USB_DESCRIPTOR_OTG				0x09	// bDescriptorType for an OTG Descriptor.

// OTG SET FEATURE Constants
#define OTG_FEATURE_B_HNP_ENABLE				3		// SET FEATURE OTG - Enable B device to perform HNP
#define OTG_FEATURE_A_HNP_SUPPORT				4		// SET FEATURE OTG - A device supports HNP
#define OTG_FEATURE_A_ALT_HNP_SUPPORT			5		// SET FEATURE OTG - Another port on the A device supports HNP

// USB Endpoint transfer types
#define USB_TRANSFER_TYPE_CONTROL				0x00	// Endpoint is a control endpoint.
#define USB_TRANSFER_TYPE_ISOCHRONOUS			0x01	// Endpoint is an isochronous endpoint.
#define USB_TRANSFER_TYPE_BULK					0x02	// Endpoint is a bulk endpoint.
#define USB_TRANSFER_TYPE_INTERRUPT				0x03	// Endpoint is an interrupt endpoint.
#define bmUSB_TRANSFER_TYPE						0x03	// bit mask to separate transfer type from ISO attributes

// Standard Feature Selectors for CLEAR_FEATURE Requests
#define USB_FEATURE_ENDPOINT_STALL				0		// Endpoint recipient
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP		1		// Device recipient
#define USB_FEATURE_TEST_MODE					2		// Device recipient

// bms in descriptors - Liu 2020-05-11
#define bmAttributes_RESERVED		0x80 // Cr this to rest of bms.
#define bmAttributes_SELF_POWERED	(1<<6)
#define bmAttributes_REMOTE_WAKEUP	(1<<5)

/***
 *    $$\   $$\       $$$$$$\       $$$$$$$\  
 *    $$ |  $$ |      \_$$  _|      $$  __$$\ 
 *    $$ |  $$ |        $$ |        $$ |  $$ |
 *    $$$$$$$$ |        $$ |        $$ |  $$ |
 *    $$  __$$ |        $$ |        $$ |  $$ |
 *    $$ |  $$ |        $$ |        $$ |  $$ |
 *    $$ |  $$ |      $$$$$$\       $$$$$$$  |
 *    \__|  \__|      \______|      \_______/ 
 */

// HID constants. Not part of chapter 9
#define HID_DESC_LEN    9
// Class-specific requests
#define HID_REQUEST_GET_REPORT		0x01
#define HID_REQUEST_GET_IDLE		0x02
#define HID_REQUEST_GET_PROTOCOL	0x03
#define HID_REQUEST_SET_REPORT		0x09
#define HID_REQUEST_SET_IDLE		0x0A
#define HID_REQUEST_SET_PROTOCOL	0x0B

// Class Descriptor Types
#define HID_DESCRIPTOR_HID			0x21
#define HID_DESCRIPTOR_REPORT		0x22
#define HID_DESCRIPTOR_PHY			0x23

// Protocol Selection
#define BOOT_PROTOCOL				0x00
#define RPT_PROTOCOL				0x01

// HID Interface Class Code
#define HID_INTF					USB_CLASS_HID   // Deprecated
// HID Interface Class SubClass Codes
#define BOOT_INTF_SUBCLASS			0x01
// HID Interface Class Protocol Codes
#define HID_PROTOCOL_NONE			0x00
#define HID_PROTOCOL_KEYBOARD		0x01
#define HID_PROTOCOL_MOUSE			0x02

// HID requests
#define bmREQ_HIDOUT        (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE)
#define bmREQ_HIDIN         (USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE) 

#define bmREQ_TYPE_GET_HIDR_DESC			(USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_STANDARD|USB_SETUP_RECIPIENT_INTERFACE) // 0xA1 //Get ASCII ID with compatible print modes
#define bmREQ_TYPE_SET_IDLE					bmREQ_HIDOUT
#define bmREQ_TYPE_SET_PROTOCOL				bmREQ_HIDOUT
#define HID_REQUEST_GET_HIDR_DESC			USB_REQUEST_GET_DESCRIPTOR //Get HID report descriptor is a standard request sent to the interface instead of to the device

#define HID_REPORT_DESC_MAX_LEN     512

/***
 *    $$\   $$\       $$\   $$\       $$$$$$$\  
 *    $$ |  $$ |      $$ |  $$ |      $$  __$$\ 
 *    $$ |  $$ |      $$ |  $$ |      $$ |  $$ |
 *    $$$$$$$$ |      $$ |  $$ |      $$$$$$$\ |
 *    $$  __$$ |      $$ |  $$ |      $$  __$$\ 
 *    $$ |  $$ |      $$ |  $$ |      $$ |  $$ |
 *    $$ |  $$ |      \$$$$$$  |      $$$$$$$  |
 *    \__|  \__|       \______/       \_______/ 
 */

// Hub Requests
#define bmREQ_CLEAR_HUB_FEATURE                 (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE)
#define bmREQ_CLEAR_PORT_FEATURE                (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER)
#define bmREQ_CLEAR_TT_BUFFER                   (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER)
#define bmREQ_GET_HUB_DESCRIPTOR                (USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE)
#define bmREQ_GET_HUB_STATUS                    (USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE)
#define bmREQ_GET_PORT_STATUS                   (USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER)
#define bmREQ_RESET_TT                          (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER)
#define bmREQ_SET_HUB_DESCRIPTOR                (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE)
#define bmREQ_SET_HUB_FEATURE                   (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE)
#define bmREQ_SET_PORT_FEATURE                  (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER)
#define bmREQ_GET_TT_STATE                      (USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER)
#define bmREQ_STOP_TT                           (USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER)

// Hub Class Requests
#define HUB_REQUEST_CLEAR_TT_BUFFER             8
#define HUB_REQUEST_RESET_TT                    9
#define HUB_REQUEST_GET_TT_STATE                10
#define HUB_REQUEST_STOP_TT                     11

// Hub Features
#define HUB_FEATURE_C_HUB_LOCAL_POWER           0
#define HUB_FEATURE_C_HUB_OVER_CURRENT          1
#define HUB_FEATURE_PORT_CONNECTION             0
#define HUB_FEATURE_PORT_ENABLE                 1
#define HUB_FEATURE_PORT_SUSPEND                2
#define HUB_FEATURE_PORT_OVER_CURRENT           3
#define HUB_FEATURE_PORT_RESET                  4
#define HUB_FEATURE_PORT_POWER                  8
#define HUB_FEATURE_PORT_LOW_SPEED              9
#define HUB_FEATURE_C_PORT_CONNECTION           16
#define HUB_FEATURE_C_PORT_ENABLE               17
#define HUB_FEATURE_C_PORT_SUSPEND              18
#define HUB_FEATURE_C_PORT_OVER_CURRENT         19
#define HUB_FEATURE_C_PORT_RESET                20
#define HUB_FEATURE_PORT_TEST                   21
#define HUB_FEATURE_PORT_INDICATOR              22

// Hub Port Test Modes
#define HUB_PORT_TEST_MODE_J                    1
#define HUB_PORT_TEST_MODE_K                    2
#define HUB_PORT_TEST_MODE_SE0_NAK              3
#define HUB_PORT_TEST_MODE_PACKET               4
#define HUB_PORT_TEST_MODE_FORCE_ENABLE         5

// Hub Port Indicator Color
#define HUB_PORT_INDICATOR_AUTO                 0
#define HUB_PORT_INDICATOR_AMBER                1
#define HUB_PORT_INDICATOR_GREEN                2
#define HUB_PORT_INDICATOR_OFF                  3

// Hub Port Status Bitmasks
#define bmHUB_PORT_STATUS_PORT_CONNECTION       0x0001
#define bmHUB_PORT_STATUS_PORT_ENABLE           0x0002
#define bmHUB_PORT_STATUS_PORT_SUSPEND          0x0004
#define bmHUB_PORT_STATUS_PORT_OVER_CURRENT     0x0008
#define bmHUB_PORT_STATUS_PORT_RESET            0x0010
#define bmHUB_PORT_STATUS_PORT_POWER            0x0100
#define bmHUB_PORT_STATUS_PORT_LOW_SPEED        0x0200
#define bmHUB_PORT_STATUS_PORT_HIGH_SPEED       0x0400
#define bmHUB_PORT_STATUS_PORT_TEST             0x0800
#define bmHUB_PORT_STATUS_PORT_INDICATOR        0x1000

// Hub Port Status Change Bitmasks (used one byte instead of two)
#define bmHUB_PORT_STATUS_C_PORT_CONNECTION     0x0001
#define bmHUB_PORT_STATUS_C_PORT_ENABLE         0x0002
#define bmHUB_PORT_STATUS_C_PORT_SUSPEND        0x0004
#define bmHUB_PORT_STATUS_C_PORT_OVER_CURRENT   0x0008
#define bmHUB_PORT_STATUS_C_PORT_RESET          0x0010

// Hub Status Bitmasks (used one byte instead of two)
#define bmHUB_STATUS_LOCAL_POWER_SOURCE         0x01
#define bmHUB_STATUS_OVER_CURRENT               0x12

// Hub Status Change Bitmasks (used one byte instead of two)
#define bmHUB_STATUS_C_LOCAL_POWER_SOURCE       0x01
#define bmHUB_STATUS_C_OVER_CURRENT             0x12


// Hub Port Configuring Substates
#define USB_STATE_HUB_PORT_CONFIGURING          0xb0
#define USB_STATE_HUB_PORT_POWERED_OFF          0xb1
#define USB_STATE_HUB_PORT_WAIT_FOR_POWER_GOOD  0xb2
#define USB_STATE_HUB_PORT_DISCONNECTED         0xb3
#define USB_STATE_HUB_PORT_DISABLED             0xb4
#define USB_STATE_HUB_PORT_RESETTING            0xb5
#define USB_STATE_HUB_PORT_ENABLED              0xb6

// Additional Error Codes
#define HUB_ERROR_PORT_HAS_BEEN_RESET           0xb1

// The bit mask to check for all necessary state bits
#define bmHUB_PORT_STATUS_ALL_MAIN              ((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION | bmHUB_PORT_STATUS_C_PORT_ENABLE | bmHUB_PORT_STATUS_C_PORT_SUSPEND | bmHUB_PORT_STATUS_C_PORT_RESET) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_SUSPEND)

// Bit mask to check for DISABLED state in HubEvent::bmStatus field
#define bmHUB_PORT_STATE_CHECK_DISABLED         (0x0000 | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_SUSPEND)

// Hub Port States
#define bmHUB_PORT_STATE_DISABLED               (0x0000 | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_CONNECTION)

// Hub Port Events
#define bmHUB_PORT_EVENT_CONNECT                (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_CONNECTION)
#define bmHUB_PORT_EVENT_DISCONNECT             (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION) << 16) | bmHUB_PORT_STATUS_PORT_POWER)
#define bmHUB_PORT_EVENT_RESET_COMPLETE         (((0UL | bmHUB_PORT_STATUS_C_PORT_RESET) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION)

#define bmHUB_PORT_EVENT_LS_CONNECT             (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_LOW_SPEED)
#define bmHUB_PORT_EVENT_LS_RESET_COMPLETE      (((0UL | bmHUB_PORT_STATUS_C_PORT_RESET) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_LOW_SPEED)
#define bmHUB_PORT_EVENT_LS_PORT_ENABLED        (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION | bmHUB_PORT_STATUS_C_PORT_ENABLE) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_LOW_SPEED)

#define USB_DESCRIPTOR_HUB						0x29
#define HUB_DESCR_LEN							8		// Liu 2021-01-14 hub descriptor length The last byte is there for compatibilit with USB 1.0 and has no use so 8 is the shortest length although you see 9 on beagle.
#define HUB_STATUS_LEN							4
#define HUB_PORT_STATUS_LEN						4

/***
 *    $$$$$$$\                        $$\                         $$\                                     
 *    $$  __$$\                       \__|                        $$ |                                    
 *    $$ |  $$ |       $$$$$$\        $$\       $$$$$$$\        $$$$$$\          $$$$$$\         $$$$$$\  
 *    $$$$$$$  |      $$  __$$\       $$ |      $$  __$$\       \_$$  _|        $$  __$$\       $$  __$$\ 
 *    $$  ____/       $$ |  \__|      $$ |      $$ |  $$ |        $$ |          $$$$$$$$ |      $$ |  \__|
 *    $$ |            $$ |            $$ |      $$ |  $$ |        $$ |$$\       $$   ____|      $$ |      
 *    $$ |            $$ |            $$ |      $$ |  $$ |        \$$$$  |      \$$$$$$$\       $$ |      
 *    \__|            \__|            \__|      \__|  \__|         \____/        \_______|      \__|      
 */

// Printer Interface Class SubClass Codes
#define PRINTER_PRINTERS_SUBCLASS					        0x01

// PRINTER Interface Class Protocol Codes
#define PRINTER_PROTOCOL_RESERVED					        0x00
#define PRINTER_PROTOCOL_UNIDIRECTIONAL				        0x01
#define PRINTER_PROTOCOL_BIDIRECTIONAL				        0x02
#define PRINTER_PROTOCOL_1284_4_COMPATIBLE_BIDIRECTIONAL    0x03

// Printer class Device Requests

#define bmREQUEST_TYPE_GET_DEVICE_ID			(USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE) // 0xA1 //Get ASCII ID with compatible print modes
#define bmREQUEST_TYPE_GET_PORT_STATUS			(USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE) // 0xA1 //Get 1-byte printer status
#define bmREQUEST_TYPE_SOFT_RESET				(USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE) // 0x21 //RESET

#define PRINTER_REQUEST_GET_DEVICE_ID			0 //Get ASCII ID with compatible print modes
#define PRINTER_REQUEST_GET_PORT_STATUS			1 //Get 1-byte printer status
#define PRINTER_REQUEST_SOFT_RESET				2 //RESET

#define PRINTER_STATUS_OK 						0x18 //Has paper, selected, no errors
#define bmPRINTER_STATUS_PAPER_EMPTY			0b00100000
#define bmPRINTER_STATUS_SELECT					0b00010000
#define bmPRINTER_STATUS_NOT_ERROR				0b00001000
#define PRN_DEVICE_ID_MAX_LEN                   256 // Only request up to this amount of device ID

/***
 *     $$$$$$\    $$\                                     $$\               
 *    $$  __$$\   $$ |                                    $$ |              
 *    $$ /  \__|$$$$$$\    $$$$$$\  $$\   $$\  $$$$$$$\ $$$$$$\    $$$$$$$\ 
 *    \$$$$$$\  \_$$  _|  $$  __$$\ $$ |  $$ |$$  _____|\_$$  _|  $$  _____|
 *     \____$$\   $$ |    $$ |  \__|$$ |  $$ |$$ /        $$ |    \$$$$$$\  
 *    $$\   $$ |  $$ |$$\ $$ |      $$ |  $$ |$$ |        $$ |$$\  \____$$\ 
 *    \$$$$$$  |  \$$$$  |$$ |      \$$$$$$  |\$$$$$$$\   \$$$$  |$$$$$$$  |
 *     \______/    \____/ \__|       \______/  \_______|   \____/ \_______/ 
 */
// Device descriptor structure
typedef struct 
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// DEVICE descriptor type (USB_DESCRIPTOR_DEVICE).
	uint16_t bcdUSB;				// USB Spec Release Number (BCD).
	uint8_t bDeviceClass;			// Class code (assigned by the USB-IF). 0xFF-Vendor specific.
	uint8_t bDeviceSubClass;		// Subclass code (assigned by the USB-IF).
	uint8_t bDeviceProtocol;		// Protocol code (assigned by the USB-IF). 0xFF-Vendor specific.
	uint8_t bMaxPacketSize0;		// Maximum packet size for endpoint 0.
	uint16_t idVendor;				// Vendor ID (assigned by the USB-IF).
	uint16_t idProduct;				// Product ID (assigned by the manufacturer).
	uint16_t bcdDevice;				// Device release number (BCD).
	uint8_t iManufacturer;			// Index of String Descriptor describing the manufacturer.
	uint8_t iProduct;				// Index of String Descriptor describing the product.
	uint8_t iSerialNumber;			// Index of String Descriptor with the device's serial number.
	uint8_t bNumConfigurations;		// Number of possible configurations.
} __attribute__((packed)) USB_DEVICE_DESCRIPTOR; // Added __attribute__((packed)) Liu 2020-05-05

// Configuration descriptor structure
typedef struct
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// CONFIGURATION descriptor type (USB_DESCRIPTOR_CONFIGURATION).
	uint16_t wTotalLength;			// Total length of all descriptors for this configuration.
	uint8_t bNumInterfaces;			// Number of interfaces in this configuration.
	uint8_t bConfigurationValue;	// Value of this configuration (1 based).
	uint8_t iConfiguration;			// Index of String Descriptor describing the configuration.
	uint8_t bmAttributes;			// Configuration characteristics.
	uint8_t bMaxPower;				// Maximum power consumed by this configuration.
} __attribute__((packed)) USB_CONFIGURATION_DESCRIPTOR; // Added __attribute__((packed)) Liu 2020-05-05

// Interface descriptor structure
typedef struct
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// INTERFACE descriptor type (USB_DESCRIPTOR_INTERFACE).
	uint8_t bInterfaceNumber;		// Number of this interface (0 based).
	uint8_t bAlternateSetting;		// Value of this alternate interface setting.
	uint8_t bNumEndpoints;			// Number of endpoints in this interface.
	uint8_t bInterfaceClass;		// Class code (assigned by the USB-IF).	0xFF-Vendor specific.
	uint8_t bInterfaceSubClass;		// Subclass code (assigned by the USB-IF).
	uint8_t bInterfaceProtocol;		// Protocol code (assigned by the USB-IF).  0xFF-Vendor specific.
	uint8_t iInterface;				// Index of String Descriptor describing the interface.
} __attribute__((packed)) USB_INTERFACE_DESCRIPTOR; // Added __attribute__((packed)) Liu 2020-05-05

// Endpoint descriptor structure
typedef struct
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// ENDPOINT descriptor type (USB_DESCRIPTOR_ENDPOINT).
	uint8_t bEndpointAddress;		// Endpoint address. Bit 7 indicates direction (0=OUT, 1=IN).
	uint8_t bmAttributes;			// Endpoint transfer type.
	uint16_t wMaxPacketSize;		// Maximum packet size.
	uint8_t bInterval;				// Polling interval in frames.
} __attribute__((packed)) USB_ENDPOINT_DESCRIPTOR; // Added __attribute__((packed)) Liu 2020-05-05

// + Liu
// Added
/* Full configuration descriptor structure */
typedef struct
{
	USB_CONFIGURATION_DESCRIPTOR * confDesc;
	uint8_t* bData;					// The rest of CONFIGURATION descriptor type (USB_DESCRIPTOR_CONFIGURATION).
} __attribute__((packed)) USB_FULL_CONFIGURATION_DESCRIPTOR;

typedef struct
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// DEVICE descriptor type (USB_DESCRIPTOR_DEVICE).
} __attribute__((packed)) USB_DESCRIPTOR; // This is handy when pointing to a generic descriptor to iterate through descriptors.

// Borrowed from UHS V2
/* HID descriptor */
typedef struct 
{
	uint8_t bLength;					   
	uint8_t bDescriptorType;   
	uint16_t bcdHID;					
	uint8_t bCountryCode;		   
	uint8_t bNumDescriptors;
	uint8_t bDescrType;						   
	uint16_t  wDescriptorLength;
} __attribute__((packed)) USB_HID_DESCRIPTOR; // Added __attribute__((packed)) Liu 2020-05-05

typedef struct 
{
		uint8_t bDescrType; 		// Type of class descriptor
		uint16_t wDescriptorLength;	// Total size of the Report descriptor
} __attribute__((packed)) HID_CLASS_DESCRIPTOR_LEN_AND_TYPE; // Added __attribute__((packed)) Liu 2020-05-05
// - Liu
#endif // _ch9_h_
