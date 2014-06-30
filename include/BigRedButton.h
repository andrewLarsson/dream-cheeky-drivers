#ifndef BIGREDBUTTON_H
#define BIGREDBUTTON_H

#include "USBDevice.h"
#include <list>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>

#define DREAM_CHEEKY_VENDOR_ID 0x1d34
#define DREAM_CHEEKY_BIG_RED_BUTTON_PRODUCT_ID 0xd
#define DREAM_CHEEKY_BIG_RED_BUTTON_ENDPOINT 0x81
#define DREAM_CHEEKY_BIG_RED_BUTTON_PRESSED 0x16
#define HID_INTERFACE 0x0
#define HID_PACKET_SIZE 8
#define HID_SET_REPORT 0x9
#define HID_REPORT_OUTPUT (0x2 << 0x8) | 0x8
#define HID_REPORT_REQUEST {0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2}
#define HARDWARE_YIELD 100

class BigRedButton
	: public USBDevice {
private:
	bool running;
	std::list<std::function<void()>> eventListeners;
	std::list<std::thread> eventListenerThreads;
	std::thread _driver;
	std::recursive_mutex _lock;
	void _sync(std::function<void()> function);
	void throwEvents();
	void poll(std::chrono::duration<int, std::milli> sleep);
public:
	BigRedButton(std::chrono::duration<int, std::milli> sleep = std::chrono::milliseconds(10), size_t index = 0);
	~BigRedButton();
	void registerEventListener(std::function<void()> eventListener);
	void start(std::chrono::duration<int, std::milli> sleep = std::chrono::milliseconds(10));
	void stop();
};

#endif