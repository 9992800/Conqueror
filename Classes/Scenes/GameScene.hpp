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
#include "FightResultData.hpp"

USING_NS_CC;


class GameScene : public cocos2d::Layer
{
        
public:
        static Scene* createScene(int gameLevel);
        virtual bool init() override;
        CREATE_FUNC2(GameScene, int, level);
        GameScene(int level):_isMoved(false),
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

        void afterPlayerBattle(FightResultData*);
        void afterRobootBattle(FightResultData*);
        void playBattleAnimation(FightResultData*, CallFunc*);
        void playSupplyAnimation(CallFunc*);
        void afterRobootSupply();
        void afterPlayerSupply();
        void gameAction();

        
        void menuEndTurn(Ref* pSender);
        void menuStartGame(Ref* pSender);
        void menuExit(Ref* pSender);
        void gameExit(Ref*, int);
        void gameOver(Ref*, int);
        void menuAnimSwitch(Ref* btn);
        
        void loadXunShouShi();
        void loadZhanshi();
        void loadZhanshi2();
        void loadZhanshi3();
        void loadZhanshi4();
        void loadZhanshi5();
        void loadZhanshi6();
        void loadZhanshi7();
        void loadFightCloud();
        void loadFightResult();
        void loadDiceResultLayer();
        
        void afterShowFightBg(FightResultData*, CallFunc*);
        void afterFightFinished(FightResultData*, CallFunc*);
        void Fighting(FightResultData*, CallFunc*);
        void WinnerBack(FightResultData*, CallFunc*);
        void ShowResultData(FightResultData*);
private:
        int             _playerNumber;
        Layer*          _controlLayer;
        Layer*          _diceResultLayer;
        Sprite*         _animationLayer;
        DiceGame*       _theGameLogic;
        bool            _isMoved;
        bool            _isPalyingAnim; 
        Sprite*         _allFightingCharacters[FIGH_ANIM_MAX][MAX_DICE_PER_AREA];
        int             _gameStatus;
        MenuItemImage*  _endTurnMenuItem;
        MenuItemImage*  _startPlayMenuItem;
        bool            _animationIsOn;
};


#endif /* DiceGameScene_hpp */
