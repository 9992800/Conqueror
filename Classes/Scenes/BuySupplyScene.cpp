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
        auto exit_btn = ui::Button::create("DIALOG2_OK.png", "DIALOG2_OK_SEL.png");
        exit_btn->setPosition(Vec2(buy_supply_back_size.width * 0.5f,
                                   exit_btn->getContentSize().height * 0.7f));
        
        exit_btn->addClickEventListener(CC_CALLBACK_1(BuySupply::menuExit, this));
        
        exit_btn->setTitleText("OK");
        exit_btn->setTitleFontName("fonts/arial.ttf");
        exit_btn->setTitleFontSize(44);
        buy_supply_back->addChild(exit_btn);
        auto exit_btn_size = exit_btn->getContentSize();
        
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


void BuySupply::menuExit(Ref*){
        
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
