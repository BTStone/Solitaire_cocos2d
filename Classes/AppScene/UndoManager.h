#ifndef __ST_SOLITAIRE_UNDO_MANAGER_H__
#define __ST_SOLITAIRE_UNDO_MANAGER_H__

#include "UICardQueue.h"

struct UndoInfo
{
	UndoInfo(){}
	UndoInfo(int num)
	{
		m_number = num;
	}
	~UndoInfo(){}

	CardArea m_resArea;       //源区域
	int m_resQueIdx;          //源队列
	CardArea m_desArea;       //目标区域 
	int m_desQueIdx;          //目标队列
	int m_number;             //纸牌的数量
	int m_scoreOff;           //分数的增减
	bool m_opened;            //操作的时候是否因为有效移动翻开最后一张牌
};

const UndoInfo Invalid_Undo_Info = UndoInfo(0);

class UndoManager
{
public:
	static UndoManager* share();
	void pushBack(UndoInfo info);
	UndoInfo pop();
	void clear();
	static bool isInvalidUndoInfo(UndoInfo info);
protected:
	static UndoManager *m_shared;
	list<UndoInfo> m_infos;
};


#endif
