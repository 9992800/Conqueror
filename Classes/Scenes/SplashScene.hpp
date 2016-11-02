//
//  SplashScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//

#ifndef SplashScene_hpp
#define SplashScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
USING_NS_CC;


class Splash : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(Splash);
        
        virtual void onEnter() override;
        void update(float delta)override;
        virtual void onExit()override;
private:
        int _count = 0;
        LoadingBar* _loadingBar;
};


#endif /* SplashScene_hpp */
