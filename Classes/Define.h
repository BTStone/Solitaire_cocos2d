#ifndef __ST_SOLITAIRE_DEFINE_H__
#define __ST_SOLITAIRE_DEFINE_H__

#define Collect_Area_Num                    4                   //�������ܹ����� 4
#define Place_Area_Num                      7                   //�������ܹ����� 7

#define Place_Area_Gap_X                    3                   //������ˮƽ����ֽ��֮��ļ��
#define Place_Area_Gap_Y                    0.35                //��������ֱ����ֽ��֮��ļ��ռһ���Ƹ߶ȵı���
#define Hand_Open_Area_Gap_X                0.3                 //���ƴ���ˮƽ����ļ������

#define Transport_Move_Speed      1000                //�����ƶ����ٶ�

#define Move_And_Untouchable_Time 0.15                //�����ƶ���ʱ�䣨�����ٶȣ����Լ��ƶ������в��ɵ����ʱ����

#define Game_Layer_Head_Label_Height        75        //ͷ������������ռ�ĸ߶�  

#define St_is_portrait  0                             //��win32�¿��ƺ�����


//==========================msg str  begin=================================

#define ST_SET_GAME_LAYER_TOUCH_ENABLE             "msgSetGameLayerTouchEnable"               //������Ϸ���Ƿ���Ե������Ϣ
#define ST_SHOW_PLAY_BOARD                         "msgShowPlayBoard"                         //��ʾplayboard
#define ST_NEW_GAME                                "msgNewGame"                               //����Ϸ
#define ST_REPLAY_GAME							   "msgReplayGame"                            //���������Ϸ
#define ST_SHOW_OPTION_BOARD					   "msgShowOptionBoard"                       //��ʾѡ����壨������壩
#define ST_SHOW_OPTION_BOARD_WITHOUT_EFFECT        "msgShowOptionBoardWithOutEffect"          //��ʾѡ����壨������壩,û���ƶ�Ч��
#define ST_UPDATE_AFTER_SELECT					   "msgUpdateGameLayer"                       //����GameLayer�㡢����������ʾ��ֽ�ƣ���Ϸ������
#define ST_TIP_FOR_GAME							   "msgTipForGame"                            //��ʾ
#define ST_UNDO_FOR_GAME						   "msgUndoForGame"                           //����
#define ST_SWITCH_SCREEN					       "msgSwitchScreen"                          //�л�������
#define ST_SELECT_CALENDAR_DAY                     "msgSelectCalendarDay"                     //ѡ�������ϵ�ĳһ������
#define ST_DAILY_CHALLENGE_GAME_START              "msgDailyChallengeGameStart"               //ÿ����ս��Ϸ��ʼ
#define ST_SWITCH_HAND_TYPE                        "msgSwitchHandTpye"                        //�л�������
#define ST_CONNECT_NETWORK                         "msgConnectNetwork"                        //����
#define ST_UPDATE_LANGUAGE                         "msgUpdateLanguage"                        //��������
#define ST_RATE_SUCCESS                            "msgRateSuccess"                           //���۳ɹ�
#define ST_INVITE_SUCCESS                          "msgInviteSuccess"                         //����ɹ�
#define ST_GAME_ENTER_BACKGROUND                   "msgGameEnterBackGround"                   //��Ϸ�����̨
#define ST_EXIT_GAME                               "msgExitGame"                              //�˳���Ϸ
#define ST_CLICK_ANDROID_RETURN_BUTTON             "msgClickAndroidReturnButton"              //������ؼ�
#define ST_RESET_STA_DATA                          "msgResetStaData"                          //������Ϣ
#define ST_CHANGE_TO_NEW_THEME                     "msgChangeToNewTheme"                      //�����µ�����
#define ST_SHOW_NEW_THEME_BOARD_DAILY              "msgShowNewThemeBoardDaily"                //
//==========================msg str  end=================================

//==========================storage str  begin=================================
#define STRG_ENTER_GAME_NUM             "strgEnterGameNum"                        //������Ϸ�Ĵ���
#define STRG_NEW_GAME_NUM               "strgNewGameNum"                          //����Ϸ�Ĵ���
#define STRG_CHALLENGE_NUM              "strgDailyChallengeNum"                   //ÿ����ս�Ĵ���
#define STRG_REPLAY_NUM                 "strgReplayNum"                           //replay�Ĵ���
#define STRG_TOTAL_PLAY_NUM             "strgTotalPlayNum"                        //�ܹ���ľ�����newGame��
#define STRG_TOTAL_WIN_NUM              "strgTotalWinNum"                         //�ܹ�ȫ�յľ���
#define STRG_WIN_STREAK_NUM             "strgWinSreakNum"                         //��ʤ�ľ���
#define STRG_BEST_SCORE_OF_WIN          "strgBestScoreOfWin"                      //ȫ����ʱ��߷�
#define STRG_BEST_TIME_OF_WIN           "strgBestTimeOfWin"                       //ȫ����ʱ���ʱ��
#define STRG_BEST_MOVES_OF_WIN          "strgBestMovesOfWin"                      //ȫ����ʱ���ٵĲ���
#define STRG_CUMULATIVE_SCORE_OF_WIN    "strgCumulativeScoreOfWin"                //ȫ����ʱ���ܻ���
#define STRG_SELECT_LANGUAGE            "strgSelectLanguage"                      //�洢�ϴ�ѡ�������
#define STRG_INIT_GAME_TYPE             "strgInitGameType"                        //�洢����һ����Ϸ�����ͣ�����Ϸ�� ����ȣ�
#define STRG_IS_NOTIFY_CHALLENGE        "strgIsNotifyChallenge"                   //������Ϸ�Ƿ���ʾÿ����ս
#define STRG_LAST_CHALLENGE_HELP_IDX    "strgLastChallengeHelpIdx"                //���ѡ���ÿ����ս�����±�
#define STRG_CURRENT_GAME_IS_CHALLENGE  "strgCurrentGameIsChallenge"              //��ǰ���Ƿ�����ս��
#define STRG_SCREEN_DIRECTION_SELECT    "strgScreenDirectionSelect"               //ѡ����Ļ����
#define STRG_TOTAL_CHALLENGE_SUCCESS_NUM  "strgTotalChallengeSuccessNum"          //����ÿ����սͨ���Ĵ���(���ظ�)
#define STRG_TOTAL_INVITE_NUM             "strgTotalInviteNum"                    //�ܹ�invite�Ĵ���
#define STRG_TOTAL_RATE_NUM               "strgTotalRateNum"                      //�ܹ�rate�Ĵ���
#define STRG_HAS_ALREADY_RATE_US          "strgHasAlreadyRateUs"                  //�Ƿ��Ѿ�rate
#define ST_HAS_ALREADY_SELECT_LANGUAGE    "strgHasAlreadySelectLanguage"          //�Ƿ� �ֶ� ѡ������
#define STRG_ADD_MORE_GAME_ICON_NUM       "strgAddMoreGameIconNum"                //���moreGameIcon�Ĵ���

//==========================storage str  end=================================


//==========================sound  begin=================================
#define SOUND_FLIP_CARD_1                "sound/draw_card1-wav.mp3"                          //��һ���Ƶ���Ч
#define SOUND_FLIP_CARD_3                "sound/draw_card3-wav.mp3"                          //�������Ƶ���Ч
#define SOUND_MOVE_NO_EFFECT             "sound/card_error-wav.mp3"                          //�ƶ���Ч��Ч
#define SOUND_NEW_GAME_DEAL              "sound/new_deal-wav.mp3"                            //����Ϸ������Ч
#define SOUND_MENU_IN                    "sound/main_menu_slide_in-wav.wav"                  //�����ֵ���Ч
#define SOUND_MENU_OUT                   "sound/main_menu_slide_out-wav.wav"                 //�����ʧ����Ч
#define SOUND_NEW_RECORD                 "sound/new_record.mp3"                              //�µļ�¼
#define SOUND_UNDO                       "sound/undo.mp3"                                    //����
#define SOUND_VICTORY                    "sound/victory-wav.mp3"                             //winȫ��
#define SOUND_PLACE_CARD                 "sound/place_card-wav.mp3"                          //ֽ���ƶ���Ч
#define SOUND_ACHIEVEMENT                "sound/Achievement.mp3"                             //
#define SOUND_COLLECT_CARD               "sound/cardbacksite-wav.mp3"                        //������Ч

//==========================sound  end=================================


//ϵͳ�����¼�id
#define Notification_Daily_Challenge_Id   10001         //ÿ����ս����
#define Notification_Three_Days_Leave     10002         //������������
#define Notification_No_Win               10003         //δ����ƾ�����

#define ST_UMENG_ADS_PARAM                "in_app_ad_opts"                        //���˿��ƹ������߲���
#define ST_UMENG_DAILY_CHALLENGE_PARAM    "in_app_opts"                           //���˿���ÿ����ս�Ƿ��������Լ���������ַ
#define ST_UMENG_DEV_AD_CTRL              "dev_ctrl"                              //���˿���dev���

#define ST_BANNER_HEIGHT                  120

#define STR_MORE_GAME_ICON                "buttonMoreGameIcon"

#endif
