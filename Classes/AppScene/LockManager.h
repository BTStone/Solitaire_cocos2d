/*解锁功能*/
#ifndef __ST_LOCK_MANAGER_H__
#define __ST_LOCK_MANAGER_H__

#include "STBase.h"
/*idx 均是从1开始的*/
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


	void unLockAllByRate();                 //解锁所有可以通过rate解锁的牌背、背景等
	void unLockAllByInvite();               //解锁所有可以通过invite解锁的牌背、背景等

protected:
	static LockManager *m_shared;
};

#endif