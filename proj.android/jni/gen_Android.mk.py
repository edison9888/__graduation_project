#! /usr/bin/env python

import os

template = '''LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MC_shared

LOCAL_MODULE_FILENAME := libMC

LOCAL_CFLAGS += \\
					-DMC_COLLISION_USE_OBB=0 \\
					-DMC_ASTAR_USING_PTHREAD=1 \\
					-DMC_SELECT_ALL_SUPPORT=0 \\
					-DMC_MULTIPLAYER_SUPPORT=0 \\
					-DMC_BATTLE_INFO_LEVEL=0 \\
					-DMC_DEBUG_NON_VISUAL_OBJECTS=1 \\
					-DUSE_FILE32API \\

LOCAL_SRC_FILES := main.cpp \\
%s \\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \\
					$(LOCAL_PATH)/../../Classes/JSON/include \\
					$(LOCAL_PATH)/../../Classes/JSON/include/JsonBox \\
					$(LOCAL_PATH)/../library/CocosDenshion/include \\
					$(LOCAL_PATH)/../library/extensions \\

LOCAL_STATIC_LIBRARIES := curl_static_prebuilt

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,scripting/lua/proj.android/jni)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,extensions)
'''

def main():
	cpp_files = ['					../../Classes/' + f for f in os.listdir("../../Classes/") if f.endswith('.cpp')]
	json_cpp_files = ['					../../Classes/JSON/src/' + f for f in os.listdir("../../Classes/JSON/src/") if f.endswith('.cpp')]
	other_cpp_files = [
	]
	remove_files = []

	for f in cpp_files:
		for o in other_cpp_files:
			if f.endswith(o):
				remove_files.append(f)

	for f in remove_files:
		cpp_files.remove(f)

	for f in json_cpp_files:
		cpp_files.append(f)
	content = template % ' \\\n'.join(cpp_files)
	out = file('Android.mk', 'w')
	out.write(content)
	out.close()

if __name__ == '__main__':
	main()
