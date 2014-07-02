#include "WebmailNotifier.h"
#include "BigRedButton.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
	WebmailNotifier notifier;
	BigRedButton button(std::chrono::milliseconds(10));
	button.registerPressedEventListener([&notifier]() {
		notifier.setRGB(0xFF, 0x00, 0x00);
	});
	button.registerReleasedEventListener([&notifier]() {
		notifier.setRGB(0x00, 0x00, 0x00);
	});
	for(;;) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}
