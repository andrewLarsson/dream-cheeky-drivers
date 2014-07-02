#ifndef BIGREDBUTTON_H
#define BIGREDBUTTON_H

#include "USBDevice.h"
#include <list>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>

#define DREAM_CHEEKY_VENDOR_ID 0x1D34
#define DREAM_CHEEKY_BIG_RED_BUTTON_PRODUCT_ID 0x000D
#define DREAM_CHEEKY_BIG_RED_BUTTON_ENDPOINT 0x81
#define DREAM_CHEEKY_BIG_RED_BUTTON_PRESSED 0x16
#define HID_INTERFACE 0x00
#define HID_PACKET_SIZE 8
#define HID_SET_REPORT 0x09
#define HID_REPORT_OUTPUT (0x02 << 0x08) | 0x08
#define HID_REPORT_REQUEST {0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}
#define HARDWARE_YIELD 100

class BigRedButton
	: public USBDevice {
private:
	bool running;
	std::list<std::function<void()>> pressedEventListeners;
	std::list<std::function<void()>> releasedEventListeners;
	std::list<std::thread> eventListenerThreads;
	std::thread _driver;
	std::recursive_mutex _lock;
	void init(std::chrono::duration<int, std::milli> sleep);
	void _sync(std::function<void()> function);
	void throwPressedEvent();
	void throwReleasedEvent();
	void poll(std::chrono::duration<int, std::milli> sleep);
public:
	BigRedButton(std::chrono::duration<int, std::milli> sleep);
	BigRedButton(std::chrono::duration<int, std::milli> sleep, size_t index);
	~BigRedButton();
	void registerPressedEventListener(std::function<void()> eventListener);
	void registerReleasedEventListener(std::function<void()> eventListener);
	void start(std::chrono::duration<int, std::milli> sleep);
	void stop();
};

#endif
