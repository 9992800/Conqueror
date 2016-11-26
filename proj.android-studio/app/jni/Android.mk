LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)
$(call import-add-path,$(LOCAL_PATH))

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_WHOLE_STATIC_LIBRARIES += PluginIAP
LOCAL_WHOLE_STATIC_LIBRARIES += PluginFacebook
LOCAL_WHOLE_STATIC_LIBRARIES += sdkbox

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/Scenes/SplashScene.cpp \
                   ../../../Classes/Scenes/StartingScene.cpp \
                   ../../../Classes/Scenes/LevelSelectScene.cpp \
                   ../../../Classes/Scenes/ReplayLastScene.cpp \
                   ../../../Classes/Scenes/GameScene.cpp \
                   ../../../Classes/RandomMap/RandomMap.cpp \
                   ../../../Classes/RandomMap/ScreenCoordinate.cpp \
                   ../../../Classes/RandomMap/MapCreator.cpp \
                   ../../../Classes/RandomMap/MapBasicInfo.cpp \
                   ../../../Classes/GameLogic/AreaData.cpp \
                   ../../../Classes/GameLogic/DiceGame.cpp \
                   ../../../Classes/GameLogic/GameAI.cpp \
                   ../../../Classes/GameLogic/GameData.cpp \
                   ../../../Classes/GameLogic/GamePlayer.cpp \
                   ../../../Classes/GameLogic/JoinData.cpp \
                   ../../../Classes/Common/GameHelpScene.cpp \
                   ../../../Classes/Common/GamePayUtil.cpp \
                   ../../../Classes/Common/GameSettingsScene.cpp \
                   ../../../Classes/Common/PopUpBaseDialog.cpp \
                   ../../../Classes/Common/PopUpOkCancelDialog.cpp \
                   ../../../Classes/Common/PopUpOkDialog.cpp \
                   ../../../Classes/Common/WebSocktUtil.cpp \
                   ../../../Classes/Common/ModalDialog.cpp \
                   ../../../Classes/Network/FindPlayerScene.cpp \
                   ../../../Classes/Network/BattleFieldScene.cpp \
                   ../../../Classes/Network/UserSessionBean.cpp \
                   ../../../Classes/Network/OnlineGameData.cpp \
                   ../../../Classes/Network/OnlineGameEngine.cpp \
                   ../../../Classes/Network/BattleFieldBean.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
					$(LOCAL_PATH)/../../../Classes/Common \
					$(LOCAL_PATH)/../../../Classes/GameLogic \
					$(LOCAL_PATH)/../../../Classes/RandomMap \
					$(LOCAL_PATH)/../../../Classes/Scenes \
					$(LOCAL_PATH)/../../../Classes/Network

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module, ./sdkbox)
$(call import-module, ./pluginiap)
$(call import-module, ./pluginfacebook)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
