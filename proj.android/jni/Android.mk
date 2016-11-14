LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/AppScene/BottomMenuNode.cpp \
                   ../../Classes/AppScene/ClientAndroid.cpp \
                   ../../Classes/AppScene/ClientCtrl.cpp \
                   ../../Classes/AppScene/CSBHelper.cpp \
                   ../../Classes/AppScene/GameLayer.cpp \
                   ../../Classes/AppScene/IOManager.cpp \
                   ../../Classes/AppScene/LockManager.cpp \
                   ../../Classes/AppScene/MySwichControl.cpp \
                   ../../Classes/AppScene/NotificationManage.cpp \
                   ../../Classes/AppScene/PlayBoard.cpp \
                   ../../Classes/AppScene/ScreenDirectionManager.cpp \
                   ../../Classes/AppScene/ServerManager.cpp \
                   ../../Classes/AppScene/SkinManager.cpp \
                   ../../Classes/AppScene/StartLayer.cpp \
                   ../../Classes/AppScene/TipManager.cpp \
                   ../../Classes/AppScene/UndoManager.cpp \
                   ../../Classes/AppScene/CardData/Card.cpp \
                   ../../Classes/AppScene/CardData/CardDataManager.cpp \
                   ../../Classes/AppScene/CardData/CardQueue.cpp \
                   ../../Classes/AppScene/CardData/CollectCardArea.cpp \
                   ../../Classes/AppScene/CardData/HandCardArea.cpp \
                   ../../Classes/AppScene/CardData/NewGameManager.cpp \
                   ../../Classes/AppScene/CardData/PlaceCardArea.cpp \
                   ../../Classes/AppScene/CardData/PlaceCardQueue.cpp \
                   ../../Classes/AppScene/CardUI/CardSprite.cpp \
                   ../../Classes/AppScene/CardUI/TransportCardQueue.cpp \
                   ../../Classes/AppScene/CardUI/UICardQueue.cpp \
                   ../../Classes/AppScene/CardUI/UICollectCardArea.cpp \
                   ../../Classes/AppScene/CardUI/UICollectCardQueue.cpp \
                   ../../Classes/AppScene/CardUI/UIHandCardArea.cpp \
                   ../../Classes/AppScene/CardUI/UIHandClosedCardQueue.cpp \
                   ../../Classes/AppScene/CardUI/UIHandOpenCardQueue.cpp \
                   ../../Classes/AppScene/CardUI/UIPlaceCardArea.cpp \
                   ../../Classes/AppScene/CardUI/UIPlaceCardQueue.cpp \
                   ../../Classes/AppScene/DailyChallenge/Calendar.cpp \
                   ../../Classes/AppScene/DailyChallenge/CalendarItem.cpp \
                   ../../Classes/AppScene/DailyChallenge/DailyChallengeBoard.cpp \
                   ../../Classes/AppScene/DailyChallenge/DailyChallengeHelpBoard.cpp \
                   ../../Classes/AppScene/DailyChallenge/DailyChallengeManager.cpp \
                   ../../Classes/AppScene/DailyChallenge/TrophyInfoBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/BackDropBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/BackStyleOptionBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/FrontStyleOptionBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/InviteDailyChallengeBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/InviteFriendBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/InviteFriendSimpleBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/InviteSuccessBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/LanguageSelectBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/NetworkFailBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/NewThemeTipBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/OptionBaseBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/OptionBoardManager.cpp \
                   ../../Classes/AppScene/OptionBoard/RateBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/RateSuccessBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/ResetDataBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/RootOptionBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/StatisticsBoard.cpp \
                   ../../Classes/AppScene/OptionBoard/WinBoard.cpp \
                   ../../Classes/Base/CLDScreen.cpp \
                   ../../Classes/Base/STLang.cpp \
                   ../../Classes/Base/NetTime.cpp \
                   ../../Classes/Base/XMLParser.cpp \
                   ../../Classes/Base/STHelper.cpp \
                   ../../Classes/Base/STLabelBMFont.cpp \
                   ../../Classes/Base/STTimeManager.cpp \
                   ../../Classes/tinyxml/tinystr.cpp \
                   ../../Classes/tinyxml/tinyxml.cpp \
                   ../../Classes/tinyxml/tinyxmlerror.cpp \
                   ../../Classes/tinyxml/tinyxmlparser.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/AppScene
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/AppScene/CardData
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/AppScene/CardUI
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/AppScene/DailyChallenge
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/AppScene/OptionBoard
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Base
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Base/UMengSDK
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/tinyxml

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
#LOCAL_SHARED_LIBRARIES := mobclickcpp_shared


# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

#$(call import-module,libmobclickcpp)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
