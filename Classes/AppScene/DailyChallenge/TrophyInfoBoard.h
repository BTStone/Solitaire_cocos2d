#ifndef __ST_TROPHY_INFO_BOARD_H__
#define __ST_TROPHY_INFO_BOARD_H__

#include "STBase.h"

enum TrophyType
{
	kCopperTrophy,
	kSilverTrophy,
	kGoldenTrophy
};

class TrophyInfoBoard : public Node
{
public:
	static TrophyInfoBoard* create(TrophyType tp, int year, int mon);
	virtual bool init(TrophyType tp, int year, int mon);
	void initBg();
	void initTrophy();
	void initSolvedState();
	void initSuccessInfo();
	void initSeperateMenu();

protected:
	void menuRemoveSelfCallBack(Ref *ref);
protected:
	TrophyType m_type;
	int m_year;
	int m_month;

};


#endif
