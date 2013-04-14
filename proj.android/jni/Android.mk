LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MC_shared

LOCAL_MODULE_FILENAME := libMC

LOCAL_SRC_FILES := main.cpp \
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
					../../Classes/MCCamera.cpp \
					../../Classes/MCConfirm.cpp \
					../../Classes/MCControllerDelegate.cpp \
					../../Classes/MCControllerLayer.cpp \
					../../Classes/MCDetail.cpp \
					../../Classes/MCDetailLayer.cpp \
					../../Classes/MCDialog.cpp \
					../../Classes/MCDialogue.cpp \
					../../Classes/MCDice.cpp \
					../../Classes/MCDungeonMaster.cpp \
					../../Classes/MCEffect.cpp \
					../../Classes/MCEffectiveItem.cpp \
					../../Classes/MCEnemy.cpp \
					../../Classes/MCEnhancingEquipmentUI.cpp \
					../../Classes/MCEntrance.cpp \
					../../Classes/MCEquipment.cpp \
					../../Classes/MCEquipmentItem.cpp \
					../../Classes/MCEquipmentLayer.cpp \
					../../Classes/MCEquipmentManager.cpp \
					../../Classes/MCFaceBox.cpp \
					../../Classes/MCFlag.cpp \
					../../Classes/MCFlagManager.cpp \
					../../Classes/MCGameScene.cpp \
					../../Classes/MCGameState.cpp \
					../../Classes/MCGeometry.cpp \
					../../Classes/MCHero.cpp \
					../../Classes/MCHiringMercenaryUI.cpp \
					../../Classes/MCIssuingTaskUI.cpp \
					../../Classes/MCItem.cpp \
					../../Classes/MCItemManager.cpp \
					../../Classes/MCJoypad.cpp \
					../../Classes/MCJoypadControllerLayer.cpp \
					../../Classes/MCJoystick.cpp \
					../../Classes/MCLoadingScene.cpp \
					../../Classes/MCLuaCocos2d.cpp \
					../../Classes/MCMainMenuScene.cpp \
					../../Classes/MCMercenary.cpp \
					../../Classes/MCMercenaryManager.cpp \
					../../Classes/MCMiscUtil.cpp \
					../../Classes/MCMutiplayerBattleImpl.cpp \
					../../Classes/MCNormalBattleImpl.cpp \
					../../Classes/MCNPC.cpp \
					../../Classes/MCOBB.cpp \
					../../Classes/MCObjectLayer.cpp \
					../../Classes/MCOre.cpp \
					../../Classes/MCOreManager.cpp \
					../../Classes/MCPotion.cpp \
					../../Classes/MCPropsLayer.cpp \
					../../Classes/MCRegion.cpp \
					../../Classes/MCRegionManager.cpp \
					../../Classes/MCRole.cpp \
					../../Classes/MCRoleBaseInfo.cpp \
					../../Classes/MCRoleEntity.cpp \
					../../Classes/MCRoleManager.cpp \
					../../Classes/MCRoleProperty.cpp \
					../../Classes/MCScene.cpp \
					../../Classes/MCSceneController.cpp \
					../../Classes/MCSceneManager.cpp \
					../../Classes/MCScenePackage.cpp \
					../../Classes/MCScript.cpp \
					../../Classes/MCSemiTransparent.cpp \
					../../Classes/MCShadow.cpp \
					../../Classes/MCSkill.cpp \
					../../Classes/MCSkillManager.cpp \
					../../Classes/MCSlider.cpp \
					../../Classes/MCSplashScene.cpp \
					../../Classes/MCStateLayer.cpp \
					../../Classes/MCTableViewTextFieldCell.cpp \
					../../Classes/MCTask.cpp \
					../../Classes/MCTaskAccessor.cpp \
					../../Classes/MCTaskBonus.cpp \
					../../Classes/MCTaskContext.cpp \
					../../Classes/MCTaskLayer.cpp \
					../../Classes/MCTaskManager.cpp \
					../../Classes/MCTaskTarget.cpp \
					../../Classes/MCTeam.cpp \
					../../Classes/MCTeamLayer.cpp \
					../../Classes/MCTestAStar.cpp \
					../../Classes/MCTestBattleController.cpp \
					../../Classes/MCTestbed.cpp \
					../../Classes/MCTestController.cpp \
					../../Classes/MCTestDetail.cpp \
					../../Classes/MCTestGameScene.cpp \
					../../Classes/MCTestLua.cpp \
					../../Classes/MCTestManagers.cpp \
					../../Classes/MCTestOBB.cpp \
					../../Classes/MCTestParticle.cpp \
					../../Classes/MCTestRoleBaseInfo.cpp \
					../../Classes/MCTestRoleManager.cpp \
					../../Classes/MCTestUI.cpp \
					../../Classes/MCTestViewport.cpp \
					../../Classes/MCToast.cpp \
					../../Classes/MCTradingPropsUI.cpp \
					../../Classes/MCTrap.cpp \
					../../Classes/MCTrigger.cpp \
					../../Classes/MCViewport.cpp \
					../../Classes/MCViewportLayer.cpp \
					../../Classes/MCVision.cpp \
					../../Classes/MCWeaponSelector.cpp \
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
