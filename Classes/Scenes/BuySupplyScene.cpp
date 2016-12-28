//
//  BuySupplyScene.cpp
//  Conqueror
//
//  Created by wsli on 16/12/28.
//
//

#include "BuySupplyScene.hpp"
#include "ShoppingScene.hpp"

bool BuySupply::init(){
        
        if (!LayerColor::initWithColor(Color4B::BLACK)){
                return false;
        }
        this->setOpacity(180);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        auto buy_supply_back = Sprite::create("shopping/buy_supply_back.png");
        buy_supply_back->setPosition(visibleSize / 2);
        this->addChild(buy_supply_back);
        auto buy_supply_back_size = buy_supply_back->getContentSize();
        
        //Exit button
        auto buy_btn = ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        auto buy_btn_size = buy_btn->getContentSize();
        buy_btn->setPosition(Vec2(buy_supply_back_size.width * 0.5f - buy_btn_size.width, buy_btn->getContentSize().height * 2.f));
        
        buy_btn->addClickEventListener(CC_CALLBACK_1(BuySupply::menuSpendCoins, this, 1));
        buy_btn->setTitleText("OK");
        buy_btn->setTitleFontName("fonts/arial.ttf");
        buy_btn->setTitleFontSize(34);
        buy_supply_back->addChild(buy_btn);
        
        
        auto cancel_btn = ui::Button::create("DIALOG_CANCEL.png", "DIALOG_CANCEL_SEL.png");
        cancel_btn->setPosition(Vec2(buy_supply_back_size.width * 0.5f + buy_btn_size.width, buy_btn->getContentSize().height * 2.f));
        cancel_btn->addClickEventListener(CC_CALLBACK_1(BuySupply::menuSpendCoins, this, 0));
        cancel_btn->setTitleText("NO");
        cancel_btn->setTitleFontName("fonts/arial.ttf");
        cancel_btn->setTitleFontSize(34);
        buy_supply_back->addChild(cancel_btn);
        
        
        
        auto tips_title = Sprite::create("common_dialog_title.png");
        tips_title->setPosition(Vec2(buy_supply_back_size.width * 0.5f,
                               buy_supply_back_size.height + tips_title->getContentSize().height * 0.3f));
        buy_supply_back->addChild(tips_title);
        
        auto label_tips = Label::createWithSystemFont("Got 8 mercenaries by 10 gold coins.", "fonts/arial.ttf", 28);
        
        label_tips->setColor(Color3B::ORANGE);
        Vec2 label_tips_pos(buy_supply_back_size.width * 0.5f,
                            buy_supply_back_size.height - label_tips->getContentSize().height * 1.2f);
        label_tips->setPosition(label_tips_pos);
        buy_supply_back->addChild(label_tips);
        
        auto content_back = Sprite::create("shopping/buy_supply_back_c.png");
        content_back->setPosition(Vec2(label_tips_pos.x,
                                       label_tips_pos.y - content_back->getContentSize().height * 0.75f
                                       ));
        buy_supply_back->addChild(content_back);
        
        auto content = Sprite::create("shopping/buy_supply_tips.png");
        content->setPosition(content_back->getContentSize() / 2);
        content_back->addChild(content);
        
        return true;
}


void BuySupply::menuSpendCoins(Ref*btn, int result){
        if (0 == result){
                this->removeFromParentAndCleanup(true);
                return;
        }
        
        auto cache = UserDefault::getInstance();
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, 0);
        int cur_mercenaries = cache->getIntegerForKey(USER_CURRENT_SUPPLY_NO, 0);
        
        if (cur_coins <= PRICE_PER_SUPPLEMENT){
                auto scene = Shopping::createScene();
                Director::getInstance()->pushScene(scene);
        }else{
                cur_coins -= PRICE_PER_SUPPLEMENT;
                cur_mercenaries += 1;
                cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
                cache->setIntegerForKey(USER_CURRENT_SUPPLY_NO, cur_mercenaries);
                cache->flush();
                this->removeFromParentAndCleanup(true);
        }
}
