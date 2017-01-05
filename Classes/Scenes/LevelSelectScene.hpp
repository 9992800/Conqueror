//
//  LevelSelecteScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//
#ifndef LevelSelecteScene_hpp
#define LevelSelecteScene_hpp

#include "AppMacros.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GamePayUtil.hpp"
#include "GameData.hpp"
#include "ReplayLastScene.hpp"
#include "SimpleAudioEngine.h"

using namespace cocos2d::ui;
USING_NS_CC;
using namespace std;

class LevelSelect : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(LevelSelect);
        ~LevelSelect();
        void onEnter() override;
        void update(float delta)override;
        virtual void onExit()override;
        
protected:
        void menuOnlineBattle(Ref*);
        void menuStartGame(Ref* btn);
        void menuSoundControl(Ref* btn);
        void menuShowSettigns(Ref* btn);
        void menuGetMoreCoins(Ref* btn);
        void menuGetMoreDices(Ref* btn);
        void menuShowAchievement(Ref*);
        void menuPlayHistory(Ref* btn);
        void pageViewEvent(cocos2d::Ref* sender, cocos2d::ui::PageView::EventType type);
        void btnChosePlayerNum(Ref*, int);
private:
        Size initCenterMainFrame(Vec2);
        void initCharactorSel(Vec2, Size);
        void initColorSel(Vec2, Size);
        void initWalls(Sprite*);
        void initButtons(Vec2 center, Size visibleSize);
        void initMainMenu();
        

        
        void loadResourceInBg(int* loader, HistoryReplayData* data);
        void afterParseArea(void*); 
        void showDailyRewards();
private:
        int                     _count;
        int                     _levelPlayerNum;
        int                     _curCoinsNum, _curMercenariesNum;
        
        Vec2                    _mostRight, _mostLeft;
        LoadingBar*             _loadingBar;
        Sprite*                 _loadingBarBack;
        Sprite*                 _num_sel_back_grd;
        MenuItemImage*          _soundCtrl, *_achievementCtrl;
        HistoryReplayData       _historyData;
        int                     _curChIdx;
        int                     _curColorIdx;
        Label                   *_mercenAriesNumLb, *_coinsNumLb;
        bool                    _soundSwitch, _musicSwitch,_soundTotalOn; 
        CocosDenshion::SimpleAudioEngine* _soundEngine;
};


#endif /* LevelSelecteScene_hpp */
