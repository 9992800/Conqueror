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
#include "PluginIAP/PluginIAP.h"
#include "GameData.hpp"
#include "ReplayLastScene.hpp"

using namespace sdkbox;
using namespace cocos2d::ui;
USING_NS_CC;
using namespace std;

class LevelSelect : public cocos2d::Layer, public sdkbox::IAPListener
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(LevelSelect);
        ~LevelSelect();
        
        void onEnter() override;
        void update(float delta)override;
        virtual void onExit()override;
        
//        void onTouchesMoved(Touch*, Event* event);
//        bool onTouchBegan(Touch*, Event*)override{return true;}
        
protected:
        void menuOnlineBattle(Ref*);
        void menuStartGame(Ref* btn);
        void menuSoundControl(Ref* btn);
        void menuShowSettigns(Ref* btn);
        void menuGetMoreCoins(Ref* btn);
        void menuGetMoreDices(Ref* btn);
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
        
private:
        void onInitialized(bool success)override{
        }
        void onSuccess(const Product& p)override;
        void onFailure(const Product& p, const std::string& msg)override;
        void onCanceled(const Product& p)override;
        void onRestored(const Product& p)override{
        };
        void onProductRequestSuccess(const std::vector<Product>& products)override;
        void onProductRequestFailure(const std::string& msg)override;
        void onRestoreComplete(bool ok, const std::string &msg)override;
        
        void loadResourceInBg(int* loader, HistoryReplayData* data);
        void afterParseArea(void*);
        
private:
        int             _count;
        int             _levelPlayerNum;
        int             _coinsNum;
        int             _dicesNum;
        
        Vec2                    _mostRight, _mostLeft;
        LoadingBar*             _loadingBar;
        Sprite*                 _loadingBarBack;
        Sprite*                 _num_sel_back_grd;
        MenuItemImage*          _soundCtrl;
        map<string, Product>    _productsMap;
        HistoryReplayData       _historyData;
        int                     _curChIdx;
        int                     _curColorIdx;
};


#endif /* LevelSelecteScene_hpp */
