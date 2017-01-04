//
//  BuySupplyScene.hpp
//  Conqueror
//
//  Created by wsli on 16/12/28.
//
//

#ifndef BuySupplyScene_hpp
#define BuySupplyScene_hpp

#include "AppMacros.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


class BuySupply : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(BuySupply);
        
        BuySupply():_soundTotalOn(true)
        ,_spawnCount(5),
        _bufferZone(90),
        _updateTimer(0),
        _updateInterval(1.0f / 24),
        _lastContentPosY(0),
        _itemTemplateHeight(0){
        }
        
        virtual void update(float dt) override;
        virtual void onEnter() override;
        virtual void onExit()override;
        
private:
        void actionGetItem(Ref*);
        
        void initItemData();
        void initCurCoins(Node*);
        void playCoinsSubAnim(MercenaryItem);
        
        ui::Layout* createListItem();
        void initItemDetails(ui::Widget*, int);
        void updateItem(int itemID, int templateID);
        float getItemPositionYInView(cocos2d::ui::Widget* item) const;

private:
        bool                                    _soundTotalOn;
        CocosDenshion::SimpleAudioEngine*       _soundEngine;
        
        
        ui::ListView* _listView;
        int _totalCount, _spawnCount;
        float _itemTemplateHeight, _reuseItemOffset;
        float _updateTimer, _updateInterval;
        float _bufferZone, _lastContentPosY;
        
        Sprite  *_coinsShow,  *_mercenaryShow;
        Label   *_coinsNumLb, *_mercenAriesNumLb;
        std::vector<MercenaryItem>      _mercenaryData;
};

#endif /* BuySupplyScene_hpp */
