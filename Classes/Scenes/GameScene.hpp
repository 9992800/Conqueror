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
        static Scene* createScene(int gameLevel);
        virtual bool init() override;
        CREATE_FUNC(GameScene);
        
private:
        inline void setLevel(int level){
                _gameLevel = level;
                _playerNumber = _gameLevel + 1;
        }
        
private:
        int _playerNumber;
        int _gameLevel;
};


#endif /* DiceGameScene_hpp */
