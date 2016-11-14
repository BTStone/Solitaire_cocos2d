#include "CLDScreen.h"
//#include "CLDMacro.h"
#include "STBase.h"
#include "ScreenDirectionManager.h"
#include "ClientCtrl.h"
// 共享对象
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

    // 更新各项参数
    refreshScreenInfo(); 
}

void CLDScreen::refreshScreenInfo()
{
    m_orgWinSize = Director::getInstance()->getWinSize(); 
	m_winSizeInPixels = Director::getInstance()->getWinSizeInPixels();

    // 设备屏幕长宽比
    float orgWinScale = m_orgWinSize.height / m_orgWinSize.width; 
    m_winFixScale = m_enableRetina ? 0.5f : 1.0f; 
    m_winSize = Size(m_resSize.width * m_winFixScale, m_resSize.width * orgWinScale * m_winFixScale); 
    m_winCenter = ccp(m_winSize.width / 2, m_winSize.height /2); 

    // 计算资源文件缩放参数
    float scaleX = m_winSizeInPixels.width / m_resSize.width; 
    float scaleY = m_winSizeInPixels.height / m_resSize.height; 
    m_resFixScale = std::min(scaleX, scaleY); 

    // 资源文件跟屏幕缩放比例
    m_resFixScreenScale.x = m_orgWinSize.width / m_resSize.width / m_resFixScale; 
    m_resFixScreenScale.y = m_orgWinSize.height / m_resSize.height / m_resFixScale; 
    m_screenFixResScale.x = 1.0f / m_resFixScreenScale.x; 
    m_screenFixResScale.y = 1.0f / m_resFixScreenScale.y; 

    // 高比宽像素高，则是竖屏
    m_isPortrait = (m_orgWinSize.height > m_orgWinSize.width); 
    // 是否Iphone5的尺寸
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


// 全局函数
Size CLDFixWinSize()
{
    return gCLDScreen->getWinSize(); 
}

Size CLDWinSize()
{
	//暂时不用
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
	//暂时不用
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
	ST_RETURN_IF(ST_ABS(fixScale - 1.0f) < 0.01f);   // 过滤原尺寸

    sceneNode->setAnchorPoint(Vec2(0,0)); 
    sceneNode->setScale(fixScale); 
}

void screenFix(Size resSize)
{
	//判断横竖屏
	bool isPortrait = (resSize.width < resSize.height);

	// initialize director
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();
	//以下就是如何进行适配的具体方法
    //getFrameSize()获得实际屏幕的大小
	Size frameSize = glview->getFrameSize();
	log("screenFix,frameSize,w:%f,h:%f", frameSize.width, frameSize.height);
	float bigNum = max(frameSize.width, frameSize.height);
	float smallNum = min(frameSize.width, frameSize.height);
	frameSize = (isPortrait) ? Size(smallNum, bigNum) : Size(bigNum, smallNum);
	//这填写的就是一般你作为背景图片的那种图片的大小，适配的原理就是放到和缩小，而以什么为参照，当然就是
    //以最大的那张图片为参照，什么图片最大，当然是背景图片了，以后美工做图的时候用的就是以下的这个尺寸
	Size winSize = resSize;

	//将宽和高做一个比，通过这个比，来具体的调整逻辑分辨率的大小
    float widthRate = frameSize.width / winSize.width;
	float heightRate = frameSize.height / winSize.height;

	glview->setDesignResolutionSize(winSize.width,
		winSize.width * frameSize.height / frameSize.width, kResolutionShowAll);

	return;
	//如果是if中的语句，说明逻辑的高度有点大了，就把逻辑的高缩小到和宽度一样的比率kResolutionNoBorder
	if (widthRate > heightRate)
	{
		//里边传入的前俩个参数就是逻辑分辨率的大小，也就是通过getWinSize()得到的大小
		glview->setDesignResolutionSize(winSize.width,
			winSize.height*heightRate / widthRate, kResolutionShowAll);

	}
	else
	{
		glview->setDesignResolutionSize(winSize.width*widthRate / heightRate, winSize.height,
			kResolutionShowAll);

	}
}
