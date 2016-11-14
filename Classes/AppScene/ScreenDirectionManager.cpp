#include "ScreenDirectionManager.h"
#include "ClientCtrl.h"

ScreenDirectionManager* ScreenDirectionManager::m_shared = NULL;


ScreenDirectionManager* ScreenDirectionManager::share()
{
	if (!m_shared)
	{
		m_shared = new ScreenDirectionManager();
		m_shared->init();
	}
	return m_shared;
}

void ScreenDirectionManager::init()
{
	m_screenType = kScreenPortrait;
	m_selectDic = UserDefault::getInstance()->getIntegerForKey(STRG_SCREEN_DIRECTION_SELECT, 0); //Ä¬ÈÏauto

	ClientCtrl::share()->setScreenDic(m_selectDic);
}

void ScreenDirectionManager::setSelectDic(int dic)
{
	ST_RETURN_IF(m_selectDic == dic);
	m_selectDic = dic;
	UserDefault::getInstance()->setIntegerForKey(STRG_SCREEN_DIRECTION_SELECT, m_selectDic);
	ClientCtrl::share()->setScreenDic(m_selectDic);
}

