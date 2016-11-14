#ifndef __ST_LABEL_BMFONT_H__
#define __ST_LABEL_BMFONT_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class STLabelBMFont : public Label
{
	
public:
	static string convertAllLetterToCapital(string str);            //���ַ���qȫ��ת���ɴ�д
	static string convertFirstLetterToCapital(string str);           //���ַ���������ĸת���ɴ�д
	static bool hasAlpha(string str);                                //�ж��Ƿ�����ĸ
	static bool allIsAlpha(string str);                              //�ж����еĶ�����ĸ
	static string fixNumPerLine(string str, int maxNumPerLine);      //����ÿ����������е���
	static string fixNumPerLineWithOutAlpha(string str, int maxNumPerLine);      //����ÿ����������е���
	static string filterColorSign(string str);                       //���˵���ɫ��ʾ
	


	STLabelBMFont(FontAtlas *atlas = nullptr, TextHAlignment hAlignment = TextHAlignment::LEFT);
	static STLabelBMFont* createWithBMFont(const std::string& bmfontPath, const std::string& text,
											bool isCapital = true, ///����ĸ�Ƿ��д
											bool allIsCapital = false,
										   const TextHAlignment& hAlignment = TextHAlignment::LEFT, int maxLineWidth = 0,
										   const Vec2& imageOffset = Vec2::ZERO);
	void setTextSize(Size textSize);

protected:
	CC_SYNTHESIZE_READONLY(Size, m_textSize, TextSize);
};

#endif
