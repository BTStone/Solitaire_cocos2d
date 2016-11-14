#ifndef __ST_SOLITAIRE_SKIN_MANAGER_H__
#define __ST_SOLITAIRE_SKIN_MANAGER_H__

#include "STBase.h"

class SkinManager
{
public:
	static SkinManager* share();
	void init();
	void selectGameBg(int bgId);                                       //ѡ����Ϸ����
	void selectCardBack(int cardBackId);                               //ѡ��ֽ�Ʊ���
	void selectCardFace(int cardFaceId);                               //ѡ��ֽ������

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


	CC_SYNTHESIZE_READONLY(int, m_gameBgId, GameBgID);                 //��Ϸ����
	CC_SYNTHESIZE_READONLY(int, m_cardBackId, CardBackID);             //ֽ�Ʊ���
	CC_SYNTHESIZE_READONLY(int, m_cardFaceId, CardFaceID);             // ֽ������
	CC_SYNTHESIZE_READONLY(int, m_insideGameBgNum, InsideGameBgNum);   //������Ϸ��������
	CC_SYNTHESIZE_READONLY(int, m_insideCardBgNum, InsideCardBgNum);   //������Ϸ��������

	CC_SYNTHESIZE(bool, m_findNewTheme, FindNewTheme);
	CC_SYNTHESIZE(string, m_newGameBgName, NewGameBgName);
	CC_SYNTHESIZE(string, m_newCardBackName, NewCardBackName);
protected:
	static SkinManager *m_shared;
	vector<string> m_gameBgFileNames;
	vector<string> m_cardBgFileNames;
};


#endif
