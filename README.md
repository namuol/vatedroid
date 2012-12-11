# vatedroid
vatedroid (v8 droid) is a basic sample project demonstrating how to embed v8 in an android application through the android ndk


## requirements

* the v8 source

* Android SDK

* Android NDK

## instructions

* create an android project with the vatedroid source

    android update project --name vatedroid --target <your call>

* compile the v8 source with for the android arm architecture

run the following from the root directory of the v8 source

    make android_arm.release

* locate the v8 libraries generated by building v8

they are buried, start looking in

    path/to/v8/out/android_arm.release/obj.target/

* copy the v8 static libraries to the libs directory of the android project

copy both libv8_base.a and libv8_nosnapshot.a with something like the following

    cp path/to/the/v8/libs/libv8_base.a path/to/your/android/project/libs/libv8_base.a

* build the ndk shared library

    ndk-build

the ndk root directory must be availabe on your path for the above to work

* build the android project

    ant debug

or

    ant release

* install on device or simulator

    adb install <project.apk>

where project.apk is the filename ofthe apk file generated by the previous build step.
