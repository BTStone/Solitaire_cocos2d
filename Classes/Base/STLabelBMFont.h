#ifndef __ST_LABEL_BMFONT_H__
#define __ST_LABEL_BMFONT_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class STLabelBMFont : public Label
{
	
public:
	static string convertAllLetterToCapital(string str);            //将字符串q全部转换成大写
	static string convertFirstLetterToCapital(string str);           //将字符串的首字母转换成大写
	static bool hasAlpha(string str);                                //判断是否有字母
	static bool allIsAlpha(string str);                              //判断所有的都是字母
	static string fixNumPerLine(string str, int maxNumPerLine);      //根据每行最大数进行调整
	static string fixNumPerLineWithOutAlpha(string str, int maxNumPerLine);      //根据每行最大数进行调整
	static string filterColorSign(string str);                       //过滤掉颜色显示
	


	STLabelBMFont(FontAtlas *atlas = nullptr, TextHAlignment hAlignment = TextHAlignment::LEFT);
	static STLabelBMFont* createWithBMFont(const std::string& bmfontPath, const std::string& text,
											bool isCapital = true, ///首字母是否大写
											bool allIsCapital = false,
										   const TextHAlignment& hAlignment = TextHAlignment::LEFT, int maxLineWidth = 0,
										   const Vec2& imageOffset = Vec2::ZERO);
	void setTextSize(Size textSize);

protected:
	CC_SYNTHESIZE_READONLY(Size, m_textSize, TextSize);
};

#endif
