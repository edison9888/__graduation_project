LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Adventure_shared

LOCAL_MODULE_FILENAME := libAdventure

LOCAL_SRC_FILES := Adventure/main.cpp \
					../../Classes/AppDelegate.cpp \
					../../Classes/CCSpiritedJoystick.cpp \
					../../Classes/HelpScene.cpp \
					../../Classes/SplashScene.cpp \
					../../Classes/CCButton.cpp \
					../../Classes/CCSpriteColor.cpp \
					../../Classes/MenuItemLabelColor.cpp \
					../../Classes/TestScene.cpp \
					../../Classes/CCJoypad.cpp \
					../../Classes/ExitDialog.cpp \
					../../Classes/MenuScene.cpp \
					../../Classes/Touch.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../library/CocosDenshion/include \
					$(LOCAL_PATH)/../../../library/extensions \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)

