TEMPLATE = app
CONFIG += console
CONFIG -= qt

INCLUDEPATH += \
    /home/df/ドキュメント/GitHub/library/cocos2dx/ \
    /home/df/ドキュメント/GitHub/library/cocos2dx/platform/ \
    /home/df/ドキュメント/GitHub/library/cocos2dx/platform/linux/ \
    /home/df/ドキュメント/GitHub/library/cocos2dx/include/ \
    /home/df/ドキュメント/GitHub/library/CocosDenshion/include/ \
    /home/df/ドキュメント/GitHub/library/cocos2dx/kazmath/include/ \
    /home/df/ドキュメント/GitHub/__graduation_project/Classes/JSON/include/\
    /home/df/ドキュメント/GitHub/__graduation_project/Classes/JSON/include/JsonBox/\

LIBS += \
    -L/home/df/ドキュメント/GitHub/library/lib/linux/Debug/ \
    -lcocos2d \
    -lcocosdenshion \
    -lz \
    -lGLEW \

DEFINES += LINUX

SOURCES += \
    ../main.cpp \
    ../../Classes/MCTestScene.cpp \
    ../../Classes/MCTestControllerScene.cpp \
    ../../Classes/MCSplashScene.cpp \
    ../../Classes/MCScript.cpp \
    ../../Classes/MCScenePackage.cpp \
    ../../Classes/MCSceneList.cpp \
    ../../Classes/MCRole.cpp \
    ../../Classes/MCPointExtension.cpp \
    ../../Classes/MCNormalBattleImpl.cpp \
    ../../Classes/MCMutiplayerBattleImpl.cpp \
    ../../Classes/MCMainMenuScene.cpp \
    ../../Classes/MCKeyboard.cpp \
    ../../Classes/MCJSONModifier.cpp \
    ../../Classes/MCJoystick.cpp \
    ../../Classes/MCJoypad.cpp \
    ../../Classes/MCItem.cpp \
    ../../Classes/MCGeometry.cpp \
    ../../Classes/MCGameScene.cpp \
    ../../Classes/MCDungeonMaster.cpp \
    ../../Classes/MCDice.cpp \
    ../../Classes/MCControllerLayer.cpp \
    ../../Classes/MCControllerDelegate.cpp \
    ../../Classes/MCBattle.cpp \
    ../../Classes/AppDelegate.cpp \
    ../../Classes/JSON/src/Value.cpp \
    ../../Classes/JSON/src/SolidusEscaper.cpp \
    ../../Classes/JSON/src/Object.cpp \
    ../../Classes/JSON/src/Indenter.cpp \
    ../../Classes/JSON/src/IndentCanceller.cpp \
    ../../Classes/JSON/src/Escaper.cpp \
    ../../Classes/JSON/src/Convert.cpp \
    ../../Classes/JSON/src/Array.cpp

HEADERS += \
    ../main.h \
    ../../Classes/MCType.h \
    ../../Classes/MCTestScene.h \
    ../../Classes/MCTestControllerScene.h \
    ../../Classes/MCSplashScene.h \
    ../../Classes/MCScript.h \
    ../../Classes/MCScenePackage.h \
    ../../Classes/MCSceneList.h \
    ../../Classes/MCRole.h \
    ../../Classes/MCPointExtension.h \
    ../../Classes/MCObject.h \
    ../../Classes/MCNormalBattleImpl.h \
    ../../Classes/MCMutiplayerBattleImpl.h \
    ../../Classes/MCMainMenuScene.h \
    ../../Classes/MCKeyboard.h \
    ../../Classes/MCJSONModifier.h \
    ../../Classes/MCJoystick.h \
    ../../Classes/MCJoypad.h \
    ../../Classes/MCItem.h \
    ../../Classes/MCGeometry.h \
    ../../Classes/MCGameScene.h \
    ../../Classes/MCDungeonMaster.h \
    ../../Classes/MCDice.h \
    ../../Classes/MCControllerLayer.h \
    ../../Classes/MCControllerDelegate.h \
    ../../Classes/MCBattle.h \
    ../../Classes/AppMacros.h \
    ../../Classes/AppDelegate.h \
    ../../Classes/JSON/include/JsonBox.h \
    ../../Classes/JSON/include/JsonBox/Value.h \
    ../../Classes/JSON/include/JsonBox/SolidusEscaper.h \
    ../../Classes/JSON/include/JsonBox/OutputFilter.h \
    ../../Classes/JSON/include/JsonBox/Object.h \
    ../../Classes/JSON/include/JsonBox/Indenter.h \
    ../../Classes/JSON/include/JsonBox/IndentCanceller.h \
    ../../Classes/JSON/include/JsonBox/Grammar.h \
    ../../Classes/JSON/include/JsonBox/Escaper.h \
    ../../Classes/JSON/include/JsonBox/Convert.h \
    ../../Classes/JSON/include/JsonBox/Array.h

