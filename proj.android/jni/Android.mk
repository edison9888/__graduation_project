LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MC_shared

LOCAL_MODULE_FILENAME := libMC

LOCAL_SRC_FILES := main.cpp \
					../../Classes/AppDelegate.cpp \
					../../Classes/MCBattle.cpp \
					../../Classes/MCControllerDelegate.cpp \
					../../Classes/MCControllerLayer.cpp \
					../../Classes/MCDice.cpp \
					../../Classes/MCDungeonMaster.cpp \
					../../Classes/MCGameScene.cpp \
					../../Classes/MCGeometry.cpp \
					../../Classes/MCItem.cpp \
					../../Classes/MCJoypad.cpp \
					../../Classes/MCJoystick.cpp \
					../../Classes/MCJSONModifier.cpp \
					../../Classes/MCMainMenuScene.cpp \
					../../Classes/MCMutiplayerBattleImpl.cpp \
					../../Classes/MCNormalBattleImpl.cpp \
					../../Classes/MCPointExtension.cpp \
					../../Classes/MCRole.cpp \
					../../Classes/MCSceneList.cpp \
					../../Classes/MCScenePackage.cpp \
					../../Classes/MCScript.cpp \
					../../Classes/MCSplashScene.cpp \
					../../Classes/MCTestControllerScene.cpp \
					../../Classes/MCTestScene.cpp \
					../../Classes/JSON/src/Array.cpp \
					../../Classes/JSON/src/Convert.cpp \
					../../Classes/JSON/src/Escaper.cpp \
					../../Classes/JSON/src/IndentCanceller.cpp \
					../../Classes/JSON/src/Indenter.cpp \
					../../Classes/JSON/src/Object.cpp \
					../../Classes/JSON/src/SolidusEscaper.cpp \
					../../Classes/JSON/src/Value.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/JSON/include \
					$(LOCAL_PATH)/../../Classes/JSON/include/JsonBox \
					$(LOCAL_PATH)/../../../library/CocosDenshion/include \
					$(LOCAL_PATH)/../../../library/extensions \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
