#include "BigRedButton.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
	if(argc < 2) {
		std::cout << "Runs a command when the Dream Cheeky Big Red Button is pressed." << std::endl << "Usage: big-red-button <command>" << std::endl;
		return 1;
	}
	BigRedButton button;
	button.registerEventListener([&argv]() {
		std::system(argv[1]);
	});
	for(;;) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}
