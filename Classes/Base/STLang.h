#ifndef __ST_LANGUAGE_H__
#define __ST_LANGUAGE_H__

#include "STBase.h"

/// ���������
class STLang
{
public:
	/// �������
	static STLang * shared();
	/// �����ʼ��
	void init();
	void initLangList();
	int getDefaultLangIdx();
	/// ͨ��ָ����ʶ����ȡ��Ӧ�ı�����
	string getValue(const char * keyStr);
	string getLanguageName(const char *langKey);
	void selectLanguage(int langIndex);

	string getReportParamByEventId(const char *enventId);
protected:
	CC_SYNTHESIZE_READONLY(int, m_curLang, CurLangIdx);    ///< ��ǰ��������
protected:
	static STLang *m_shared;
	vector<string> m_langList;
};

class STParserXML
{
public:
	static string getStrValueByKey(const char *fileName, const char *keyStr);      //�������ڵ���
protected:
};



#endif
