#!/bin/bash 

#make clean

adb push ../configs/S835.config /data/local/tmp

make -f Makefile-Android

cd ../bin/x86_64-linux/

adb push ./* /data/local/tmp 


