#include <iostream>
#include <iomanip>
#include <thread>
#include <cstdlib>
#include <libusb.h>

#define PACKET_SIZE 8

int main(int argc, char* argv[]) {
	if(argc < 2) {
		std::cout << "Runs a command when the Dream Cheeky Big Red Button is pressed." << std::endl << "Usage: big-red-button <command>" << std::endl;
		return 1;
	}
	libusb_device_handle *deviceHandle;
	libusb_context *usbContext = NULL;
	unsigned char data[PACKET_SIZE];
	unsigned char setReport[PACKET_SIZE] = {0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2};
	int bytesTransferred = 0;
	bool buttonPressed = false;
	if(libusb_init(&usbContext) < 0) {
		std::cerr << "Cannot initialize libusb." << std::endl;
		return 1;
	}
	deviceHandle = libusb_open_device_with_vid_pid(usbContext, 0x1d34, 0xd);
	if(!deviceHandle) {
		std::cerr << "Cannot open device." << std::endl;
		return 1;
	}
	if(libusb_kernel_driver_active(deviceHandle, 0x0)) {
		libusb_detach_kernel_driver(deviceHandle, 0x0);
	}
	if(libusb_claim_interface(deviceHandle, 0x0) < 0) {
		std::cerr << "Cannot claim device interface." << std::endl;
		return 1;
	}
	for(;;) {
		libusb_control_transfer(
			deviceHandle,
			LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT,
			0x9,
			(0x2 << 0x8) | 0x8,
			0x0,
			setReport,
			PACKET_SIZE,
			100
		);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		libusb_interrupt_transfer(
			deviceHandle,
			0x81 | LIBUSB_ENDPOINT_IN,
			data,
			PACKET_SIZE,
			&bytesTransferred,
			100
		);
		if(bytesTransferred) {
			if(data[0] == 0x16) {
				if(buttonPressed == false) {
					buttonPressed = true;
					std::system(argv[1]);
				}
			} else {
				buttonPressed = false;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	if(!libusb_release_interface(deviceHandle, 0x0)) {
		std::cerr << "Cannot release device interface." << std::endl;
		return 1;
	}
	libusb_close(deviceHandle);
	libusb_exit(usbContext);
	return 0;
}
