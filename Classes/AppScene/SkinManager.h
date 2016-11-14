#ifndef __ST_SOLITAIRE_SKIN_MANAGER_H__
#define __ST_SOLITAIRE_SKIN_MANAGER_H__

#include "STBase.h"

class SkinManager
{
public:
	static SkinManager* share();
	void init();
	void selectGameBg(int bgId);                                       //选择游戏背景
	void selectCardBack(int cardBackId);                               //选择纸牌背面
	void selectCardFace(int cardFaceId);                               //选择纸牌正面

	string getGameBgFileNameByGameBgId(int bgId);
	string getCardBgFileNameByCardBgId(int bgId);

	void initGameBgFileNames();
	void initCardBgFileNames();
	vector<string>  parseThemeString(string themeStr);
	vector<string> getGameBgFileNames();
	vector<string> getCardBgFileNames();

	int getGameBgIdByGameBgName(string bgName);
	int getCardBgIdByGameBgName(string bgName);

	void refreshFileNames();


	CC_SYNTHESIZE_READONLY(int, m_gameBgId, GameBgID);                 //游戏背景
	CC_SYNTHESIZE_READONLY(int, m_cardBackId, CardBackID);             //纸牌背面
	CC_SYNTHESIZE_READONLY(int, m_cardFaceId, CardFaceID);             // 纸牌正面
	CC_SYNTHESIZE_READONLY(int, m_insideGameBgNum, InsideGameBgNum);   //内置游戏背景数量
	CC_SYNTHESIZE_READONLY(int, m_insideCardBgNum, InsideCardBgNum);   //内置游戏背景数量

	CC_SYNTHESIZE(bool, m_findNewTheme, FindNewTheme);
	CC_SYNTHESIZE(string, m_newGameBgName, NewGameBgName);
	CC_SYNTHESIZE(string, m_newCardBackName, NewCardBackName);
protected:
	static SkinManager *m_shared;
	vector<string> m_gameBgFileNames;
	vector<string> m_cardBgFileNames;
};


#endif
