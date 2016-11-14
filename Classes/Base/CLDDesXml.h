#ifndef __CLD_DES_XML_H__
#define __CLD_DES_XML_H__

#include "tinyxml.h"
#include "cocos2d.h"
using namespace cocos2d;

class CLDDesXml : public TiXmlDocument
{
public:
    CLDDesXml(); 
    bool LoadDesFile(const char * fileName, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING ); 
    bool SaveDesFile(const char * fileName); 
protected:
    bool DesEncodeFile(FILE * file); 
    bool LoadDesData(char * buf, long bufLen, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING ); 
}; 

#endif // __CLD_DES_XML_H__

