#ifndef __ST_MACRO_H__
#define __ST_MACRO_H__

// ���ô�����
#define ST_SUCCESS   1
#define ST_ERROR     0
inline void confuse_func_magic_sunflower_card_solitaire_free(){}

// �����߼��жϴ���
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

// ��ȡ����ֵ
#define ST_ABS(value) ((value) >= 0 ? (value) : -(value))

/// ���鳤��
#define ST_ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

// �Զ���������
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
