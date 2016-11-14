#include "SkinManager.h"
#include "ClientCtrl.h"

SkinManager* SkinManager::m_shared = NULL;
const char *strGameBgId = "game_bg_id";
const char *strCardBackId = "card_back_id";
const char *strCardFaceId = "card_face_id";

SkinManager* SkinManager::share()
{
	if (!m_shared)
	{
		m_shared = new SkinManager();
		m_shared->init();
	}
	return m_shared;
}

void SkinManager::init()
{
	m_findNewTheme = false;
	m_newGameBgName = "";
	m_newCardBackName = "";
	int gameBgId = UserDefault::getInstance()->getIntegerForKey(strGameBgId, 1);
	int cardBackId = UserDefault::getInstance()->getIntegerForKey(strCardBackId, 1);
	int cardFaceId = UserDefault::getInstance()->getIntegerForKey(strCardFaceId, 1);

	m_gameBgId = (gameBgId>0)?gameBgId:1;
	m_cardBackId = (cardBackId>0) ? cardBackId : 1;
	m_cardFaceId = (cardFaceId>0) ? cardFaceId : 1;

	m_insideGameBgNum = 9;
	m_insideCardBgNum = 10;

	initGameBgFileNames();
	initCardBgFileNames();
}

void SkinManager::refreshFileNames()
{
	initGameBgFileNames();
	initCardBgFileNames();
}

string SkinManager::getGameBgFileNameByGameBgId(int bgId) //bgId´Ó1¿ªÊ¼
{
	string docPath = STHelper::getDocPath();
	string toDir = "background/";
	docPath += toDir;
	char bgName[256];
	if (bgId <= m_insideGameBgNum)
	{
		sprintf(bgName, "cocostudio/ui/GameBg/%s", m_gameBgFileNames[bgId-1].c_str());
	}
	else
	{
		sprintf(bgName, "%s%s", docPath.c_str(), m_gameBgFileNames[bgId-1].c_str());
	}
	log("getGameBgFileNameByGameBgId, id:%d, bgName:%s", bgId, bgName);
	return bgName;
}
string SkinManager::getCardBgFileNameByCardBgId(int bgId)
{
	string docPath = STHelper::getDocPath();
	string toDir = "card/";
	docPath += toDir;
	char bgName[256];
	if (bgId <= m_insideCardBgNum)
	{
		sprintf(bgName, "cocostudio/ui/Card/Bg/%s", m_cardBgFileNames[bgId-1].c_str());
	}
	else
	{
		sprintf(bgName, "%s%s", docPath.c_str(), m_cardBgFileNames[bgId-1].c_str());
	}
	//log("getCardBgFileNameByCardBgId, id:%d, bgName:%s", bgId, bgName);
	return bgName;
}

void SkinManager::initGameBgFileNames()
{
	m_gameBgFileNames.clear();
	char bgName[64];
	for (int i = 1; i <= m_insideGameBgNum; i++)
	{
		sprintf(bgName, "BG%02d.jpg", i);
		m_gameBgFileNames.push_back(bgName);
	}
	string bgThemeStr = ClientCtrl::share()->getBackGroundThemeNames();
	log("initGameBgFileNames,bgStr:%s", bgThemeStr.c_str());
	vector<string> themeNames = parseThemeString(bgThemeStr);
	int ssz = themeNames.size();
	for (int i = 0; i < ssz; i++)
	{
		m_gameBgFileNames.push_back(themeNames[i]);
	}
}


void SkinManager::initCardBgFileNames()
{
	m_cardBgFileNames.clear();
	char bgName[64];
	for (int i = 1; i <= m_insideCardBgNum; i++)
	{
		sprintf(bgName, "Card-Back-BG-%02d.png", i);
		m_cardBgFileNames.push_back(bgName);
	}
	string bgThemeStr = ClientCtrl::share()->getCardBackThemeNames();
	log("initCardBgFileNames,bgStr:%s", bgThemeStr.c_str());
	vector<string> themeNames = parseThemeString(bgThemeStr);
	int ssz = themeNames.size();
	for (int i = 0; i < ssz; i++)
	{
		m_cardBgFileNames.push_back(themeNames[i]);
	}
}

vector<string> SkinManager::parseThemeString(string themeStr)
{
	vector<string> re;
	int len = themeStr.size();
	string singleStr = "";
	for (int i = 0; i < len; i++)
	{
		char ch = themeStr[i];
		if (ch == ',')
		{
			re.push_back(singleStr);
			singleStr = "";
		}
		else
		{
			singleStr += ch;
		}
	}
	return re;
}

vector<string> SkinManager::getGameBgFileNames()
{
	return m_gameBgFileNames;
}

vector<string> SkinManager::getCardBgFileNames()
{
	return m_cardBgFileNames;
}

int SkinManager::getGameBgIdByGameBgName(string bgName)
{
	int len = m_gameBgFileNames.size();
	for (int i = 0; i < len; i++)
	{
		string str = m_gameBgFileNames[i];
		if (str == bgName)
		{
			return (i + 1);
		}
	}

	return -1;
}
int SkinManager::getCardBgIdByGameBgName(string bgName)
{
	int len = m_cardBgFileNames.size();
	for (int i = 0; i < len; i++)
	{
		string str = m_cardBgFileNames[i];
		if (str == bgName)
		{
			return (i + 1);
		}
	}

	return -1;
}

void SkinManager::selectGameBg(int gameBgId)
{
	int len = m_gameBgFileNames.size();
	ST_RETURN_IF(gameBgId <= 0 || gameBgId > len);
	m_gameBgId = gameBgId;
	UserDefault::getInstance()->setIntegerForKey(strGameBgId, m_gameBgId);
}

void SkinManager::selectCardBack(int cardBackId)
{
	int len = m_cardBgFileNames.size();
	ST_RETURN_IF(cardBackId <= 0 || cardBackId > len);
	m_cardBackId = cardBackId;
	UserDefault::getInstance()->setIntegerForKey(strCardBackId, m_cardBackId);
}

void SkinManager::selectCardFace(int cardFaceId)
{
	m_cardFaceId = cardFaceId; 
	UserDefault::getInstance()->setIntegerForKey(strCardFaceId, m_cardFaceId);
}