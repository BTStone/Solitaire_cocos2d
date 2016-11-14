#ifndef __ST_HELPER_H__
#define __ST_HELPER_H__
#include "cocos2d.h"
#include "STMacro.h"
#include "CLDScreen.h"
#include <iostream>
USING_NS_CC;
using namespace std;
class STHelper
{
public:
	static void separateTouch(Node *board);
	static string getDocPath();
	static void addDarkLayer(Node *board);
protected:

};

#endif
