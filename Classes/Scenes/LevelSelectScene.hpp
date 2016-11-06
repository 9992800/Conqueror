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

using namespace sdkbox;
using namespace cocos2d::ui;
USING_NS_CC;
using namespace std;

class LevelSelect : public cocos2d::Layer, public sdkbox::IAPListener
{
public:
        static Scene* createScene();
        virtual bool init() override;
        void onEnter() override;
        CREATE_FUNC(LevelSelect);
        
        void update(float delta)override;
        virtual void onExit()override;
        
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        
protected:
        void menuSelectLevel(Ref* btn, int num);
        void menuBuyLevel(Ref* btn, std::string name);
        void purchaseCallBack(sdkbox::Product&);
        
        void menuStartGame(Ref* btn);
        void menuSoundControl(Ref* btn);
        void menuShowSettigns(Ref* btn);
        void menuGetMoreCoins(Ref* btn);
        void menuGetMoreDices(Ref* btn);
        
private:
        void loadLevelShow(Vec2 center, Size visibleSize);
        void setSelectLevelBackPos();
        void loadLevelSelectedBackGround();
        void initActionListener();
        
        void initButtons(Vec2 center, Size visibleSize);
        
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
        
private:
        int             _count;
        int             _lastLevel;
        int             _coinsNum;
        int             _dicesNum;
        
        Vec2            _mostRight, _mostLeft;
        LoadingBar*     _loadingBar;
        LayerColor*     _selectedBackGround;
        LayerColor*     _levelShowBackGround;
        MenuItemImage*  _soundCtrl;
        map<string, Product> _productsMap;
};


#endif /* LevelSelecteScene_hpp */
