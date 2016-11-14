#ifndef __ST_XML_PARSER_H__
#define __ST_XML_PARSER_H__

#include "STBase.h"


class XMLParser : public Ref, public SAXDelegator
{
public:
	static XMLParser* parseWithFile(const char *xmlFileName);
	static XMLParser* parseWithString(const char *content);
	XMLParser();
	virtual ~XMLParser();
	//从本地xml文件读取     
	bool initWithFile(const char *xmlFileName);
	//从字符中读取，可用于读取网络中的xml数据     
	bool initWithString(const char *content);
	//对应xml标签开始,如：<STRING name="app_name">     
	virtual void startElement(void *ctx, const char *name, const char **atts);
	//对应xml标签结束,如：</STRING>     
	virtual void endElement(void *ctx, const char *name);
	//对应xml标签文本     
	virtual void textHandler(void *ctx, const char *s, int len);
	String* getString(const char *key);
protected:
	Dictionary *m_pDictionary;
	string m_key;
	string startXMLElement;
	string endXMLElement;
};



#endif
