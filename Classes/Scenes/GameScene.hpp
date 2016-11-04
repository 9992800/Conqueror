//
//  DiceGameScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#ifndef DiceGameScene_hpp
#define DiceGameScene_hpp
#include "DiceGame.hpp"
#include "cocos2d.h"
USING_NS_CC;


class GameScene : public cocos2d::Layer
{
        
public:
        static Scene* createScene(int gameLevel);
        virtual bool init() override;
        CREATE_FUNC2(GameScene, int, level);
        GameScene(int level):_lowestPostion_y(0.f),
        _isMoved(false){
                _gameLevel = level;
                _playerNumber = level + 1;
        }     
        
        ~GameScene(){
                _theGameLogic->release();
        }
        
private:
        void initMapLayer();
        void initControlLayer();
        void initAnimationLayer();
        
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        void onTouchesEnded(const std::vector<Touch*>& touches, Event *event)override;
        void tryAgain();

        void afterPlayerBattle(int result);
        void playBattleAnimation(int, CallFunc*);
        
private:
        int             _playerNumber;
        int             _gameLevel;
        Layer*          _controlLayer;
        Layer*          _animationLayer;
        DiceGame*       _theGameLogic;
        bool            _isMoved;
        float           _lowestPostion_y;
        Sprite*         _tamara;
};


#endif /* DiceGameScene_hpp */
