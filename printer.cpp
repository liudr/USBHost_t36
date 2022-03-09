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

void Printer::init()
{
	ready=0;
	contribute_Pipes(mypipes, sizeof(mypipes)/sizeof(Pipe_t));
	contribute_Transfers(mytransfers, sizeof(mytransfers)/sizeof(Transfer_t));
	contribute_String_Buffers(mystring_bufs, sizeof(mystring_bufs)/sizeof(strbuf_t));
	driver_ready_for_device(this);
	printer_device_id=(uint8_t*)malloc(PRN_DEVICE_ID_MAX_LEN);
}

bool Printer::claim(Device_t *dev, int type, const uint8_t *descriptors, uint32_t len)
{
    USB_INTERFACE_DESCRIPTOR* intDesc;
    USB_ENDPOINT_DESCRIPTOR* epDesc;
    intDesc=(USB_INTERFACE_DESCRIPTOR*)descriptors;
    epDesc=(USB_ENDPOINT_DESCRIPTOR*)(descriptors+INTR_DESCR_LEN);

	intfNum		=	intDesc->bInterfaceNumber;
	// Claim whole device. The descriptor is always the full conf desc without the 9-byte of conf desc, i.e. intf descs, endp descs, and any class-specifc descs that are included in the full conf desc, such as HIDD.
	println("type = ",type, HEX);
	print_hexbytes(descriptors, len);
	if (len < INTR_DESCR_LEN+EP_DESCR_LEN) return false;    // Should have at least one interface and one end point

	// Here we iterate through each interface and determine whether it is an appropriate printer interface. It must have the right class and subclass and possibly get printer ID to determine that it is NOT a fax machine.

	uint32_t numendpoint = descriptors[INTR_DESC_offset_bNumEndpoints];
	if (numendpoint < 1) return false;
	if (intDesc->bInterfaceClass != USB_CLASS_PRINTER) return false;
	if (intDesc->bInterfaceSubClass != PRINTER_PRINTERS_SUBCLASS) return false;
	if (!((intDesc->bInterfaceProtocol == PRINTER_PROTOCOL_UNIDIRECTIONAL)||(intDesc->bInterfaceProtocol == PRINTER_PROTOCOL_BIDIRECTIONAL)||(intDesc->bInterfaceProtocol == PRINTER_PROTOCOL_1284_4_COMPATIBLE_BIDIRECTIONAL))) return false;
    // Update from here on. Need to do better job to skip over INT EP among the EPs under an interface.
	println("EPAddr:", (epDesc->bEndpointAddress)&0x0F, HEX);
	if ((epDesc->bEndpointAddress)&USB_SETUP_DEVICE_TO_HOST)	// First EP is IN, then OUT
	{
		EPAddrIN=(epDesc->bEndpointAddress)&0x0F;
		EPINwMaxPacketSize=epDesc->wMaxPacketSize;
		intervalIN=epDesc->bInterval;
		epDesc++;
		EPAddrOUT=(epDesc->bEndpointAddress)&0x0F;
		EPOUTwMaxPacketSize=epDesc->wMaxPacketSize;
		intervalOUT=epDesc->bInterval;
	}
	else	// First EP is OUT, then IN
	{
		EPAddrOUT=(epDesc->bEndpointAddress)&0x0F;
		EPOUTwMaxPacketSize=epDesc->wMaxPacketSize;
		intervalOUT=epDesc->bInterval;
		epDesc++;
		EPAddrIN=(epDesc->bEndpointAddress)&0x0F;
		EPINwMaxPacketSize=epDesc->wMaxPacketSize;
		intervalIN=epDesc->bInterval;
	}
	INdatapipe = new_Pipe(dev, pipe_type_bulk, EPAddrIN, pipe_direction_IN, EPINwMaxPacketSize, intervalIN);
	INdatapipe->callback_function = callback;
	OUTdatapipe = new_Pipe(dev, pipe_type_bulk, EPAddrOUT, pipe_direction_OUT, EPOUTwMaxPacketSize, intervalIN);
	OUTdatapipe->callback_function = callback;
	/*
	queue_Data_Transfer(datapipe, report, 8, this);
*/
    mk_PRN_GET_DEVICE_ID(setup,0,intfNum,PRN_DEVICE_ID_MAX_LEN);
	queue_Control_Transfer(dev, &setup, printer_device_id, this,true);	// Enable ctrl xfer data stage interrupt to capture bytes-to-transfer
	control_queued = true;
	println("PRN claimed this=", (uint32_t)this, HEX);
	return true;
}

void Printer::control(const Transfer_t *transfer)
{
    uint32_t len = transfer->length - USBHost::ctrldata_bytes_to_transfer;
	uint32_t mesg = transfer->setup.word1;
	USBHost::ctrldata_bytes_to_transfer=0;	// Set it to zero just in case it's not so it only gets used once and next time won't affect length.
	control_queued = false;
	println("PRN CTRL CB. qh.token:", transfer->qtd.token, HEX);
	println("msg:", mesg, HEX);

	if (mesg == 0x00B21 && transfer->length == 0) {
		mk_HID_SET_IDLE(setup);
		control_queued = true;
		queue_Control_Transfer(device, &setup, NULL, this);
	}
	else if ((transfer->setup.bmRequestType==bmREQUEST_TYPE_GET_DEVICE_ID)&&(transfer->setup.bRequest==PRINTER_REQUEST_GET_DEVICE_ID))
    {
		println("Len:",len);
		print_hexbytes(transfer->buffer, len);
		printer_device_id=(uint8_t*)realloc((void*)printer_device_id,len);
		memcpy((void*)printer_device_id,(void*)transfer->buffer,len);
		printer_device_id_len=len;
		ready=true;	// Device is now ready for use.
	}
}

void Printer::callback(const Transfer_t *transfer)
{
	//println("printer Callback (static)");
	if (transfer->driver) {
		((Printer *)(transfer->driver))->new_data(transfer);
	}
}

void Printer::disconnect()
{
	if(printer_device_id) free(printer_device_id);
}

bool Printer::print_data(const uint8_t* data, size_t len)
{
    ready=false;
	queue_Data_Transfer(OUTdatapipe, (uint8_t*)data, len, this);
}

void Printer::new_data(const Transfer_t *transfer)
{
    if (transfer->pipe->direction==pipe_direction_IN) {
    	println("PRN data rcvd:",transfer->length);
	    print_hexbytes(transfer->buffer, transfer->length);
    }
    if (transfer->pipe->direction==pipe_direction_OUT) {
        ready=true;
    	println("PRN data sent:",transfer->length);
    }
	if (printerReturnsDataFunction) {
        printerReturnsDataFunction((uint8_t*)(transfer->buffer));
	}
}
