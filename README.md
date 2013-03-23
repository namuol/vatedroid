# vatedroid
vatedroid (v8 droid) is a basic sample project demonstrating how to embed v8 in an android application through the android ndk


## requirements

* the v8 source
* Android SDK
* Android NDK

## instructions

First, you need v8 source somewhere.

    git clone git://github.com/v8/v8.git ~/v8

Now you can run the build script (try `./build.sh -h` for details about options)

    ./build.sh -j4 -s ~/v8
