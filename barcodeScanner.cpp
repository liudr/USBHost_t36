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
	println("BarcodescannerController claim this=", (uint32_t)this, HEX);

	// only claim at interface level
	if (type != 1) return false;
	if (len < 9+9+7) return false;
	print_hexbytes(descriptors, len);

	uint32_t numendpoint = descriptors[4];
	if (numendpoint < 1) return false;
	if (descriptors[5] != 3) return false; // bInterfaceClass, 3 = HID
	if (descriptors[6] != 1) return false; // bInterfaceSubClass, 1 = Boot Device
	if (descriptors[7] != 1) return false; // bInterfaceProtocol, 1 = Keyboard
	if (descriptors[9] != 9) return false;
	if (descriptors[10] != 33) return false; // HID descriptor (ignored, Boot Protocol)
	if (descriptors[18] != 7) return false;
	if (descriptors[19] != 5) return false; // endpoint descriptor
	uint32_t endpoint = descriptors[20];
	println("ep = ", endpoint, HEX);
	if ((endpoint & 0xF0) != 0x80) return false; // must be IN direction
	endpoint &= 0x0F;
	if (endpoint == 0) return false;
	if (descriptors[21] != 3) return false; // must be interrupt type
	uint32_t size = descriptors[22] | (descriptors[23] << 8);
	println("packet size = ", size);
	if ((size < 8) || (size > 64)) {
		return false; // Keyboard Boot Protocol is 8 bytes, but maybe others have longer... 
	}
#ifdef USBHS_KEYBOARD_INTERVAL 
	uint32_t interval = USBHS_KEYBOARD_INTERVAL;
#else
	uint32_t interval = descriptors[24];
#endif
	println("polling interval = ", interval);
	datapipe = new_Pipe(dev, 3, endpoint, 1, 8, interval);
	datapipe->callback_function = callback;
	queue_Data_Transfer(datapipe, report, 8, this);

    mk_setup(setup, 0x21, 10, 0, 0, 0); // 10=SET_IDLE
	queue_Control_Transfer(dev, &setup, NULL, this);
	control_queued = true;
	return true;
}

void BarcodescannerController::control(const Transfer_t *transfer)
{
	println("control callback (keyboard)");
	control_queued = false;
	print_hexbytes(transfer->buffer, transfer->length);
	// To decode hex dump to human readable HID report summary:
	//   http://eleccelerator.com/usbdescreqparser/
	uint32_t mesg = transfer->setup.word1;
	println("  mesg = ", mesg, HEX);
	if (mesg == 0x00B21 && transfer->length == 0) { // SET_PROTOCOL
		mk_setup(setup, 0x21, 10, 0, 0, 0); // 10=SET_IDLE
		control_queued = true;
		queue_Control_Transfer(device, &setup, NULL, this);
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
	println("BarcodescannerController Callback (member)");
	print("  KB Data: ");
	print_hexbytes(transfer->buffer, 8);
	if (codeScannedFunction) {
        codeScannedFunction((char*)(transfer->buffer));
	}
	memcpy(prev_report, report, 8);
	queue_Data_Transfer(datapipe, report, 8, this);
}
