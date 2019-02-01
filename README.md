# Input Recorder

This is a small binary capturing system input events for Android devices. I
personally using this to accomplish some mobile game automations. It can
however be used in stealling sensitive information from phones, like passwords
or patterns. You should be fully responsible for any consequence using this
software. 

I assume the readers have basic ideas about Android devices developments, so
here I won't explain much on thing like steps to install NDK or how to
establish an adb connection to your computer. There are plenty of web resources
for these topics.

* Build

	1. Clone this project
	2. Install ndk from [here](https://developer.android.com/ndk/)
	3. Run `ndk-build` at project's root folder

* Setup

	1. Connect to your device using adb.

	2. Simply execute `bash setup.sh` if you're running a bash.

	3. Optionally, if you don't know cpu architecture of the targeting phone.
	   Run below. Which should give you the abi to use.
	```adb shell getprop ro.product.cpu.abi```

	4. Push the binary onto your device, changing them into exectuable.
	```
	adb push ./obj/local/<arch>/event_record /data/local/tmp/
	adb push ./obj/local/<arch>/event_replay /data/local/tmp/
	adb shell chmod 755 /data/local/tmp/event_*
	
	```

* Usage
