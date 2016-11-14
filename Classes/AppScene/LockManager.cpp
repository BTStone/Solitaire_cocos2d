#include "LockManager.h"
#include "OptionBaseBoard.h"

LockManager * LockManager::m_shared = NULL;

const char *strCardBackStyleLockKey = "st_card_back_style_isLock_";
const char *strGameBgLockKey = "st_game_bg_isLock_";
const char *strCardBackIsNew = "st_card_bark_is_new_";
const char *strGameBgIsNew = "st_game_bg_is_new_";

LockManager * LockManager::share()
{
	if (!m_shared)
	{
		m_shared = new LockManager();
		m_shared->init();
	}
	return m_shared;
}

void LockManager::init()
{
	//仅仅初始化一次
	int firstOnce = UserDefault::getInstance()->getIntegerForKey("stShowBackStyleBoardNum", 0);
	ST_RETURN_IF(firstOnce != 0);
	UserDefault::getInstance()->setIntegerForKey("stShowBackStyleBoardNum", 1);
	lockOriginBackStyle();
	lockOriginBg();
}

void LockManager::lockOriginBackStyle()
{
	int lockNum = 5;
	int lockId[] = { 6, 7, 8, 9, 10 };     //不是下标，下标+1
	char buf[64];
	for (int i = 0; i < lockNum; i++)
	{
		sprintf(buf, "%s%d", strCardBackStyleLockKey,lockId[i]);
		UserDefault::getInstance()->setBoolForKey(buf, true);
	}
}

void LockManager::lockOriginBg()
{
	int lockNum = 4;
	int lockId[] = {7, 8, 9};      //不是下标，下标+1
	char buf[64];
	for (int i = 0; i < lockNum; i++)
	{
		sprintf(buf, "%s%d", strGameBgLockKey, lockId[i]);
		UserDefault::getInstance()->setBoolForKey(buf, true);
	}
}

void LockManager::lockBackStyle(int idx)
{
	char buf[64];
	sprintf(buf, "%s%d",strCardBackStyleLockKey, idx + 1);
	UserDefault::getInstance()->setBoolForKey(buf, true);
}

void LockManager::unLockBackStyle(int idx)
{
	char buf[64];
	sprintf(buf, "%s%d", strCardBackStyleLockKey, idx + 1);
	UserDefault::getInstance()->setBoolForKey(buf, false);
}

void LockManager::lockBg(int idx)
{
	char buf[64];
	sprintf(buf, "%s%d", strGameBgLockKey, idx + 1);
	UserDefault::getInstance()->setBoolForKey(buf, true);
}

void LockManager::unLockBg(int idx)
{
	char buf[64];
	sprintf(buf, "%s%d", strGameBgLockKey, idx + 1);
	UserDefault::getInstance()->setBoolForKey(buf, false);
}

bool LockManager::isBackStyleLock(int idx)
{
	char buf[64];
	sprintf(buf, "%s%d", strCardBackStyleLockKey, idx);
	return UserDefault::getInstance()->getBoolForKey(buf, false);
}

bool LockManager::isBgLock(int idx)
{
	char buf[64];
	sprintf(buf, "%s%d", strGameBgLockKey, idx);
	return UserDefault::getInstance()->getBoolForKey(buf, false);
}

bool LockManager::isBackStyleIsNew(int idx)
{
	char buf[64];
	sprintf(buf, "%s%d", strCardBackIsNew, idx);
	return UserDefault::getInstance()->getBoolForKey(buf, false);
}

bool LockManager::isBgIsNew(int idx)
{
	char buf[64];
	sprintf(buf, "%s%d", strGameBgIsNew, idx);
	return UserDefault::getInstance()->getBoolForKey(buf, false);
}

void LockManager::setBackStyleIsNew(int idx, bool isNew)
{
	char buf[64];
	sprintf(buf, "%s%d", strCardBackIsNew, idx);
	UserDefault::getInstance()->setBoolForKey(buf, isNew);
}

void LockManager::setBgIsNew(int idx, bool isNew)
{
	char buf[64];
	sprintf(buf, "%s%d", strGameBgIsNew, idx);
	UserDefault::getInstance()->setBoolForKey(buf, isNew);
}

void LockManager::unLockAllByRate()
{
	int totalRateNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_RATE_NUM, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_TOTAL_RATE_NUM, totalRateNum + 1);
	ST_RETURN_IF(totalRateNum > 0);

	char buf[64];

	//解锁牌背
	int backStyleLockNum = 5;
	int backStyleLockId[] = { 6, 7, 8, 9, 10 };     //不是下标，下标+1
	for (int i = 0; i < backStyleLockNum; i++)
	{
		sprintf(buf, "%s%d", strCardBackStyleLockKey, backStyleLockId[i]);
		UserDefault::getInstance()->setBoolForKey(buf, false);

		//设置成最新牌背
		sprintf(buf, "%s%d", strCardBackIsNew, backStyleLockId[i]);
		UserDefault::getInstance()->setBoolForKey(buf, true);
	}



	//弹出rate成功面板
	NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kRateSuccessBoard);
	NotificationCenter::getInstance()->postNotification(ST_RATE_SUCCESS);

}

void LockManager::unLockAllByInvite()
{
	int totalInviteNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_INVITE_NUM, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_TOTAL_INVITE_NUM, totalInviteNum + 1);
	ST_RETURN_IF(totalInviteNum > 0);

	char buf[64];

	//解锁背景
	int bgLockNum = 4;
	int bgLockId[] = { 7, 8, 9 };     //不是下标，下标+1
	for (int i = 0; i < bgLockNum; i++)
	{
		sprintf(buf, "%s%d", strGameBgLockKey, bgLockId[i]);
		UserDefault::getInstance()->setBoolForKey(buf, false);

		//设置成最新背景
		sprintf(buf, "%s%d", strGameBgIsNew, bgLockId[i]);
		UserDefault::getInstance()->setBoolForKey(buf, true);
	}


	//弹出rate成功面板
	NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kInviteSuccessBoard);
	NotificationCenter::getInstance()->postNotification(ST_INVITE_SUCCESS);
}