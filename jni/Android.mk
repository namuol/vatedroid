LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE	:= v8_base
LOCAL_SRC_FILES := ../libs/libv8_base.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= v8_nosnapshot
LOCAL_SRC_FILES :=  ../libs/libv8_nosnapshot.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := fiblib
LOCAL_SRC_FILES := fiblib.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := vatewrap
LOCAL_SRC_FILES := vatewrap.cpp V8Runner.cpp V8Value.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_STATIC_LIBRARIES := v8_base v8_nosnapshot
include $(BUILD_SHARED_LIBRARY)
