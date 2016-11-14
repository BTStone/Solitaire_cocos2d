#include "STLang.h"
#include "tinyxml.h"
#include "IOManager.h"

///==============================================STLang begin=====================================

STLang* STLang::m_shared = NULL;

STLang* STLang::shared()
{
	if (!m_shared)
	{
		m_shared = new STLang();
		m_shared->init();
	}
	return m_shared;
}

void STLang::init()
{
	initLangList();
	int defaultIdx = getDefaultLangIdx();
	m_curLang = UserDefault::getInstance()->getIntegerForKey(STRG_SELECT_LANGUAGE, defaultIdx);
}

int STLang::getDefaultLangIdx()
{
	int re = 1;
	ccLanguageType la = CCApplication::sharedApplication()->getCurrentLanguage();
	switch (la)
	{
	case kLanguageChinese:
		re = 2;
		break;
	case kLanguageEnglish:
		re = 1;
		break;
	case kLanguageFrench:
		re = 3;
		break;
	case kLanguageGerman:
		re = 4;
		break;
	case kLanguageKorean:
		re = 5;
		break;
	case kLanguagePortuguese:
		re = 6;
		break;
	case kLanguageRussian:
		re = 7;
		break;
	case kLanguageSpanish:
		re = 8;
		break;
	case kLanguageJapanese:
		re = 9;
		break;
	case kLanguageItalian:
		re = 10;
		break;
	default:
		break;
	}
	return re;   //English
}

string STLang::getLanguageName(const char * keyStr)
{
	string fileName = "data/lang/LanguageList.xml";
	return STParserXML::getStrValueByKey(fileName.c_str(), keyStr);
}


string STLang::getValue(const char * keyStr)
{
	string beginStr = "data/lang/";
	//string curLang = m_langList[m_curLang];
	string curLang = m_langList[m_curLang];
	string fileName = beginStr + curLang + ".xml";
	return STParserXML::getStrValueByKey(fileName.c_str(), keyStr);
}

string STLang::getReportParamByEventId(const char *enventId)
{
	string fileName = "data/umeng/Parameter.xml";
	return STParserXML::getStrValueByKey(fileName.c_str(), enventId);
}

void STLang::initLangList()
{
	const char *langList[] = { "ChineseSimplified", "English", "ChineseTraditional", "French",
		"German", "Korean", "Portuguese", "Russian", "Spanish", "Japanese", "Indonesian" };
	int len = 11;            //语言个数
	for (int i = 0; i < len; i++)
	{
		m_langList.push_back(langList[i]);
	}
}

void STLang::selectLanguage(int langIndex)
{
	int defaultIdx = getDefaultLangIdx();
	bool hasSelect = UserDefault::getInstance()->getBoolForKey(ST_HAS_ALREADY_SELECT_LANGUAGE, false);
	if (!hasSelect)
	{
		langIndex = defaultIdx;
	}
	ST_RETURN_IF(langIndex == m_curLang);
	m_curLang = langIndex;
	UserDefault::getInstance()->setIntegerForKey(STRG_SELECT_LANGUAGE, m_curLang);

	//更新显示信息
	NotificationCenter::getInstance()->postNotification(ST_UPDATE_LANGUAGE);
}


///==============================================STLang end=====================================

///==============================================STParserXML begin=====================================

string STParserXML::getStrValueByKey(const char *fileName, const char *keyStr)
{
	/*=======================
	CLDDesXml * myDocument = new CLDDesXml();
	string filePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
	bool isSuc = myDocument->LoadDesFile(filePath.c_str());
	if (!isSuc){
		log("isSuc no...");
		return "";
	}*///========================================
	/*
	string fullFileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
	log("fullFileName::%s", fullFileName.c_str());
	TiXmlDocument* myDocument = new TiXmlDocument(fileName);
	bool isSuc = myDocument->LoadFile();
	FILE * f = fopen(fullFileName.c_str(), "r");
	if (!f){
		log("fopen fail...");
	}
	*/
	TiXmlDocument myDocument;
	unsigned char* pBuffer = NULL;
	ssize_t bufferSize = 0;

	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fileName, "r", &bufferSize);

	if (pBuffer)
	{
		myDocument.Parse((const char*)pBuffer);

	}


	TiXmlElement* rootElement = myDocument.RootElement();  // 根元素  
	TiXmlElement* firstRootChild = rootElement->FirstChildElement();  // 根元素下的第一个元素 
	while (firstRootChild) {
		TiXmlAttribute* firstAt = firstRootChild->FirstAttribute();  //获得firstRootChild的第一个属性 
		const char *firstVal = firstAt->Value();   // 例如<string name="key" value="xxx"\>中的key
		const char * name = firstRootChild->Value();   // 例如<string name="key" value="value"\>中的string
		if (strcmp(keyStr, firstVal) == 0)
		{
			TiXmlAttribute* secondAt = firstAt->Next();
			const char *seVal = secondAt->Value();   // 例如<string name="key" value="xxx"\>中的xxx
			return seVal;
		}
		else
		{
			firstRootChild = firstRootChild->NextSiblingElement();
		}
	}
	return "";
}



///==============================================STParserXML end=====================================
