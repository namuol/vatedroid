#!/usr/bin/env bash

usage()
{
cat <<EOF
Usage: $0 options...

This script builds v8 against the Android NDK and a sample project skeleton that uses it.
Options:
  -h                  Show this help message and exit
  -s <v8_src>         The path to v8's project sourcetree's root. (default \$V8_SRC_ROOT)
  -n <ndk_dir>        The path to the Android NDK. (default \$ANDROID_NDK_ROOT)
  -t <toolchain_dir>  The path to the Android's toolchain binaries. (default \$ANDROID_TOOLCHAIN)
  -p <platform>       The Android SDK version to support (default android-8)
  -j <num-cpus>       The number of processors to use in building (default 1)
EOF
}

NUM_CPUS=1
PLATFORM_VERSION=android-8
ANT_TARGET=debug

while getopts "hs:n:t:p:j:" OPTION; do
  case $OPTION in
    h)
      usage
      exit
      ;;
    s)
      V8_SRC_ROOT=$OPTARG
      ;;
    n)
      ANDROID_NDK_ROOT=$OPTARG
      ;;
    t)
      ANDROID_TOOLCHAIN=$OPTARG
      ;;
    j)
      NUM_CPUS=$OPTARG
      ;;
    p)
      PLATFORM_VERSION=$OPTARG
      ;;
    ?)
      usage
      exit
      ;;
  esac
done

if [[ -z "$V8_SRC_ROOT" ]]
then
  echo Please set \$V8_SRC_ROOT or use the -s option.
  usage
  exit
fi

if [[ -z "$ANDROID_NDK_ROOT" ]]
then
  echo Please set \$ANDROID_NDK_ROOT or use the -n option.
  usage
  exit
fi

if [[ -z "$ANDROID_TOOLCHAIN" ]]
then
  echo Please set \$ANDROID_TOOLCHAIN or use the -t option.
  usage
  exit
fi

echo Updating/creating android project files...
android update project --target $PLATFORM_VERSION --path .

echo Building v8 for android target...
pushd $V8_SRC_ROOT
make android_arm.release -j4
popd

echo Copying static library files... 
cp $V8_SRC_ROOT/out/android_arm.release/obj.target/tools/gyp/*.a libs/.

echo Copying v8 header files...
cp $V8_SRC_ROOT/include/* include/.

echo Building NDK libraries...
$ANDROID_NDK_ROOT/ndk-build

echo Building $ANT_TARGET APK...
ant $ANT_TARGET
