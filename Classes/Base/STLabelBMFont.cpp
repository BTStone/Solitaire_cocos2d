#include "STLabelBMFont.h"



STLabelBMFont::STLabelBMFont(FontAtlas *atlas, TextHAlignment hAlignment):
	Label(atlas, hAlignment)
{
	
}

STLabelBMFont* STLabelBMFont::createWithBMFont(const std::string& bmfontFilePath, const std::string& text, bool isCapital, bool allIsCapital,
	const TextHAlignment& alignment /* = TextHAlignment::LEFT */, int maxLineWidth /* = 0 */, const Vec2& imageOffset /* = Vec2::ZERO */)
{
	auto ret = new (std::nothrow) STLabelBMFont(nullptr, alignment);
	string txt = text;
	if (isCapital)
	{
		txt = convertFirstLetterToCapital(text);
	}
	if (allIsCapital)
	{
		txt = convertAllLetterToCapital(text);
	}
	if (ret && ret->setBMFontFilePath(bmfontFilePath, imageOffset))
	{
		ret->setMaxLineWidth(maxLineWidth);
		ret->setString(txt);
		
		ret->autorelease();

		return ret;
	}

	delete ret;
	return nullptr;
}

void STLabelBMFont::setTextSize(Size textSize)
{
	Size contSize = getContentSize();
	float scaleX = textSize.width / contSize.width;
	float scaleY = textSize.height / contSize.height;
	float minScale = min(scaleX, scaleY);
	this->setScale(minScale);
	m_textSize = contSize * minScale;
}

string STLabelBMFont::convertFirstLetterToCapital(string str)
{
	int len = str.size();
	string re = "";
	for (int i = 0; i < len; i++)
	{
		if (i == 0 && (str[i] >= 'a'&& str[i] <= 'z'))
		{
			re += str[i] - ('a' - 'A');
		}
		else
		{
			re += str[i];
		}
	}
	return re;
}

string STLabelBMFont::convertAllLetterToCapital(string str)
{
	bool allAlp = allIsAlpha(str);
	if (!allAlp){ return str; }
	int len = str.size();
	string re = "";
	for (int i = 0; i < len; i++)
	{
		if ((str[i] >= 'a'&& str[i] <= 'z'))
		{
			re += str[i] - ('a' - 'A');
		}
		else
		{
			re += str[i];
		}
	}
	return re;
}


string STLabelBMFont::fixNumPerLine(string str,int maxNumPerLine)
{
	int len = str.size();
	if (len <= maxNumPerLine)
	{
		return str;
	}
	string reStr = "";
	string wordStr = "";
	int numPerLine = 0;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == ',')
		{
			if (str[i] == ',')
			{
				wordStr += str[i];
			}

			int wordLen = wordStr.size();

			if ((numPerLine + wordLen) >= maxNumPerLine)
			{
				reStr += '\n';
				numPerLine = 0;
			}
			reStr += wordStr;
			reStr += ' ';
			numPerLine += (wordLen+1);
			wordStr = "";
		}
		else
		{
			wordStr += str[i];
		}
	}
	if (wordStr.size() > 0)
	{
		int wordLen = wordStr.size();
		if ((numPerLine + wordLen) > maxNumPerLine)
		{
			reStr += '\n';
			numPerLine = 0;
		}
		numPerLine += wordLen;
		reStr += wordStr;
	}
	return reStr;
}

string STLabelBMFont::fixNumPerLineWithOutAlpha(string str, int maxNumPerLine)
{
	int len = str.size();
	if (len <= maxNumPerLine)
	{
		return str;
	}
	string reStr = "";
	string wordStr = "";
	int numPerLine = 0;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == ',')
		{
			if (str[i] == ',')
			{
				wordStr += str[i];
			}

			int wordLen = wordStr.size();

			if ((numPerLine + wordLen) >= maxNumPerLine)
			{
				reStr += '\n';
				numPerLine = 0;
			}
			reStr += wordStr;
			reStr += ' ';
			numPerLine += (wordLen + 1);
			wordStr = "";
		}
		else
		{
			wordStr += str[i];
		}
	}
	if (wordStr.size() > 0)
	{
		int wordLen = wordStr.size();
		if ((numPerLine + wordLen) > maxNumPerLine)
		{
			reStr += '\n';
			numPerLine = 0;
		}
		numPerLine += wordLen;
		reStr += wordStr;
	}
	return reStr;
}


bool STLabelBMFont::hasAlpha(string str)
{
	int len = str.size();
	for (int i = 0; i < len; i++)
	{
		if ((str[i] >= 'A' && str[i] <= 'Z') ||
			(str[i] >= 'a' && str[i] <= 'z'))
		{
			return true;
		}
	}
	return false;
}

bool STLabelBMFont::allIsAlpha(string str)
{
	int len = str.size();
	for (int i = 0; i < len; i++)
	{
		if (!((str[i] >= 'A' && str[i] <= 'Z') ||
			(str[i] >= 'a' && str[i] <= 'z') ||
			(str[i] == ' ')))
		{
			return false;
		}
	}
	return true;
}

string STLabelBMFont::filterColorSign(string str)
{
	int len = str.size();
	string reStr = "";
	bool canAdd = true;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '[')
		{
			canAdd = false;
		}
		else if (str[i] == ']')
		{
			canAdd = true;
		}
		else if (canAdd)
		{
			reStr += str[i];
		}
	}
	return reStr;
}

