#ifndef __ST_SOLITAIRE_OPTION_BASE_BOARD_H__
#define __ST_SOLITAIRE_OPTION_BASE_BOARD_H__

#include "STBase.h"

enum OptionBoardType
{
	kRootOptionBoard = 0,
	kBackDropBoard,
	kFrontStyleBoard,
	kBackStyleBoard,
	kStatisticsBoard,
	kWinBoard,
	kDailyChallengeBoard,
	kDailyChallengeHelpBoard,
	kNetworkFailBoard,
	kLanguageSelectBoard,
	kInviteDailyChallengeBoard,
	kRateBoard,
	kRateSuccessBoard,
	kResetDataBoard,
	kInviteFriendBoard,
	kInviteFriendSimpleBoard,
	kNewThemeTipBoard,
	kInviteSuccessBoard
};


class OptionBaseBoard : public Node
{
public:
	virtual bool init(OptionBoardType tp);
	virtual void initRootCSBNode();        //◊”¿‡÷ÿ–¥
	virtual void closeEffect();

	Size getBgSize();
protected:
	virtual void removeSelfCallFunc();

	CC_SYNTHESIZE(OptionBoardType, m_optionBoardType, OptionBoardType);
protected:
	Node *m_rootCSBNode;
};

#endif
