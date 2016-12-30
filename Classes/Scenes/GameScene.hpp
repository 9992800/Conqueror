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
#include "ui/CocosGUI.h"
#include "PluginFacebook/PluginFacebook.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
class GameScene : public cocos2d::Layer, sdkbox::FacebookListener
{
        
public:
        static Scene* createScene(int gameLevel, int charactorIdx = 0, int colorIdx = 0);
        virtual bool init() override;
        CREATE_FUNC(GameScene);
        GameScene():_isMoved(false),
        _isPalyingAnim(false),
        _attackResult(nullptr),
        _gameStatus(GAME_STATUS_INIT){
        }     
        ~GameScene();
        
private:         
        void initAreaTcShow();
        void initOperateBoard();
        void initControlLayer();
        void initAnimationLayer();
        void initDialog();
        
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        void onTouchesEnded(const std::vector<Touch*>& touches, Event *event)override;
        void tryAgain();

        void afterPlayerBattle();
        void afterRobootBattle();
        void playBattleAnimation(bool);
        void playSupplyAnimation();
        void playSupplyAnimation2(GamePlayer*);
        void afterSupply();
        void refreshAreaTcShow(std::map<int, int>);
        void refreshSupplyDiceNum();
        void gameAction();

        
        void menuEndTurn(Ref* pSender);
        void createNewMap(Ref* pSender);
        void menuStartGame(Ref* pSender, Layer*);
        void menuExit(Ref* pSender);
        void gameOver(Ref*, int);
        void shareThisGame(Ref*);
        void afterCaptureScreen(bool yes, const std::string &outputFilename);
        void menuAnimSwitch(Ref* btn);
        void menuAddArmy(Ref*);
        
        void loadCharact(int, std::string);
        void initMapSize(GameData*);
        
        void afterShowFightBg();
        void afterFightFinished();
        void Fighting();
        void WinnerBack();
        void ShowResultData();
        
protected:
        void onLogin(bool, const std::string&)override;
        void onSharedSuccess(const std::string&)override;
        void onSharedFailed(const std::string&)override;
        void onSharedCancel()override;
        void onAPI(const std::string&, const std::string&)override;
        void onPermission(bool, const std::string&)override;
        void onFetchFriends(bool, const std::string&)override;
        void onRequestInvitableFriends(const sdkbox::FBInvitableFriendsInfo&)override;
        void onInviteFriendsWithInviteIdsResult(bool, const std::string&)override;
        void onInviteFriendsResult(bool, const std::string&)override;
        void onGetUserInfo(const sdkbox::FBGraphUser&)override;
        
        void onEnter() override;
        void update(float delta)override;
        virtual void onExit()override;
        
private:
        inline void playSoundEffect(const char *sound = EFFECT_FILE_SELECTED){
                if (_soundTotalOn){
                        _soundEngine->playEffect(sound);
                }
        }
        
private:
        static int      _playerNumber, _charactorIdx, _colorIdx;
        Layer*          _controlLayer;
        Layer*          _diceResultLayer;
        Layer*          _endTurnTipsLayer;
        Layer*          _supplyShowLayer;
        Sprite*         _animationLayer;
        DiceGame*       _theGameLogic;
        bool            _isMoved;
        bool            _isPalyingAnim; 
        Sprite*         _allFightingCharacters[FIGH_ANIM_MAX][MAX_DICE_PER_AREA];
        int             _gameStatus;
        bool            _animationIsOn;
        Size            _showAreaSize;
        Vec2            _invaderPos[8], _keeperPos[8];
        GameData*       _curGameData;
        std::map<int, Label*> _supplyLabelMap;
        LayerColor      *_mapLayer;
        Rect            _minFrameShow, _maxFrameShow;
        cocos2d::ui::Button*         _animCtlBtn, *_addArmyBtn;
        int             _addtionalSupplyCounter;
        Sprite*         _curPlayerSupFlag, *_curInTurnBack;
        FightResultData* _attackResult;
        CallFunc*       _afterBattleCallback, *_afterSupplyCallback;
        Layer           *_winDialogLayer, *_lostDialogLayer;
        int             _curCoinsNo, _curSupplyNo;
        
        bool            _soundTotalOn, _musicSwitch;
        CocosDenshion::SimpleAudioEngine* _soundEngine;
};


#endif /* DiceGameScene_hpp */
