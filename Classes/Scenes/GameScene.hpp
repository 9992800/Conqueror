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
        static Scene* createScene(int gameLevel, int charactorIdx = 0, int colorIdx = 0);
        virtual bool init() override;
        CREATE_FUNC(GameScene);
        GameScene():_isMoved(false),
        _isPalyingAnim(false),
        _gameStatus(GAME_STATUS_INIT){
        }     
        ~GameScene();
        
private:         
        void initAreaTcShow();
        void initOperateBoard();
        void initControlLayer();
        void initAnimationLayer();
        
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        void onTouchesEnded(const std::vector<Touch*>& touches, Event *event)override;
        void tryAgain();

        void afterPlayerBattle(FightResultData*);
        void afterRobootBattle(FightResultData*);
        void playBattleAnimation(FightResultData*, CallFunc*, bool);
        void playSupplyAnimation(CallFunc*);
        void afterRobootSupply();
        void afterPlayerSupply();
        void refreshAreaTcShow(std::map<int, int>);
        void gameAction();

        
        void menuEndTurn(Ref* pSender);
        void menuStartGame(Ref* pSender);
        void menuExit(Ref* pSender);
        void gameExit(Ref*, int);
        void gameOver(Ref*, int);
        void menuAnimSwitch(Ref* btn);
        
        void loadCharact(int, std::string);
        void loadDiceResultLayer();
        void initMapSize(GameData*);
        
        void afterShowFightBg(FightResultData*, CallFunc*);
        void afterFightFinished(FightResultData*, CallFunc*);
        void Fighting(FightResultData*, CallFunc*);
        void WinnerBack(FightResultData*, CallFunc*);
        void ShowResultData(FightResultData*);
private:
        static int      _playerNumber, _charactorIdx, _colorIdx;
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
        Size            _showAreaSize;
        Vec2            _invaderPos[8], _keeperPos[8];
        GameData*       _curGameData;
        std::map<int, Label*> _supplyLabelMap;
        LayerColor      *_mapLayer;
        Rect            _minFrameShow, _maxFrameShow;
};


#endif /* DiceGameScene_hpp */
