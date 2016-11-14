#ifndef __CLD_SCREEN_H__
#define __CLD_SCREEN_H__

#include "cocos2d.h"
USING_NS_CC;

/// ����λ�õ�
struct CLDPos
{
    float x;    ///< ˮƽ������Ϣֵ
    float y;    ///< ��ֱ������Ϣֵ
}; 

/// ����ߴ�
struct CLDSize
{
    float width;   ///< ���
    float height;  ///< �߶�
}; 

/// ��λ��Ϣ
enum CLDPosition
{
    CPCenter = 0,           ///< ������
    CPLeftTop,              ///< ����
    CPTop,                  ///< ����
    CPRightTop,             ///< ����
    CPRight,                ///< ����
    CPRightBottom,          ///< ����
    CPBottom,               ///< ����
    CPLeftBottom,           ///< ����
    CPLeft,                 ///< ����

    CPNum,                  ///< ��λ����
}; 

/// CLDScreenר��Ϊ�����ͬ�豸�ϲ�ͬ�ߴ��������ⴴ��
class CLDScreen
{
public: 
    CLDScreen(); 
    /// �Ƿ����ø�����
    void init(bool isEnableRetina, Size resSize);
    /// ˢ����Ļ����
    void refreshScreenInfo(); 
    /// ������λ�õ�ת��Ϊ����λ�õ�
    Vec2 convertConfigPosToWinPos(Vec2 cfgPos);           
    /// ת�����ó���
    float convertConfigLengthToWinLength(float cfgLength); 

protected:
    CC_SYNTHESIZE_READONLY(Size, m_winSize, WinSize);                   ///< ��Ļ�ߴ�
    CC_SYNTHESIZE_READONLY(Vec2, m_winCenter, WinCenter);              ///< ��Ļ���ĵ�
    CC_SYNTHESIZE_READONLY(float, m_resFixScale, ResFixScale);            ///< ��Դ�ļ���������
    CC_SYNTHESIZE_READONLY(float, m_winFixScale, WinFixScale);            ///< ��Ļ�ߴ���������

    CC_SYNTHESIZE_READONLY(Size, m_orgWinSize, OrgWinSize);             ///< ԭʼ��Ļ�ߴ�
    CC_SYNTHESIZE_READONLY(Size, m_winSizeInPixels, WinSizeInPixels);   ///< ��Ļ���سߴ�
    CC_SYNTHESIZE_READONLY(bool, m_enableRetina, EnableRetina);           ///< �Ƿ�ʹ�ø�����Ļ
    CC_SYNTHESIZE_READONLY(bool, m_isPortrait, Portrait);                 ///< �Ƿ�����
    CC_SYNTHESIZE_READONLY(Size, m_resSize, ResSize);                   ///< ��Դ�ļ��ߴ�

    CC_SYNTHESIZE_READONLY(bool, m_isIphone5Size, IsIphone5Size);               ///< �Ƿ�Iphone5�ĳߴ磨1136��640��
    CC_SYNTHESIZE_READONLY(Vec2, m_resFixScreenScale, ResFixScreenScale);    ///< ��Դ�ļ���Ӧ��Ļ�����ű���
    CC_SYNTHESIZE_READONLY(Vec2, m_screenFixResScale, ScreenFixResScale);    ///< ��Ļ����Ӧ��Դ�ļ������ű���
}; 

/// ȫ�ֱ�������Ļ�ߴ繲�����
extern CLDScreen * gCLDScreen; 


// ��Ļ�ߴ����ȫ�ֺ���
/// ��Ļ�ߴ�
Size CLDWinSize(); 
Size CLDFixWinSize(); 
float CLDScreenFixScale(); 
float CLDWinFixScale(); 
/// ��Ļ���ĵ�
Vec2 CLDWinCenter();
float CLDCfgLength(float length); 
/// ͨ��x��y�����ֵ��λ����Ļ�ϵĵ�
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


// ��1024��768Ϊ��׼�������ֵ�ֵ��Ҫ��������
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


#define CLD_HIT_RANGE    CLDFix(20)    // 20���������ƶ��Ĵ�������Ϊ�ǵ��
#define CLD_NEAR_OFFSET  CLDFix(50)    // 50����������Ϊ�ǽ�����


#endif // __CLD_SCREEN_H__

