/*��������*/
#ifndef __ST_LOCK_MANAGER_H__
#define __ST_LOCK_MANAGER_H__

#include "STBase.h"
/*idx ���Ǵ�1��ʼ��*/
class LockManager
{
public:
	static LockManager * share();
	void init();

	void lockOriginBackStyle();
	void lockOriginBg();

	void lockBackStyle(int idx);
	void unLockBackStyle(int idx);
	void lockBg(int idx);
	void unLockBg(int idx);

	bool isBackStyleLock(int idx);
	bool isBgLock(int idx);
	bool isBackStyleIsNew(int idx);
	bool isBgIsNew(int idx);

	void setBgIsNew(int idx, bool isNew);
	void setBackStyleIsNew(int idx, bool isNew);


	void unLockAllByRate();                 //�������п���ͨ��rate�������Ʊ���������
	void unLockAllByInvite();               //�������п���ͨ��invite�������Ʊ���������

protected:
	static LockManager *m_shared;
};

#endif