LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MC_shared

LOCAL_MODULE_FILENAME := libMC

LOCAL_SRC_FILES := main.cpp \
					../../../library/scripting/lua/lua/lapi.c \
					../../../library/scripting/lua/lua/lauxlib.c \
					../../../library/scripting/lua/lua/lbaselib.c \
					../../../library/scripting/lua/lua/lcode.c \
					../../../library/scripting/lua/lua/ldblib.c \
					../../../library/scripting/lua/lua/ldebug.c \
					../../../library/scripting/lua/lua/ldo.c \
					../../../library/scripting/lua/lua/ldump.c \
					../../../library/scripting/lua/lua/lfunc.c \
					../../../library/scripting/lua/lua/lgc.c \
					../../../library/scripting/lua/lua/linit.c \
					../../../library/scripting/lua/lua/liolib.c \
					../../../library/scripting/lua/lua/llex.c \
					../../../library/scripting/lua/lua/lmathlib.c \
					../../../library/scripting/lua/lua/lmem.c \
					../../../library/scripting/lua/lua/loadlib.c \
					../../../library/scripting/lua/lua/lobject.c \
					../../../library/scripting/lua/lua/lopcodes.c \
					../../../library/scripting/lua/lua/loslib.c \
					../../../library/scripting/lua/lua/lparser.c \
					../../../library/scripting/lua/lua/lstate.c \
					../../../library/scripting/lua/lua/lstring.c \
					../../../library/scripting/lua/lua/lstrlib.c \
					../../../library/scripting/lua/lua/ltable.c \
					../../../library/scripting/lua/lua/ltablib.c \
					../../../library/scripting/lua/lua/ltm.c \
					../../../library/scripting/lua/lua/lua.c \
					../../../library/scripting/lua/lua/lundump.c \
					../../../library/scripting/lua/lua/lvm.c \
					../../../library/scripting/lua/lua/lzio.c \
					../../../library/scripting/lua/lua/print.c \
					../../../library/scripting/lua/tolua/tolua_event.c \
					../../../library/scripting/lua/tolua/tolua_is.c \
					../../../library/scripting/lua/tolua/tolua_map.c \
					../../../library/scripting/lua/tolua/tolua_push.c \
					../../../library/scripting/lua/tolua/tolua_to.c \
					../../../library/scripting/lua/cocos2dx_support/tolua_fix.c \
					../../../library/scripting/lua/cocos2dx_support/Cocos2dxLuaLoader.cpp \
					../../../library/scripting/lua/cocos2dx_support/CCLuaEngine.cpp \
					../../Classes/AppDelegate.cpp \
					../../Classes/MCActionBar.cpp \
					../../Classes/MCAI.cpp \
					../../Classes/MCAStar.cpp \
					../../Classes/MCBackgroundLayer.cpp \
					../../Classes/MCBackpack.cpp \
					../../Classes/MCBarrier.cpp \
					../../Classes/MCBase64.cpp \
					../../Classes/MCBattle.cpp \
					../../Classes/MCBattleController.cpp \
					../../Classes/MCBattleControllerLayer.cpp \
					../../Classes/MCBattleFieldScene.cpp \
					../../Classes/MCControllerDelegate.cpp \
					../../Classes/MCDetailLayer.cpp \
					../../Classes/MCDetailScene.cpp \
					../../Classes/MCDialog.cpp \
					../../Classes/MCDialogue.cpp \
					../../Classes/MCDice.cpp \
					../../Classes/MCDungeonMaster.cpp \
					../../Classes/MCEffect.cpp \
					../../Classes/MCEffectiveItem.cpp \
					../../Classes/MCEnemy.cpp \
					../../Classes/MCEntrance.cpp \
					../../Classes/MCEquipment.cpp \
					../../Classes/MCEquipmentItem.cpp \
					../../Classes/MCFaceBox.cpp \
					../../Classes/MCFlag.cpp \
					../../Classes/MCFlagManager.cpp \
					../../Classes/MCGameScene.cpp \
					../../Classes/MCGameState.cpp \
					../../Classes/MCGeometry.cpp \
					../../Classes/MCHero.cpp \
					../../Classes/MCItem.cpp \
					../../Classes/MCItemManager.cpp \
					../../Classes/MCJoypad.cpp \
					../../Classes/MCJoypadControllerLayer.cpp \
					../../Classes/MCJoystick.cpp \
					../../Classes/MCJSONModifier.cpp \
					../../Classes/MCLuaCocos2d.cpp \
					../../Classes/MCMainMenuScene.cpp \
					../../Classes/MCMercenary.cpp \
					../../Classes/MCMercenaryManager.cpp \
					../../Classes/MCMicsUtil.cpp \
					../../Classes/MCMutiplayerBattleImpl.cpp \
					../../Classes/MCNormalBattleImpl.cpp \
					../../Classes/MCNPC.cpp \
					../../Classes/MCOBB.cpp \
					../../Classes/MCObjectLayer.cpp \
					../../Classes/MCOre.cpp \
					../../Classes/MCOreManager.cpp \
					../../Classes/MCPotion.cpp \
					../../Classes/MCRole.cpp \
					../../Classes/MCRoleBaseInfo.cpp \
					../../Classes/MCRoleEntity.cpp \
					../../Classes/MCRoleManager.cpp \
					../../Classes/MCScene.cpp \
					../../Classes/MCSceneController.cpp \
					../../Classes/MCSceneManager.cpp \
					../../Classes/MCScenePackage.cpp \
					../../Classes/MCScript.cpp \
					../../Classes/MCSemiTransparent.cpp \
					../../Classes/MCSkill.cpp \
					../../Classes/MCSkillManager.cpp \
					../../Classes/MCSplashScene.cpp \
					../../Classes/MCStateLayer.cpp \
					../../Classes/MCTask.cpp \
					../../Classes/MCTaskAccessor.cpp \
					../../Classes/MCTaskBonus.cpp \
					../../Classes/MCTaskContext.cpp \
					../../Classes/MCTaskManager.cpp \
					../../Classes/MCTaskTarget.cpp \
					../../Classes/MCTeam.cpp \
					../../Classes/MCTeamLayer.cpp \
					../../Classes/MCTestAStar.cpp \
					../../Classes/MCTestBattleController.cpp \
					../../Classes/MCTestbed.cpp \
					../../Classes/MCTestController.cpp \
					../../Classes/MCTestGameScene.cpp \
					../../Classes/MCTestLua.cpp \
					../../Classes/MCTestManagers.cpp \
					../../Classes/MCTestOBB.cpp \
					../../Classes/MCTestRoleBaseInfo.cpp \
					../../Classes/MCTestRoleManager.cpp \
					../../Classes/MCTestViewport.cpp \
					../../Classes/MCTrap.cpp \
					../../Classes/MCTrigger.cpp \
					../../Classes/MCViewport.cpp \
					../../Classes/MCViewportLayer.cpp \
					../../Classes/JSON/src/Array.cpp \
					../../Classes/JSON/src/Convert.cpp \
					../../Classes/JSON/src/Escaper.cpp \
					../../Classes/JSON/src/IndentCanceller.cpp \
					../../Classes/JSON/src/Indenter.cpp \
					../../Classes/JSON/src/Object.cpp \
					../../Classes/JSON/src/SolidusEscaper.cpp \
					../../Classes/JSON/src/Value.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/JSON/include \
					$(LOCAL_PATH)/../../Classes/JSON/include/JsonBox \
					$(LOCAL_PATH)/../../../library/CocosDenshion/include \
					$(LOCAL_PATH)/../../../library/extensions \
					$(LOCAL_PATH)/../../../library/scripting/lua/cocos2dx_support \
					$(LOCAL_PATH)/../../../library/scripting/lua/lua \
					$(LOCAL_PATH)/../../../library/scripting/lua/tolua \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
