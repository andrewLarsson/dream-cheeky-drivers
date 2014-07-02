#include "WebmailNotifier.h"

WebmailNotifier::WebmailNotifier()
	: USBDevice(DREAM_CHEEKY_VENDOR_ID, DREAM_CHEEKY_WEBMAIL_NOTIFIER_PRODUCT_ID) {
	this->init();
}

WebmailNotifier::WebmailNotifier(size_t index)
	: USBDevice(DREAM_CHEEKY_VENDOR_ID, DREAM_CHEEKY_WEBMAIL_NOTIFIER_PRODUCT_ID, index) {
	this->init();
}

WebmailNotifier::~WebmailNotifier() {
	this->setRGB(0x00, 0x00, 0x00);
	if(!libusb_release_interface(this->device, HID_INTERFACE)) {
		USBDevice::error("Cannot release device interface.");
	}
}

void WebmailNotifier::init() {
	if(libusb_kernel_driver_active(this->device, HID_INTERFACE)) {
		libusb_detach_kernel_driver(this->device, HID_INTERFACE);
	}
	if(libusb_claim_interface(this->device, HID_INTERFACE) < 0) {
		USBDevice::error("Cannot claim device interface.");
	}
	unsigned char reportInit0[HID_PACKET_SIZE] = DREAM_CHEEKY_WEBMAIL_NOTIFIER_INIT_0;
	unsigned char reportInit1[HID_PACKET_SIZE] = DREAM_CHEEKY_WEBMAIL_NOTIFIER_INIT_1;
	unsigned char reportInit2[HID_PACKET_SIZE] = DREAM_CHEEKY_WEBMAIL_NOTIFIER_INIT_2;
	libusb_control_transfer(
		this->device,
		LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT,
		HID_SET_REPORT,
		HID_REPORT_INPUT,
		HID_INTERFACE,
		reportInit0,
		HID_PACKET_SIZE,
		HARDWARE_YIELD
	);
	libusb_control_transfer(
		this->device,
		LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT,
		HID_SET_REPORT,
		HID_REPORT_INPUT,
		HID_INTERFACE,
		reportInit1,
		HID_PACKET_SIZE,
		HARDWARE_YIELD
	);
	libusb_control_transfer(
		this->device,
		LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT,
		HID_SET_REPORT,
		HID_REPORT_INPUT,
		HID_INTERFACE,
		reportInit2,
		HID_PACKET_SIZE,
		HARDWARE_YIELD
	);
}

void WebmailNotifier::setRGB(unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char reportColor[HID_PACKET_SIZE] = DREAM_CHEEKY_WEBMAIL_NOTIFIER_SET_RGB;
	reportColor[0] = red >> 2;
	reportColor[1] = green >> 2;
	reportColor[2] = blue >> 2;
	libusb_control_transfer(
		this->device,
		LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT,
		HID_SET_REPORT,
		HID_REPORT_INPUT,
		HID_INTERFACE,
		reportColor,
		HID_PACKET_SIZE,
		HARDWARE_YIELD
	);
}
