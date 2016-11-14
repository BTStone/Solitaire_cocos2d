#include "AppDelegate.h"
#include "GameLayer.h"
#include "CLDScreen.h"
#include "STBase.h"
#include "StartLayer.h"
#include "ClientCtrl.h"
USING_NS_CC;


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)//android
#include "MobClickCpp.h"
using namespace umeng;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  //ios
#include "MobClickCpp.h"
using namespace umeng;
#else         //win32

#endif

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    // turn on display FPS
    director->setDisplayStats(false);
	ClientCtrl::share()->init();
    // set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);
	setFrameSizeOfWin32();
	Size bgSize = Size(720, 1280);
	screenFix(bgSize);
	preloadPicture();
	preloadSound();
    // Set the design resolution
    //glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    /*Size frameSize = glview->getFrameSize();
    //// if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
	*/
	register_all_packages();

    // create a scene. it's an autorelease object
	auto scene = StartLayer::scene();

    // run
    director->runWithScene(scene);

    return true;
}

void AppDelegate::preloadSound()
{
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_FLIP_CARD_1);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_FLIP_CARD_3);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_MOVE_NO_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_NEW_GAME_DEAL);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_MENU_IN);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_MENU_OUT);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_NEW_RECORD);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_UNDO);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_VICTORY);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_PLACE_CARD);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_ACHIEVEMENT);
	SimpleAudioEngine::getInstance()->preloadEffect(SOUND_COLLECT_CARD);

	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
	SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
}

void AppDelegate::preloadPicture()
{
	vector<string> searchPath; //搜索资源路径
	searchPath.push_back("Resources");
	searchPath.push_back("cocostudio");
	searchPath.push_back("cocostudio/Default");
	searchPath.push_back("cocostudio/ui");
	searchPath.push_back("cocostudio/land");
	searchPath.push_back("cocostudio/port");

	FileUtils::getInstance()->setSearchPaths(searchPath); //设置资源搜索路径 
	vector<string> r = FileUtils::getInstance()->getSearchPaths();

}

void AppDelegate::setFrameSizeOfWin32()
{

	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	{
#define CLD_DEVICE  3
#if (CLD_DEVICE == 1)
		Size winSize = Size(768, 1024);   // ipad 1, 2
		char * resPath = "ipad";
#elif (CLD_DEVICE == 2)
		Size winSize = Size(1536, 2048);  // new ipad
		char * resPath = "ipadhd";
#elif (CLD_DEVICE == 3)
		Size winSize = Size(640, 1136);   // iphone 5
		char * resPath = "ipad";
#elif (CLD_DEVICE == 4)
		Size winSize = Size(640, 960);    // ipod 5
		char * resPath = "ipad";
#endif
		if (St_is_portrait == 0)
		{
			winSize = Size(winSize.height, winSize.width);
		}
		float scal = 1.0f;
		Size winSize2 = Size(550, 704);
		Director::getInstance()->getOpenGLView()->setFrameSize(winSize.width / scal, winSize.height / scal);
		Director::getInstance()->getOpenGLView()->setFrameZoomFactor(0.5f);
		//Director::getInstance()->getOpenGLView()->setDesignResolutionSize(winSize.width, winSize.height, ResolutionPolicy::SHOW_ALL);
		if (0)
		{
			Director::getInstance()->getOpenGLView()->setFrameSize(winSize.height / scal, winSize.width / scal);
			Director::getInstance()->getOpenGLView()->setDesignResolutionSize(winSize.height, winSize.width, ResolutionPolicy::SHOW_ALL);
			return;
		}
		gCLDScreen->init(false, Size(768, 1024));
	}
	

}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	NotificationCenter::getInstance()->postNotification(ST_GAME_ENTER_BACKGROUND);
	ClientCtrl::share()->umengApplicationDidEnterBackground();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
	ClientCtrl::share()->umengApplicationWillEnterForeground();
	//ClientCtrl::share()->cancelNotification(Notification_No_Win);
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
