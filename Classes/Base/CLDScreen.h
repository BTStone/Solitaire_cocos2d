#ifndef __CLD_SCREEN_H__
#define __CLD_SCREEN_H__

#include "cocos2d.h"
USING_NS_CC;

/// 抽象位置点
struct CLDPos
{
    float x;    ///< 水平方向信息值
    float y;    ///< 竖直方向信息值
}; 

/// 抽象尺寸
struct CLDSize
{
    float width;   ///< 宽度
    float height;  ///< 高度
}; 

/// 方位信息
enum CLDPosition
{
    CPCenter = 0,           ///< 正中央
    CPLeftTop,              ///< 左上
    CPTop,                  ///< 正上
    CPRightTop,             ///< 右上
    CPRight,                ///< 正右
    CPRightBottom,          ///< 右下
    CPBottom,               ///< 正下
    CPLeftBottom,           ///< 左下
    CPLeft,                 ///< 正左

    CPNum,                  ///< 方位数量
}; 

/// CLDScreen专门为解决不同设备上不同尺寸适配问题创建
class CLDScreen
{
public: 
    CLDScreen(); 
    /// 是否启用高清屏
    void init(bool isEnableRetina, Size resSize);
    /// 刷新屏幕参数
    void refreshScreenInfo(); 
    /// 将配置位置点转换为窗口位置点
    Vec2 convertConfigPosToWinPos(Vec2 cfgPos);           
    /// 转换配置长度
    float convertConfigLengthToWinLength(float cfgLength); 

protected:
    CC_SYNTHESIZE_READONLY(Size, m_winSize, WinSize);                   ///< 屏幕尺寸
    CC_SYNTHESIZE_READONLY(Vec2, m_winCenter, WinCenter);              ///< 屏幕中心点
    CC_SYNTHESIZE_READONLY(float, m_resFixScale, ResFixScale);            ///< 资源文件修正参数
    CC_SYNTHESIZE_READONLY(float, m_winFixScale, WinFixScale);            ///< 屏幕尺寸修正参数

    CC_SYNTHESIZE_READONLY(Size, m_orgWinSize, OrgWinSize);             ///< 原始屏幕尺寸
    CC_SYNTHESIZE_READONLY(Size, m_winSizeInPixels, WinSizeInPixels);   ///< 屏幕像素尺寸
    CC_SYNTHESIZE_READONLY(bool, m_enableRetina, EnableRetina);           ///< 是否使用高清屏幕
    CC_SYNTHESIZE_READONLY(bool, m_isPortrait, Portrait);                 ///< 是否竖屏
    CC_SYNTHESIZE_READONLY(Size, m_resSize, ResSize);                   ///< 资源文件尺寸

    CC_SYNTHESIZE_READONLY(bool, m_isIphone5Size, IsIphone5Size);               ///< 是否Iphone5的尺寸（1136×640）
    CC_SYNTHESIZE_READONLY(Vec2, m_resFixScreenScale, ResFixScreenScale);    ///< 资源文件适应屏幕的缩放比例
    CC_SYNTHESIZE_READONLY(Vec2, m_screenFixResScale, ScreenFixResScale);    ///< 屏幕反适应资源文件的缩放比例
}; 

/// 全局变量，屏幕尺寸共享对象
extern CLDScreen * gCLDScreen; 


// 屏幕尺寸相关全局函数
/// 屏幕尺寸
Size CLDWinSize(); 
Size CLDFixWinSize(); 
float CLDScreenFixScale(); 
float CLDWinFixScale(); 
/// 屏幕中心点
Vec2 CLDWinCenter();
float CLDCfgLength(float length); 
/// 通过x、y轴比例值定位到屏幕上的点
Vec2 CLDWinPos(float x, float y);

void screenFix(Size resSize);


inline float CLDGetStdWidth()
{
    static float stdWidth = -1.0f; 
    if (stdWidth < 0)
    {
        stdWidth = CLDWinSize().width > CLDWinSize().height ? 1024 : 768; 
    }
    return stdWidth; 
}

inline float CLDGetStdHeight()
{
    static float stdHeight = -1.0f; 
    if (stdHeight < 0)
    {
        stdHeight = CLDWinSize().width > CLDWinSize().height ? 768 : 1024; 
    }
    return stdHeight; 
}


// 以1024×768为标准，纯数字的值需要加以修正
inline float CLDGetFactorX()
{
    static float factorX = -1;
    if (factorX < 0)
    {
        if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
			factorX = Director::getInstance()->getWinSize().width / CLDGetStdWidth();
        }
        else
        {
            factorX = 1.0f; 
        }
    }
    return factorX; 
}

inline float CLDGetFactorY()
{
    static float factorY = -1;
    if (factorY < 0)
    {
		if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
			factorY = Director::getInstance()->getWinSize().height / CLDGetStdHeight();
        }
        else
        {
            factorY = 1.0f;
        }
    }
    return factorY; 
}

inline float CLDGetFactor()
{
    static float factor = (CLDGetFactorX() + CLDGetFactorY()) / 2.0f; 
    return factor; 
}
#define CLDFixX(width) (width * CLDGetFactorX())
#define CLDFixY(height) (height * CLDGetFactorY())
//#define CLDFix(size) (size * CLDGetFactor())
#define CLDFix(size)   ((size) * CLDWinFixScale())
//#define CLDFontSize(size) ((size) / gCLDScreen->getResFixScale())
#define CLDFontSize(size) CLDFix(size)

void CLDFullScreenFix(Node * sceneNode); 


#define CLD_HIT_RANGE    CLDFix(20)    // 20像素以内移动的触碰都认为是点击
#define CLD_NEAR_OFFSET  CLDFix(50)    // 50像素以内认为是近距离


#endif // __CLD_SCREEN_H__

