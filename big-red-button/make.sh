#!/usr/bin/env bash
g++-4.8 ./main.cpp -std=c++11 -I /usr/include/libusb-1.0 -L /usr/lib -lusb-1.0 -o big-red-button
