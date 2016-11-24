#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;

class Starting : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(Starting);

        void update(float delta)override;
        virtual void onExit()override;
        
protected:
        void menuStartGame(Ref* pSender);
        void menuHelp(Ref* pSender);
        void menuSoundCtrl(Ref* pSender);
        void menuShareGame(Ref* pSender);
        void menuOnlineBattle(Ref* pSender);
        
private:
        void afterCaptureScreen(bool, const std::string&);
        
private:
        int _count = 0;
        LoadingBar* _loadingBar;
        MenuItemImage* _startGame,
                        *_soundCtrl,
                        *_helpBtn,
                        *_onlineBattle;
};

#endif // __HELLOWORLD_SCENE_H__
