#! /usr/bin/env python

import os

template = '''LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MC_shared

LOCAL_MODULE_FILENAME := libMC

LOCAL_SRC_FILES := main.cpp \\
					../../../library/scripting/lua/lua/lapi.c \\
					../../../library/scripting/lua/lua/lauxlib.c \\
					../../../library/scripting/lua/lua/lbaselib.c \\
					../../../library/scripting/lua/lua/lcode.c \\
					../../../library/scripting/lua/lua/ldblib.c \\
					../../../library/scripting/lua/lua/ldebug.c \\
					../../../library/scripting/lua/lua/ldo.c \\
					../../../library/scripting/lua/lua/ldump.c \\
					../../../library/scripting/lua/lua/lfunc.c \\
					../../../library/scripting/lua/lua/lgc.c \\
					../../../library/scripting/lua/lua/linit.c \\
					../../../library/scripting/lua/lua/liolib.c \\
					../../../library/scripting/lua/lua/llex.c \\
					../../../library/scripting/lua/lua/lmathlib.c \\
					../../../library/scripting/lua/lua/lmem.c \\
					../../../library/scripting/lua/lua/loadlib.c \\
					../../../library/scripting/lua/lua/lobject.c \\
					../../../library/scripting/lua/lua/lopcodes.c \\
					../../../library/scripting/lua/lua/loslib.c \\
					../../../library/scripting/lua/lua/lparser.c \\
					../../../library/scripting/lua/lua/lstate.c \\
					../../../library/scripting/lua/lua/lstring.c \\
					../../../library/scripting/lua/lua/lstrlib.c \\
					../../../library/scripting/lua/lua/ltable.c \\
					../../../library/scripting/lua/lua/ltablib.c \\
					../../../library/scripting/lua/lua/ltm.c \\
					../../../library/scripting/lua/lua/lua.c \\
					../../../library/scripting/lua/lua/lundump.c \\
					../../../library/scripting/lua/lua/lvm.c \\
					../../../library/scripting/lua/lua/lzio.c \\
					../../../library/scripting/lua/lua/print.c \\
					../../../library/scripting/lua/tolua/tolua_event.c \\
					../../../library/scripting/lua/tolua/tolua_is.c \\
					../../../library/scripting/lua/tolua/tolua_map.c \\
					../../../library/scripting/lua/tolua/tolua_push.c \\
					../../../library/scripting/lua/tolua/tolua_to.c \\
					../../../library/scripting/lua/cocos2dx_support/tolua_fix.c \\
					../../../library/scripting/lua/cocos2dx_support/Cocos2dxLuaLoader.cpp \\
					../../../library/scripting/lua/cocos2dx_support/CCLuaEngine.cpp \\
%s \\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \\
					$(LOCAL_PATH)/../../Classes/JSON/include \\
					$(LOCAL_PATH)/../../Classes/JSON/include/JsonBox \\
					$(LOCAL_PATH)/../../../library/CocosDenshion/include \\
					$(LOCAL_PATH)/../../../library/extensions \\
					$(LOCAL_PATH)/../../../library/scripting/lua/cocos2dx_support \\
					$(LOCAL_PATH)/../../../library/scripting/lua/lua \\
					$(LOCAL_PATH)/../../../library/scripting/lua/tolua \\

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
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
