#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <libusb.h>
#include <stdint.h>
#include <string>

class USBDevice {
public:
	typedef libusb_device_handle DeviceHandle;
	typedef libusb_context USBContext;
private:
	static USBDevice::DeviceHandle* openDevice(uint16_t vendorID, uint16_t productID);
	static USBDevice::DeviceHandle* openDevice(uint16_t vendorID, uint16_t productID, size_t index);
	void init();
protected:
	USBDevice::DeviceHandle* device;
	static class _Initializer {
	private:
		static USBDevice::USBContext* initializeLibraryContext();
	public:
		USBDevice::USBContext* context;
		_Initializer();
		~_Initializer();
	} _initializer;
	static void error(std::string message);
public:
	USBDevice(uint16_t vendorID, uint16_t productID);
	USBDevice(uint16_t vendorID, uint16_t productID, size_t index);
	USBDevice(USBDevice::DeviceHandle* device);
	virtual ~USBDevice();
};

#endif
