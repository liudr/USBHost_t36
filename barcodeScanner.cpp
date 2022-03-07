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

void BarcodescannerController::init()
{
	contribute_Pipes(mypipes, sizeof(mypipes)/sizeof(Pipe_t));
	contribute_Transfers(mytransfers, sizeof(mytransfers)/sizeof(Transfer_t));
	contribute_String_Buffers(mystring_bufs, sizeof(mystring_bufs)/sizeof(strbuf_t));
	driver_ready_for_device(this);
}

bool BarcodescannerController::claim(Device_t *dev, int type, const uint8_t *descriptors, uint32_t len)
{
    USB_INTERFACE_DESCRIPTOR* intDesc;
    USB_HID_DESCRIPTOR* hidDesc;
    USB_ENDPOINT_DESCRIPTOR* epDesc;
	// Claim whole device. The descriptor is always the full conf desc without the 9-byte of conf desc, i.e. intf descs, endp descs, and any class-specifc descs that are included in the full conf desc, such as HIDD.
	if (type != claim_type_device) return false;
	println("type = ",type, HEX);
    print_hexbytes(descriptors, len);
	if (len < INTR_DESCR_LEN+HID_DESC_LEN+EP_DESCR_LEN) return false;
    intDesc=(USB_INTERFACE_DESCRIPTOR*)descriptors;
    hidDesc=(USB_HID_DESCRIPTOR*)(descriptors+INTR_DESCR_LEN);
    epDesc=(USB_ENDPOINT_DESCRIPTOR*)(descriptors+INTR_DESCR_LEN+HID_DESC_LEN);
	uint32_t numendpoint = intDesc->bNumEndpoints;
	if (numendpoint < 1) return false;
	if (intDesc->bInterfaceClass != USB_CLASS_HID) return false;
	if (intDesc->bInterfaceSubClass != BOOT_INTF_SUBCLASS) return false;
	if (intDesc->bInterfaceProtocol != RPT_PROTOCOL) return false;
	if (hidDesc->bLength != HID_DESC_LEN) return false;
	if (hidDesc->bDescriptorType != HID_DESCRIPTOR_HID) return false;
	if (epDesc->bLength != EP_DESCR_LEN) return false;
	if (epDesc->bDescriptorType != USB_DESCRIPTOR_ENDPOINT) return false;
	uint32_t EP_addr = epDesc->bEndpointAddress;    // Don't mask the dir bit yet.
	println("EP Addr = ", EP_addr, HEX);
	if ((EP_addr & 0xF0) != USB_SETUP_DEVICE_TO_HOST) return false; // must be IN direction
	EP_addr &= 0x0F;    // Now mask the dir bit
	if (EP_addr == 0) return false;
	if (epDesc->bmAttributes != USB_TRANSFER_TYPE_INTERRUPT) return false; // must be interrupt type
    wDescriptorLength=hidDesc->wDescriptorLength;
	uint32_t size = epDesc->wMaxPacketSize;
	println("PK size = ", size);
	if ((size < 8) || (size > 64)) {
		return false; // Keyboard Boot Protocol is 8 bytes, but maybe others have longer... 
	}
#ifdef USBHS_KEYBOARD_INTERVAL 
	uint32_t interval = USBHS_KEYBOARD_INTERVAL;
#else
	uint32_t interval = epDesc->bInterval;
#endif
	println("Interval= ", interval);
	datapipe = new_Pipe(dev, 3, EP_addr, 1, 8, interval);
	datapipe->callback_function = callback;
	queue_Data_Transfer(datapipe, report, 8, this);

    mk_HID_SET_IDLE(setup);//mk_setup(setup, 0x21, 10, 0, 0, 0); // 10=SET_IDLE
	queue_Control_Transfer(dev, &setup, NULL, this);
	control_queued = true;
	println("BSC claimed this=", (uint32_t)this, HEX);
	return true;
}

void BarcodescannerController::control(const Transfer_t *transfer)
{
    uint32_t len = transfer->length - USBHost::ctrldata_bytes_to_transfer;
	uint32_t mesg = transfer->setup.word1;
	USBHost::ctrldata_bytes_to_transfer=0;	// Set it to zero just in case it's not so it only gets used once and next time won't affect length.
	println("BSC CTRL CB. qh.token:", uint32_t(transfer->qtd.token), HEX);
	//println("HIDR:", (bmREQ_TYPE_GET_HIDR_DESC+(HID_REQUEST_GET_HIDR_DESC<<8U)+(HID_DESCRIPTOR_REPORT<<24U)), HEX);
	println("msg:", mesg, HEX);
	control_queued = false;
	if (mesg == 0x00B21 && transfer->length == 0) { // SET_PROTOCOL
		mk_HID_SET_IDLE(setup);//mk_setup(setup, 0x21, 10, 0, 0, 0); // 10=SET_IDLE
		control_queued = true;
		queue_Control_Transfer(device, &setup, NULL, this);
	}
	else if (mesg == HID_SET_IDLE_MSG) { // SET_IDLE
		//mk_HID_GET_REPORT_DESC(setup,0,wDescriptorLength);
		mk_HID_GET_REPORT_DESC(setup,0,HID_REPORT_DESC_MAX_LEN);
		control_queued = true;
		queue_Control_Transfer(device, &setup, hid_rep_desc_buf, this, true);	// Enable ctrl xfer data stage interrupt to capture bytes-to-transfer
	}
	else if (mesg == HID_GET_REPORT_DESC_MSG) { // GET_HIDR_DESC
        println("Len:",len);
        print_hexbytes(transfer->buffer, len);
	}
}

void BarcodescannerController::callback(const Transfer_t *transfer)
{
	//println("BarcodescannerController Callback (static)");
	if (transfer->driver) {
		((BarcodescannerController *)(transfer->driver))->new_data(transfer);
	}
}

void BarcodescannerController::disconnect()
{
	// TODO: free resources
}


// Arduino defined this static weak symbol callback, and their
// examples use it as the only way to detect new key presses,
// so unfortunate as static weak callbacks are, it probably
// needs to be supported for compatibility
extern "C" {
void __keyboardControllerEmptyCallback() { }
}
void keyPressed()  __attribute__ ((weak, alias("__keyboardControllerEmptyCallback")));
void keyReleased() __attribute__ ((weak, alias("__keyboardControllerEmptyCallback")));

void BarcodescannerController::new_data(const Transfer_t *transfer)
{
    uint32_t len = transfer->length - (((transfer->qtd.token) >> 16) & 0x7FFF);
	println("BSC DATA CB. qh.token:", uint32_t(transfer->qtd.token), HEX);
    println("Len:",len);
	print("Report:");
	print_hexbytes(transfer->buffer, len);
	if (codeScannedFunction) {
        codeScannedFunction((char*)(transfer->buffer));
	}
	memcpy(prev_report, report, len);
	queue_Data_Transfer(datapipe, report, 8, this);
}
