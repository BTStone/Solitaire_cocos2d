#ifndef __ST_MACRO_H__
#define __ST_MACRO_H__

// 常用错误码
#define ST_SUCCESS   1
#define ST_ERROR     0
inline void confuse_func_magic_sunflower_card_solitaire_free(){}

// 常用逻辑判断处理
#define ST_RETURN_ERR_IF(value) if((value)){\
	confuse_func_magic_sunflower_card_solitaire_free();\
	return ST_ERROR; \
};
#define ST_RETURN_NULL_IF(value) if ((value)){\
	confuse_func_magic_sunflower_card_solitaire_free();\
	 return NULL; \
};
#define ST_RETURN_FALSE_IF(value) if ((value)){\
	confuse_func_magic_sunflower_card_solitaire_free();\
	 return false; \
};
#define ST_RETURN_IF(value) if ((value)){\
	confuse_func_magic_sunflower_card_solitaire_free();\
	 return; \
};

// 获取绝对值
#define ST_ABS(value) ((value) >= 0 ? (value) : -(value))

/// 数组长度
#define ST_ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

// 自动创建场景
#define ST_SCENE_FUNC(__TYPE__) static Scene * scene() {\
\
	Scene * scene = NULL; \
	do \
		{\
		scene = Scene::create();\
		CC_BREAK_IF(! scene);\
		__TYPE__ * layer = __TYPE__::create();\
		CC_BREAK_IF(! layer);\
		scene->addChild(layer);\
		} while (0);\
	return scene;\
}; 

#define ST_SAFE_REMOVE_FROM_PARENT(p)           if(p){ p->removeFromParent(); (p) = nullptr; }



#endif
