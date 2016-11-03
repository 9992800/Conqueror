//
//  DiceGameScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#ifndef DiceGameScene_hpp
#define DiceGameScene_hpp

#include "cocos2d.h"
USING_NS_CC;


class GameScene : public cocos2d::Layer
{
        
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(GameScene);
};


#endif /* DiceGameScene_hpp */
