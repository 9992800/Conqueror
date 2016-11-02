#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
USING_NS_CC;


class Starting : public cocos2d::Layer
{
public:
        static cocos2d::Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(Starting);
        
        
        virtual void onEnter() override;
        virtual void onExit() override;
        void update(float delta)override;
        
        
        void loadingResource();
private:
        int _count = 0;
        LoadingBar* _loadingBar;
};

#endif // __HELLOWORLD_SCENE_H__
