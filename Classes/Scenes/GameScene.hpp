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
        
protected:
        void initMapLayer();
        void initControlLayer();
        void initAnimationLayer();
        
protected:
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        void onTouchesEnded(const std::vector<Touch*>& touches, Event *event)override;
 
private:
        void playAnimation(int);
private:
        int _playerNumber;
        int _gameLevel;
        
private:
        Layer*  _controlLayer;
        Layer*  _animationLayer;
        
        
private:
        DiceGame*       _theGameLogic;
        
private:
        bool            _isMoved;
        float           _lowestPostion_y;
};


#endif /* DiceGameScene_hpp */
