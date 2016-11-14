#include "CLDScreen.h"
//#include "CLDMacro.h"
#include "STBase.h"
#include "ScreenDirectionManager.h"
#include "ClientCtrl.h"
// �������
static CLDScreen sCLDScreenObject; 
CLDScreen * gCLDScreen = &sCLDScreenObject; 

CLDScreen::CLDScreen()
: m_enableRetina(false)
, m_isPortrait(true)
, m_resFixScale(1.0f)
, m_winFixScale(1.0f)
{
}

void CLDScreen::init(bool isEnableRetina, Size resSize)
{
    m_enableRetina = isEnableRetina; 
    m_resSize = resSize; 

    // ���¸������
    refreshScreenInfo(); 
}

void CLDScreen::refreshScreenInfo()
{
    m_orgWinSize = Director::getInstance()->getWinSize(); 
	m_winSizeInPixels = Director::getInstance()->getWinSizeInPixels();

    // �豸��Ļ�����
    float orgWinScale = m_orgWinSize.height / m_orgWinSize.width; 
    m_winFixScale = m_enableRetina ? 0.5f : 1.0f; 
    m_winSize = Size(m_resSize.width * m_winFixScale, m_resSize.width * orgWinScale * m_winFixScale); 
    m_winCenter = ccp(m_winSize.width / 2, m_winSize.height /2); 

    // ������Դ�ļ����Ų���
    float scaleX = m_winSizeInPixels.width / m_resSize.width; 
    float scaleY = m_winSizeInPixels.height / m_resSize.height; 
    m_resFixScale = std::min(scaleX, scaleY); 

    // ��Դ�ļ�����Ļ���ű���
    m_resFixScreenScale.x = m_orgWinSize.width / m_resSize.width / m_resFixScale; 
    m_resFixScreenScale.y = m_orgWinSize.height / m_resSize.height / m_resFixScale; 
    m_screenFixResScale.x = 1.0f / m_resFixScreenScale.x; 
    m_screenFixResScale.y = 1.0f / m_resFixScreenScale.y; 

    // �߱ȿ����ظߣ���������
    m_isPortrait = (m_orgWinSize.height > m_orgWinSize.width); 
    // �Ƿ�Iphone5�ĳߴ�
    m_isIphone5Size = m_isPortrait ? (m_winSizeInPixels.height == 1136) : (m_winSizeInPixels.width == 1136); 
}

Vec2 CLDScreen::convertConfigPosToWinPos(Vec2 cfgPos)
{
    return ccp(cfgPos.x * m_winFixScale, cfgPos.y * m_winFixScale); 
}

float CLDScreen::convertConfigLengthToWinLength(float cfgLength)
{
    return cfgLength * m_winFixScale; 
}


// ȫ�ֺ���
Size CLDFixWinSize()
{
    return gCLDScreen->getWinSize(); 
}

Size CLDWinSize()
{
	//��ʱ����
	Size winSize = Director::getInstance()->getWinSize();// Director::getInstance()->getWinSize();
	float minS = min(winSize.width, winSize.height);
	float maxS = max(winSize.width, winSize.height);
	bool isPortrait = ClientCtrl::share()->isPortrait();
	if (isPortrait)
	{
		winSize = Size(minS, maxS);
	}
	else
	{
		winSize = Size(maxS, minS);
	}
	return winSize;

}

float CLDScreenFixScale()
{
    return gCLDScreen->getResFixScale();  
}

Vec2 CLDWinCenter()
{
	//��ʱ����
	return Vec2(CLDWinSize().width / 2.0, CLDWinSize().height / 2.0);


    return gCLDScreen->getWinCenter(); 
}

float CLDWinFixScale()
{
    return gCLDScreen->getWinFixScale();  
}

float CLDCfgLength(float length)
{
    return gCLDScreen->convertConfigLengthToWinLength(length); 
}

Vec2 CLDWinPos(float x, float y)
{
    return ccp(CLDWinSize().width * x, CLDWinSize().height * y); 
}

void CLDFullScreenFix(Node * sceneNode)
{
    float fixScale = CLDScreenFixScale(); 
	ST_RETURN_IF(ST_ABS(fixScale - 1.0f) < 0.01f);   // ����ԭ�ߴ�

    sceneNode->setAnchorPoint(Vec2(0,0)); 
    sceneNode->setScale(fixScale); 
}

void screenFix(Size resSize)
{
	//�жϺ�����
	bool isPortrait = (resSize.width < resSize.height);

	// initialize director
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();
	//���¾�����ν�������ľ��巽��
    //getFrameSize()���ʵ����Ļ�Ĵ�С
	Size frameSize = glview->getFrameSize();
	log("screenFix,frameSize,w:%f,h:%f", frameSize.width, frameSize.height);
	float bigNum = max(frameSize.width, frameSize.height);
	float smallNum = min(frameSize.width, frameSize.height);
	frameSize = (isPortrait) ? Size(smallNum, bigNum) : Size(bigNum, smallNum);
	//����д�ľ���һ������Ϊ����ͼƬ������ͼƬ�Ĵ�С�������ԭ����Ƿŵ�����С������ʲôΪ���գ���Ȼ����
    //����������ͼƬΪ���գ�ʲôͼƬ��󣬵�Ȼ�Ǳ���ͼƬ�ˣ��Ժ�������ͼ��ʱ���õľ������µ�����ߴ�
	Size winSize = resSize;

	//����͸���һ���ȣ�ͨ������ȣ�������ĵ����߼��ֱ��ʵĴ�С
    float widthRate = frameSize.width / winSize.width;
	float heightRate = frameSize.height / winSize.height;

	glview->setDesignResolutionSize(winSize.width,
		winSize.width * frameSize.height / frameSize.width, kResolutionShowAll);

	return;
	//�����if�е���䣬˵���߼��ĸ߶��е���ˣ��Ͱ��߼��ĸ���С���Ϳ��һ���ı���kResolutionNoBorder
	if (widthRate > heightRate)
	{
		//��ߴ����ǰ�������������߼��ֱ��ʵĴ�С��Ҳ����ͨ��getWinSize()�õ��Ĵ�С
		glview->setDesignResolutionSize(winSize.width,
			winSize.height*heightRate / widthRate, kResolutionShowAll);

	}
	else
	{
		glview->setDesignResolutionSize(winSize.width*widthRate / heightRate, winSize.height,
			kResolutionShowAll);

	}
}
