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
        _isMoved(false),
        _isPalyingAnim(false),
        _gameStatus(GAME_STATUS_INIT),
        _playerNumber(level){
        }     
        ~GameScene();
        
private:
        void initMapLayer();
        void initControlLayer();
        void initAnimationLayer();
        
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        void onTouchesEnded(const std::vector<Touch*>& touches, Event *event)override;
        void tryAgain();

        void afterPlayerBattle(int result);
        void afterRobootBattle(int result);
        void playBattleAnimation(int, CallFunc*, bool isManual);
        void playSupplyAnimation(CallFunc*);
        void afterRobootSupply();
        void afterPlayerSupply();
        void gameAction();

        
        void menuEndTurn(Ref* pSender);
        void menuStartGame(Ref* pSender);
        void menuExit(Ref* pSender);
        void gameExit(Ref*, int);
        void gameOver(Ref*, int);
        
        void loadXunShouShi();
        void loadZhanshi();
        
private:
        int             _playerNumber;
        Layer*          _controlLayer;
        Sprite*         _animationLayer;
        DiceGame*       _theGameLogic;
        bool            _isMoved;
        bool            _isPalyingAnim;
        float           _lowestPostion_y;
        Sprite*         _allFightingCharacters[8][8];
        int             _gameStatus;
        MenuItemImage*  _endTurnMenuItem;
        MenuItemImage*  _startPlayMenuItem;
};


#endif /* DiceGameScene_hpp */
