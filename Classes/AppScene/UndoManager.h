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

	CardArea m_resArea;       //Դ����
	int m_resQueIdx;          //Դ����
	CardArea m_desArea;       //Ŀ������ 
	int m_desQueIdx;          //Ŀ�����
	int m_number;             //ֽ�Ƶ�����
	int m_scoreOff;           //����������
	bool m_opened;            //������ʱ���Ƿ���Ϊ��Ч�ƶ��������һ����
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
