#include "USBDevice.h"
#include <iostream>

USBDevice::USBDevice(uint16_t vendorID, uint16_t productID, size_t index)
	: device(USBDevice::openDevice(vendorID, productID, index)) {
	if(!device) {
		USBDevice::error("Could not open device.");
	}
}

USBDevice::USBDevice(USBDevice::DeviceHandle* device)
	: device(device) {
	if(!device) {
		USBDevice::error("Could not open device.");
	}
}

USBDevice::~USBDevice() {
	libusb_close(this->device);
}

USBDevice::_Initializer::_Initializer()
	: context(USBDevice::_Initializer::initializeLibraryContext()) {
	if(!context) {
		USBDevice::error("Could not initialize USB library.");
	}
	libusb_set_debug(this->context, LIBUSB_LOG_LEVEL_NONE);
}

USBDevice::_Initializer::~_Initializer() {
	libusb_exit(this->context);
}

USBDevice::USBContext* USBDevice::_Initializer::initializeLibraryContext() {
	libusb_context* context = nullptr;
	return ((libusb_init(&context) == 0)
		? context
		: nullptr
	);
}

USBDevice::DeviceHandle* USBDevice::openDevice(uint16_t vendorID, uint16_t productID, size_t index) {
	libusb_device** devices;
	libusb_device* device = nullptr;
	libusb_device_descriptor* deviceDescriptor;
	libusb_device_handle* deviceHandle;
	size_t indexCount = 0;
	ssize_t deviceCount = libusb_get_device_list(USBDevice::_initializer.context, &devices);
	if(deviceCount < 0) {
		libusb_free_device_list(devices, 1);
		return nullptr;
	}
	for(size_t i = 0; i < deviceCount; i ++) {
		if(libusb_get_device_descriptor(devices[i], deviceDescriptor) == 0) {
			if((deviceDescriptor->idVendor == vendorID) && (deviceDescriptor->idProduct == productID)) {
				device = devices[i];
				if(indexCount == index) {
					break;
				}
				indexCount ++;
			}
		}
	}
	libusb_free_device_list(devices, 1);
	if(!device) {
		return nullptr;
	}
	return ((libusb_open(device, &deviceHandle) == 0)
		? deviceHandle
		: nullptr
	);
}

void USBDevice::error(std::string message) {
	std::cerr << message << std::endl;
	std::abort();
}

USBDevice::_Initializer USBDevice::_initializer;
