#ifndef WEBMAILNOTIFIER_H
#define WEBMAILNOTIFIER_H

#include "USBDevice.h"

#define DREAM_CHEEKY_VENDOR_ID 0x1D34
#define DREAM_CHEEKY_WEBMAIL_NOTIFIER_PRODUCT_ID 0x0004
#define DREAM_CHEEKY_WEBMAIL_NOTIFIER_ENDPOINT 0x81
#define HID_INTERFACE 0x00
#define HID_PACKET_SIZE 8
#define HID_SET_REPORT 0x09
#define HID_REPORT_INPUT (0x03 << 0x08) | 0x01
#define DREAM_CHEEKY_WEBMAIL_NOTIFIER_INIT_0 {0x1F, 0x02, 0x00, 0x2E, 0x00, 0x00, 0x2B, 0x03}
#define DREAM_CHEEKY_WEBMAIL_NOTIFIER_INIT_1 {0x00, 0x02, 0x00, 0x2E, 0x00, 0x00, 0x2B, 0x04}
#define DREAM_CHEEKY_WEBMAIL_NOTIFIER_INIT_2 {0x00, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x2B, 0x05}
#define DREAM_CHEEKY_WEBMAIL_NOTIFIER_SET_RGB {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05}
#define HARDWARE_YIELD 100

class WebmailNotifier
	: public USBDevice {
private:
	void init();
public:
	WebmailNotifier();
	WebmailNotifier(size_t index);
	~WebmailNotifier();
	void setRGB(unsigned char red, unsigned char green, unsigned char blue);
};

#endif
