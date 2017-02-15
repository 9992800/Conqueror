//
//  GameBattleScene.hpp
//  Conqueror
//
//  Created by wsli on 17/2/8.
//
//

#ifndef GameBattleScene_hpp
#define GameBattleScene_hpp

#include "cocos2d.h"
#include <stdio.h>

using namespace cocos2d::ui;
USING_NS_CC;
using namespace std;

class GameBattle : public cocos2d::Layer{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(GameBattle);
        ~GameBattle();
        void onEnter() override;
        void update(float delta)override;
        virtual void onExit()override;
private:
        
};

#endif /* GameBattleScene_hpp */
