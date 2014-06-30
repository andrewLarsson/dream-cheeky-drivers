#include "BigRedButton.h"
#include <iostream>

BigRedButton::BigRedButton(std::chrono::duration<int, std::milli> sleep)
	: USBDevice(DREAM_CHEEKY_VENDOR_ID, DREAM_CHEEKY_BIG_RED_BUTTON_PRODUCT_ID) {
	this->init(sleep);
}

BigRedButton::BigRedButton(std::chrono::duration<int, std::milli> sleep, size_t index)
	: USBDevice(DREAM_CHEEKY_VENDOR_ID, DREAM_CHEEKY_BIG_RED_BUTTON_PRODUCT_ID, index) {
	this->init(sleep);
}

BigRedButton::~BigRedButton() {
	this->stop();
	if(!libusb_release_interface(this->device, HID_INTERFACE)) {
		USBDevice::error("Cannot release device interface.");
	}
}

void BigRedButton::init(std::chrono::duration<int, std::milli> sleep) {
	if(libusb_kernel_driver_active(this->device, HID_INTERFACE)) {
		libusb_detach_kernel_driver(this->device, HID_INTERFACE);
	}
	if(libusb_claim_interface(this->device, HID_INTERFACE) < 0) {
		USBDevice::error("Cannot claim device interface.");
	}
	this->running = false;
	this->start(sleep);
}

void BigRedButton::poll(std::chrono::duration<int, std::milli> sleep) {
	unsigned char buffer[HID_PACKET_SIZE];
	unsigned char reportRequest[HID_PACKET_SIZE] = HID_REPORT_REQUEST;
	int bytesTransferred = 0;
	bool buttonPressed = false;
	for(;;) {
		this->_lock.lock();
		if(!this->running) {
			this->_lock.unlock();
			break;
		}
		this->_lock.unlock();
		libusb_control_transfer(
			this->device,
			LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT,
			HID_SET_REPORT,
			HID_REPORT_OUTPUT,
			HID_INTERFACE,
			reportRequest,
			HID_PACKET_SIZE,
			HARDWARE_YIELD
		);
		std::this_thread::sleep_for(sleep);
		libusb_interrupt_transfer(
			this->device,
			DREAM_CHEEKY_BIG_RED_BUTTON_ENDPOINT | LIBUSB_ENDPOINT_IN,
			buffer,
			HID_PACKET_SIZE,
			&bytesTransferred,
			HARDWARE_YIELD
		);
		if(bytesTransferred == HID_PACKET_SIZE) {
			if(buffer[0] == DREAM_CHEEKY_BIG_RED_BUTTON_PRESSED) {
				if(buttonPressed == false) {
					buttonPressed = true;
					this->throwEvents();
				}
			} else {
				buttonPressed = false;
			}
		}
		std::this_thread::sleep_for(sleep);
	}
}

void BigRedButton::_sync(std::function<void()> function) {
	this->_lock.lock();
	function();
	this->_lock.unlock();
}

void BigRedButton::throwEvents() {
	this->_sync([&]() {
		for(std::function<void()> eventListener : this->eventListeners) {
			this->eventListenerThreads.push_back(std::thread(eventListener));
		}
	});
}

void BigRedButton::registerEventListener(std::function<void()> eventListener) {
	this->_sync([&]() {
		this->eventListeners.push_back(eventListener);
	});
}

void BigRedButton::start(std::chrono::duration<int, std::milli> sleep) {
	if(!this->running) {
		this->_sync([&]() {
			this->running = true;
		});
		this->_driver = std::thread(std::bind(&BigRedButton::poll, this, sleep));
	}
}

void BigRedButton::stop() {
	this->_sync([&]() {
		this->running = false;
	});
}
