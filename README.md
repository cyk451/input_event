# Input Recorder and Replayer

This tiny project is to capture and replay system input events for Android
devices without rooting the phone. If you know Android well enough, AOSP do
provides a tool getevent to do similar thing. The performance was not desired,
so I wrote this binary.

I personally using this to accomplish some mobile game automations. It can
however be maliciously used in stealling sensitive information from mobiles,
like passwords or patterns. You should be fully responsible for any consequence
using this software.

I assume the readers have basic ideas about Android developments, so I won't
explain much on things like steps to install NDK or the way to establish an adb
connection to your computer. There are plenty of web resources for these
topics.

### Build

1. Clone this project
2. Install ndk from [here](https://developer.android.com/ndk/)
3. Run `ndk-build` at project's root folder

### Setup

Connect to your device using adb. Simply execute `bash setup.sh` if you're running a bash.

Optionally, if you don't have a bash in your environment. Run ` adb shell getprop ro.product.cpu.abi ` manually, which should give you the abi your phone using.
Then push binaries into your device, and change them into exectuable.

```bash
adb push ./obj/local/<arch>/event_record /data/local/tmp/
adb push ./obj/local/<arch>/event_replay /data/local/tmp/
adb shell chmod 755 /data/local/tmp/event_*
```

### Usage

To test this application, it's recommended to enable the development option to
visualize input taps first. 

Start a recording into test.bin.
```bash
adb shell /data/local/tmp/event_record /sdcard/test.bin
```

Perform actions, like slide around, tapping home or back.  Halt the program
using ctrl C when you're done. And then try play them back.

```bash
adb shell /data/local/tmp/event_replay /sdcard/test.bin
```

You should see your previous actions are replayed in pretty much
the same time intervals.


### Limitations

1. Portability of recorded events is not of my consideration. The replayer
   assume playing events are all captured from the replaying machine, and hence
the input devices are all in identical order as events are captured.  This
simplifies file format. You may not expect captured events to work from machine
to machine therefore.

2. Ideally, input devices covers physical keys on your devices, including
   headphone jack and its buttons. But that's not neccessary the case. Chances
are your phone maneufacture craving their own way for hardware control.  This
program may not working then. Another case is that the file system permissions
are changed. You need to hack around to see if there is a way to access input
devices. This program is provided in hope that would be useful, but no
guarantee.
