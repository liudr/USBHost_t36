/* USB EHCI Host for Teensy 3.6
 * Copyright 2017 Paul Stoffregen (paul@pjrc.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#include <USBHost_t36.h>  // Read this header first for key info
#include <usb_ch9.h>

#define print   USBHost::print_
#define println USBHost::println_

void printer::init()
{
	contribute_Pipes(mypipes, sizeof(mypipes)/sizeof(Pipe_t));
	contribute_Transfers(mytransfers, sizeof(mytransfers)/sizeof(Transfer_t));
	contribute_String_Buffers(mystring_bufs, sizeof(mystring_bufs)/sizeof(strbuf_t));
	driver_ready_for_device(this);
}

bool printer::claim(Device_t *dev, int type, const uint8_t *descriptors, uint32_t len)
{
	// Claim whole device. The descriptor is always the full conf desc without the 9-byte of conf desc, i.e. intf descs, endp descs, and any class-specifc descs that are included in the full conf desc, such as HIDD.
	println("type = ",type, HEX);
    print_hexbytes(descriptors, len);
	if (len < INTR_DESCR_LEN+EP_DESCR_LEN) return false;    // Should have at least one interface and one end point

    // Here we iterate through each interface and determine whether it is an appropriate printer interface. It must have the right class and subclass and possibly get printer ID to determine that it is NOT a fax machine.

	uint32_t numendpoint = descriptors[INTR_DESC_offset_bNumEndpoints];
	if (numendpoint < 1) return false;
	if (descriptors[INTR_DESC_offset_bInterfaceClass] != USB_CLASS_PRINTER) return false;
	if (descriptors[INTR_DESC_offset_bInterfaceSubClass] != PRINTER_PRINTERS_SUBCLASS) return false;
	if ((descriptors[INTR_DESC_offset_bInterfaceProtocol] != PRINTER_PROTOCOL_UNIDIRECTIONAL)||(descriptors[INTR_DESC_offset_bInterfaceProtocol] != PRINTER_PROTOCOL_BIDIRECTIONAL)) return false;
    // Update from here on
	if (descriptors[INTR_DESCR_LEN+DESC_offset_bLength] != HID_DESC_LEN) return false;
	if (descriptors[INTR_DESCR_LEN+DESC_offset_bDescriptorType] != HID_DESCRIPTOR_HID) return false;
	if (descriptors[INTR_DESCR_LEN+HID_DESC_LEN+DESC_offset_bLength] != EP_DESCR_LEN) return false;
	if (descriptors[INTR_DESCR_LEN+HID_DESC_LEN+DESC_offset_bDescriptorType] != USB_DESCRIPTOR_ENDPOINT) return false; // endpoint descriptor
	uint32_t EP_addr = descriptors[INTR_DESCR_LEN+HID_DESC_LEN+ENDP_DESC_offset_bEndpointAddress];
	println("EP Addr = ", EP_addr, HEX);
	if ((EP_addr & 0xF0) != USB_SETUP_DEVICE_TO_HOST) return false; // must be IN direction
	EP_addr &= 0x0F;
	if (EP_addr == 0) return false;
	if (descriptors[INTR_DESCR_LEN+HID_DESC_LEN+ENDP_DESC_offset_bmAttributes] != 3) return false; // must be interrupt type
	uint32_t size = descriptors[INTR_DESCR_LEN+HID_DESC_LEN+ENDP_DESC_offset_wMaxPacketSize] | (descriptors[INTR_DESCR_LEN+HID_DESC_LEN+ENDP_DESC_offset_wMaxPacketSize+1] << 8);
	println("PK size = ", size);
	if ((size < 8) || (size > 64)) {
		return false; // Keyboard Boot Protocol is 8 bytes, but maybe others have longer... 
	}
#ifdef USBHS_KEYBOARD_INTERVAL 
	uint32_t interval = USBHS_KEYBOARD_INTERVAL;
#else
	uint32_t interval = descriptors[INTR_DESCR_LEN+HID_DESC_LEN+ENDP_DESC_offset_bInterval];
#endif
	println("Interval= ", interval);
	datapipe = new_Pipe(dev, 3, EP_addr, 1, 8, interval);
	datapipe->callback_function = callback;
	queue_Data_Transfer(datapipe, report, 8, this);

    HID_SET_IDLE(setup);//mk_setup(setup, 0x21, 10, 0, 0, 0); // 10=SET_IDLE
	queue_Control_Transfer(dev, &setup, NULL, this);
	control_queued = true;
	println("printer claimed this=", (uint32_t)this, HEX);
	return true;
}

void printer::control(const Transfer_t *transfer)
{
	println("printer control callback");
	control_queued = false;
	print_hexbytes(transfer->buffer, transfer->length);
	uint32_t mesg = transfer->setup.word1;
	println("  mesg = ", mesg, HEX);
	if (mesg == 0x00B21 && transfer->length == 0) { // SET_PROTOCOL
		HID_SET_IDLE(setup);//mk_setup(setup, 0x21, 10, 0, 0, 0); // 10=SET_IDLE
		control_queued = true;
		queue_Control_Transfer(device, &setup, NULL, this);
	}
}

void printer::callback(const Transfer_t *transfer)
{
	//println("printer Callback (static)");
	if (transfer->driver) {
		((printer *)(transfer->driver))->new_data(transfer);
	}
}

void printer::disconnect()
{
	// TODO: free resources
}


void printer::new_data(const Transfer_t *transfer)
{
	println("printer data callback");
	print("  Report: ");
	print_hexbytes(transfer->buffer, 8);
	if (printerReturnsDataFunction) {
        printerReturnsDataFunction((char*)(transfer->buffer));
	}
	memcpy(prev_report, report, 8);
	queue_Data_Transfer(datapipe, report, 8, this);
}
