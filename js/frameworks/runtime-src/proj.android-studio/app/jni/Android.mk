LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2djs_shared

LOCAL_MODULE_FILENAME := libcocos2djs

LOCAL_SRC_FILES := hellojavascript/main.cpp ../../../Classes/AppDelegate.cpp ../../../Classes/PluginFacebookJS.cpp ../../../Classes/PluginFacebookJS.hpp ../../../Classes/PluginFacebookJSHelper.cpp ../../../Classes/PluginFacebookJSHelper.h ../../../Classes/SDKBoxJSHelper.cpp ../../../Classes/SDKBoxJSHelper.h

LOCAL_LDLIBS := -landroid -llog
LOCAL_CPPFLAGS := -DSDKBOX_ENABLED
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes
LOCAL_WHOLE_STATIC_LIBRARIES := PluginFacebook sdkbox

LOCAL_STATIC_LIBRARIES := cocos2d_js_static

LOCAL_EXPORT_CFLAGS := -DCOCOS2D_DEBUG=2 -DCOCOS2D_JAVASCRIPT

include $(BUILD_SHARED_LIBRARY)
$(call import-add-path, $(LOCAL_PATH))


$(call import-module, scripting/js-bindings/proj.android)
$(call import-module, ./sdkbox)
$(call import-module, ./pluginfacebook)
