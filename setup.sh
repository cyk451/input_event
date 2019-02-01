#! /bin/bash

ABI=$(adb shell getprop ro.product.cpu.abi)
REMOTE=/data/local/tmp

adb push ./obj/local/$ABI/event_record $REMOTE
adb push ./obj/local/$ABI/event_replay $REMOTE
adb shell chmod 755 $REMOTE/event_*
adb shell ls -l $REMOTE/event_*
