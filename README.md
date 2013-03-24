# vatedroid
vatedroid (v8 droid) is a simple example project demonstrating how to embed v8 in an Android application through the Android NDK.

The applicationcode itself is a continuation of [marakana's intro to Android NDK](http://marakana.com/s/post/1153/introduction_to_ndk) tutorial series. The original code for that project can be found [here](https://github.com/marakana/FibonacciNative).

## Requirements

* Android SDK
* Android NDK

## Build Instructions

First, you need a copy of the v8 source somewhere.

    git clone git://github.com/v8/v8.git ~/v8

Now you can run the build script (try `./build.sh -h` for details about options)

    ./build.sh -j4 -s ~/v8

*Note:* An Eclipse `.project` is also provided for modifying the UI visually, but you should still use `build.sh` to build
the application, since it handles external v8 dependencies and running `ndk-build`.
