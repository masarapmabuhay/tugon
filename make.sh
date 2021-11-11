#!/bin/bash

# Usbong Tugon System
#
# C/C++ Computer Language and Simple Directmedia Layer (SDL) + SDL Image
#
# Compile and Link Multiple Source Files
#
# @company: USBONG
# @author: SYSON, MICHAEL B.
# @date created: 20211111
# @last updated: 20211111
# @website address: http://www.usbong.ph
#
# Reference:
# 1) https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html;
# last accessed: 20211111

#note: sequence

g++ -c mainLinux.cpp

g++ -o ./output/UsbongTugonLinux mainLinux.o -lSDL2 -lSDL2_image
