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
	//�ӱ���xml�ļ���ȡ     
	bool initWithFile(const char *xmlFileName);
	//���ַ��ж�ȡ�������ڶ�ȡ�����е�xml����     
	bool initWithString(const char *content);
	//��Ӧxml��ǩ��ʼ,�磺<STRING name="app_name">     
	virtual void startElement(void *ctx, const char *name, const char **atts);
	//��Ӧxml��ǩ����,�磺</STRING>     
	virtual void endElement(void *ctx, const char *name);
	//��Ӧxml��ǩ�ı�     
	virtual void textHandler(void *ctx, const char *s, int len);
	String* getString(const char *key);
protected:
	Dictionary *m_pDictionary;
	string m_key;
	string startXMLElement;
	string endXMLElement;
};



#endif
