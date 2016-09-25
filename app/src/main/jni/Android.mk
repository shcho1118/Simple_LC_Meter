LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := openslIO
LOCAL_SRC_FILES := opensl_io.c \
                   audioIO.c
LOCAL_LDLIBS := -llog
LOCAL_LDLIBS += -lOpenSLES
LOCAL_LDLIBS += -landroid
include $(BUILD_SHARED_LIBRARY)