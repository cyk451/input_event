# Input Recorder

These tiny project is to capture and replay system input events for Android
devices without rooting the phone. If you know Android well enough, AOSP do
provides a tool getevent to do similar thing. The only reason 

I personally using this to accomplish some mobile game automations. It can
however be maliciously used in stealling sensitive information from phones,
like passwords or patterns. You should be fully responsible for any consequence
using this software.

I assume the readers have basic ideas about Android devices developments, so I
won't explain much on things like steps to install NDK or how to establish an
adb connection to your computer. There are plenty of web resources for these
topics.

* Build

	1. Clone this project
	2. Install ndk from [here](https://developer.android.com/ndk/)
	3. Run `ndk-build` at project's root folder

* Setup

	1. Connect to your device using adb.

	2. Simply execute `bash setup.sh` if you're running a bash.

	3. Optionally, if your environment don't have a bash. Run below
	   commands manually, which should give you the abi your phone using.
	```adb shell getprop ro.product.cpu.abi```

	4. Push binaries into your device, change them into exectuable.
	```
	adb push ./obj/local/<arch>/event_record /data/local/tmp/
	adb push ./obj/local/<arch>/event_replay /data/local/tmp/
	adb shell chmod 755 /data/local/tmp/event_*
	
	```

* Usage

	1. To test this application, you may enable the development option to
	   visualize input taps first. Then, start recording.
	   ```adb shell /data/local/tmp/event_record /sdcard/test.bin```

	2. Perform actions, like touch slide around, tapping home or back.
	
	3. Halt the program using ctrl C. And the play them back.
	   ```adb shell /data/local/tmp/event_replay /sdcard/test.bin```
	   You should see your previous actions are replayed pretty much
           exactly the same.
	

* Limitations

	1. Portability of recorded events is not of my consideration. The
	   replayer assume playing events are all captured from the replaying
machine, and hence the input devices are all in identical order as events are
captured.  This simplifies file format. You may not expect captured events to
work from machine to machine therefore.
