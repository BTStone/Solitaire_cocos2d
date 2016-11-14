#include "IOManager.h"
#include "json/document.h"

IOManager* IOManager::m_shared = NULL;

IOManager* IOManager::share()
{
	if (!m_shared)
	{
		m_shared = new IOManager();
	}
	return m_shared;
}

void IOManager::write(string contentStr, string fileName, string typeStr)
{
	string path = STHelper::getDocPath();
	char filePath[128];
	sprintf(filePath, "%s%s", path.c_str(), fileName.c_str());
	FILE *file = fopen(filePath, typeStr.c_str());
	if (file)
	{
		fwrite(contentStr.c_str(), 1, strlen(contentStr.c_str()), file);
		fclose(file);
	}
}

string IOManager::convertCardToString(Card pCard)
{
	char buf[64];
	int num = pCard.m_number;
	int color = pCard.m_color;
	bool isOpen = pCard.m_isOpen;
	string str;
	sprintf(buf, "        {\"number\":%d,", num);
	str += buf;
	sprintf(buf, "\"color\":%d,", color);
	str += buf;
	sprintf(buf, "\"isOpen\":%d}", isOpen);
	str += buf;

	return str;
}

vector<Card> IOManager::readCardByKey(const char *fileName, const char *keyStr)
{
	vector<Card> cards;
	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	string path = STHelper::getDocPath();
	char filePath[1024];
	sprintf(filePath, "%s%s", path.c_str(), fileName);
	pBytes = FileUtils::getInstance()->getFileData(filePath, "r", &size);
	if (pBytes == NULL || strcmp((char*)pBytes, "") == 0) return cards;
	std::string load_str((const char*)pBytes, size);
	CC_SAFE_DELETE_ARRAY(pBytes);
	_doc.Parse<0>(load_str.c_str());
	if (_doc.HasParseError()) return cards;
	if (!_doc.IsObject())
	{
		return cards;
	}

	//是否有此成员  
	//if(!_doc.HasMember("entities"))  
	//return;  
	// 通过[]取成员值,再根据需要转为array,int,double,string  
	const rapidjson::Value &pArray = _doc[keyStr];

	//是否是数组  
	if (!pArray.IsArray())
	{
		return cards;
	}

	for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
	{
		Card card;
		const rapidjson::Value &p = pArray[i];
		const rapidjson::Value &numVal = p["number"];
		const rapidjson::Value &colorVal = p["color"];
		const rapidjson::Value &isOpenVal = p["isOpen"];
		card.m_number = numVal.GetInt();
		card.m_color = (CardColor)colorVal.GetInt();
		card.m_isOpen = (bool)isOpenVal.GetInt();
		cards.push_back(card);
	}
	return cards;
}

