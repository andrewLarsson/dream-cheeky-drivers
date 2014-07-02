#!/usr/bin/env bash
g++-4.8 ./src/* -I ./include -I /usr/include/libusb-1.0 -L /usr/lib -lusb-1.0 -std=c++11 -o dream-cheeky-drivers
