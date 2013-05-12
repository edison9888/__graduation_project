APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -DCOCOS2D_DEBUG=1
APP_CPPFLAGS += -DCC_ENABLE_BOX2D_INTEGRATION
APP_CPPFLAGS += -DMC_COLLISION_USE_OBB=0
APP_CPPFLAGS += -DMC_ASTAR_USING_PTHREAD=1
APP_CPPFLAGS += -DMC_SELECT_ALL_SUPPORT=0
APP_CPPFLAGS += -DMC_MULTIPLAYER_SUPPORT=0
APP_CPPFLAGS += -DMC_BATTLE_INFO_LEVEL=0
APP_CPPFLAGS += -DMC_DEBUG_NON_VISUAL_OBJECTS=0
APP_CPPFLAGS += -DUSE_FILE32API
APP_CPPFLAGS += -fexceptions
