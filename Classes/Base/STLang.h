#ifndef __ST_LANGUAGE_H__
#define __ST_LANGUAGE_H__

#include "STBase.h"

/// 多语言相关
class STLang
{
public:
	/// 共享对象
	static STLang * shared();
	/// 对象初始化
	void init();
	void initLangList();
	int getDefaultLangIdx();
	/// 通过指定标识名获取对应文本描述
	string getValue(const char * keyStr);
	string getLanguageName(const char *langKey);
	void selectLanguage(int langIndex);

	string getReportParamByEventId(const char *enventId);
protected:
	CC_SYNTHESIZE_READONLY(int, m_curLang, CurLangIdx);    ///< 当前语言类型
protected:
	static STLang *m_shared;
	vector<string> m_langList;
};

class STParserXML
{
public:
	static string getStrValueByKey(const char *fileName, const char *keyStr);      //仅适用于单层
protected:
};



#endif
