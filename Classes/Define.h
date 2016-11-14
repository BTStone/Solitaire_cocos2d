#ifndef __ST_SOLITAIRE_DEFINE_H__
#define __ST_SOLITAIRE_DEFINE_H__

#define Collect_Area_Num                    4                   //收牌区总共列数 4
#define Place_Area_Num                      7                   //置牌区总共列数 7

#define Place_Area_Gap_X                    3                   //置牌区水平方向纸牌之间的间距
#define Place_Area_Gap_Y                    0.35                //置牌区垂直方向纸牌之间的间距占一张牌高度的比例
#define Hand_Open_Area_Gap_X                0.3                 //手牌打开区水平方向的间隔比例

#define Transport_Move_Speed      1000                //卡牌移动的速度

#define Move_And_Untouchable_Time 0.15                //卡牌移动的时间（控制速度），以及移动过程中不可点击的时间间隔

#define Game_Layer_Head_Label_Height        75        //头部分数，步数占的高度  

#define St_is_portrait  0                             //在win32下控制横竖屏


//==========================msg str  begin=================================

#define ST_SET_GAME_LAYER_TOUCH_ENABLE             "msgSetGameLayerTouchEnable"               //设置游戏层是否可以点击的消息
#define ST_SHOW_PLAY_BOARD                         "msgShowPlayBoard"                         //显示playboard
#define ST_NEW_GAME                                "msgNewGame"                               //新游戏
#define ST_REPLAY_GAME							   "msgReplayGame"                            //重新这局游戏
#define ST_SHOW_OPTION_BOARD					   "msgShowOptionBoard"                       //显示选项面板（设置面板）
#define ST_SHOW_OPTION_BOARD_WITHOUT_EFFECT        "msgShowOptionBoardWithOutEffect"          //显示选项面板（设置面板）,没有移动效果
#define ST_UPDATE_AFTER_SELECT					   "msgUpdateGameLayer"                       //更新GameLayer层、设置面板的显示（纸牌，游戏背景）
#define ST_TIP_FOR_GAME							   "msgTipForGame"                            //提示
#define ST_UNDO_FOR_GAME						   "msgUndoForGame"                           //回退
#define ST_SWITCH_SCREEN					       "msgSwitchScreen"                          //切换横竖屏
#define ST_SELECT_CALENDAR_DAY                     "msgSelectCalendarDay"                     //选中日历上的某一个日期
#define ST_DAILY_CHALLENGE_GAME_START              "msgDailyChallengeGameStart"               //每日挑战游戏开始
#define ST_SWITCH_HAND_TYPE                        "msgSwitchHandTpye"                        //切换左右手
#define ST_CONNECT_NETWORK                         "msgConnectNetwork"                        //联网
#define ST_UPDATE_LANGUAGE                         "msgUpdateLanguage"                        //更新语言
#define ST_RATE_SUCCESS                            "msgRateSuccess"                           //评价成功
#define ST_INVITE_SUCCESS                          "msgInviteSuccess"                         //邀请成功
#define ST_GAME_ENTER_BACKGROUND                   "msgGameEnterBackGround"                   //游戏进入后台
#define ST_EXIT_GAME                               "msgExitGame"                              //退出游戏
#define ST_CLICK_ANDROID_RETURN_BUTTON             "msgClickAndroidReturnButton"              //点击返回键
#define ST_RESET_STA_DATA                          "msgResetStaData"                          //重置信息
#define ST_CHANGE_TO_NEW_THEME                     "msgChangeToNewTheme"                      //设置新的主题
#define ST_SHOW_NEW_THEME_BOARD_DAILY              "msgShowNewThemeBoardDaily"                //
//==========================msg str  end=================================

//==========================storage str  begin=================================
#define STRG_ENTER_GAME_NUM             "strgEnterGameNum"                        //进入游戏的次数
#define STRG_NEW_GAME_NUM               "strgNewGameNum"                          //新游戏的次数
#define STRG_CHALLENGE_NUM              "strgDailyChallengeNum"                   //每日挑战的次数
#define STRG_REPLAY_NUM                 "strgReplayNum"                           //replay的次数
#define STRG_TOTAL_PLAY_NUM             "strgTotalPlayNum"                        //总共玩的局数（newGame）
#define STRG_TOTAL_WIN_NUM              "strgTotalWinNum"                         //总共全收的局数
#define STRG_WIN_STREAK_NUM             "strgWinSreakNum"                         //连胜的局数
#define STRG_BEST_SCORE_OF_WIN          "strgBestScoreOfWin"                      //全收牌时最高分
#define STRG_BEST_TIME_OF_WIN           "strgBestTimeOfWin"                       //全收牌时最短时间
#define STRG_BEST_MOVES_OF_WIN          "strgBestMovesOfWin"                      //全收牌时最少的步骤
#define STRG_CUMULATIVE_SCORE_OF_WIN    "strgCumulativeScoreOfWin"                //全收牌时的总积分
#define STRG_SELECT_LANGUAGE            "strgSelectLanguage"                      //存储上次选择的语言
#define STRG_INIT_GAME_TYPE             "strgInitGameType"                        //存储重新一局游戏的类型（新游戏， 重玩等）
#define STRG_IS_NOTIFY_CHALLENGE        "strgIsNotifyChallenge"                   //进入游戏是否提示每日挑战
#define STRG_LAST_CHALLENGE_HELP_IDX    "strgLastChallengeHelpIdx"                //最后选择的每日挑战帮助下标
#define STRG_CURRENT_GAME_IS_CHALLENGE  "strgCurrentGameIsChallenge"              //当前局是否是挑战局
#define STRG_SCREEN_DIRECTION_SELECT    "strgScreenDirectionSelect"               //选择屏幕方向
#define STRG_TOTAL_CHALLENGE_SUCCESS_NUM  "strgTotalChallengeSuccessNum"          //所有每日挑战通过的次数(不重复)
#define STRG_TOTAL_INVITE_NUM             "strgTotalInviteNum"                    //总共invite的次数
#define STRG_TOTAL_RATE_NUM               "strgTotalRateNum"                      //总共rate的次数
#define STRG_HAS_ALREADY_RATE_US          "strgHasAlreadyRateUs"                  //是否已经rate
#define ST_HAS_ALREADY_SELECT_LANGUAGE    "strgHasAlreadySelectLanguage"          //是否 手动 选择语言
#define STRG_ADD_MORE_GAME_ICON_NUM       "strgAddMoreGameIconNum"                //添加moreGameIcon的次数

//==========================storage str  end=================================


//==========================sound  begin=================================
#define SOUND_FLIP_CARD_1                "sound/draw_card1-wav.mp3"                          //翻一张牌的音效
#define SOUND_FLIP_CARD_3                "sound/draw_card3-wav.mp3"                          //翻三张牌的音效
#define SOUND_MOVE_NO_EFFECT             "sound/card_error-wav.mp3"                          //移动无效音效
#define SOUND_NEW_GAME_DEAL              "sound/new_deal-wav.mp3"                            //新游戏发牌音效
#define SOUND_MENU_IN                    "sound/main_menu_slide_in-wav.wav"                  //面板出现的音效
#define SOUND_MENU_OUT                   "sound/main_menu_slide_out-wav.wav"                 //面板消失的音效
#define SOUND_NEW_RECORD                 "sound/new_record.mp3"                              //新的记录
#define SOUND_UNDO                       "sound/undo.mp3"                                    //返回
#define SOUND_VICTORY                    "sound/victory-wav.mp3"                             //win全收
#define SOUND_PLACE_CARD                 "sound/place_card-wav.mp3"                          //纸牌移动音效
#define SOUND_ACHIEVEMENT                "sound/Achievement.mp3"                             //
#define SOUND_COLLECT_CARD               "sound/cardbacksite-wav.mp3"                        //收牌音效

//==========================sound  end=================================


//系统推送事件id
#define Notification_Daily_Challenge_Id   10001         //每日挑战推送
#define Notification_Three_Days_Leave     10002         //三天离线推送
#define Notification_No_Win               10003         //未完成牌局推送

#define ST_UMENG_ADS_PARAM                "in_app_ad_opts"                        //友盟控制广告的在线参数
#define ST_UMENG_DAILY_CHALLENGE_PARAM    "in_app_opts"                           //友盟控制每日挑战是否联网，以及服务器地址
#define ST_UMENG_DEV_AD_CTRL              "dev_ctrl"                              //友盟控制dev广告

#define ST_BANNER_HEIGHT                  120

#define STR_MORE_GAME_ICON                "buttonMoreGameIcon"

#endif
