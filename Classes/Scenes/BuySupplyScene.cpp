//
//  BuySupplyScene.cpp
//  Conqueror
//
//  Created by wsli on 16/12/28.
//
//

#include "BuySupplyScene.hpp"
#include "ShoppingScene.hpp"

Scene* BuySupply::createScene(){
        auto scene = Scene::create();
        auto layer = BuySupply::create();
        scene->addChild(layer);
        return scene;
}

bool BuySupply::init(){
        
        if (!Layer::init()){
                return false;
        }
        _soundEngine     = CocosDenshion::SimpleAudioEngine::getInstance();
        _soundTotalOn    = UserDefault::getInstance()->getBoolForKey(SOUND_MUSIC_TOTAL_KEY, true);
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        
        auto back_ground = Sprite::create("shopping/shopping_back.png");
        auto back_ground_size = back_ground->getContentSize();
        back_ground->setPosition(visible_size / 2);
        this->addChild(back_ground);
        
        auto return_btn = cocos2d::ui::Button::create("DIALOG_CANCEL.png","DIALOG_CANCEL_SEL.png");
        return_btn->setPosition(Vec2(back_ground_size.width * 0.92, back_ground_size.height * 0.08));
        return_btn->addClickEventListener([this](Ref*){
                if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
                Director::getInstance()->popScene();
        });
        return_btn->setTitleText("Return");
        return_btn->setTitleFontName("fonts/arial.ttf");
        return_btn->setTitleFontSize(16);
        return_btn->setScale(1.4f);
        back_ground->addChild(return_btn);
        
        return true;
}


void BuySupply::menuSpendCoins(Ref*btn, int result){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        if (0 == result){
                this->removeFromParentAndCleanup(true);
                return;
        }
        
        auto cache = UserDefault::getInstance();
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, 0);
        int cur_mercenaries = cache->getIntegerForKey(USER_CURRENT_SUPPLY_NO, 0);
        
        if (cur_coins < PRICE_PER_SUPPLEMENT){
                auto scene = Shopping::createScene();
                Director::getInstance()->pushScene(scene);
        }else{
                cur_coins -= PRICE_PER_SUPPLEMENT;
                cur_mercenaries += 1;
                cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
                cache->setIntegerForKey(USER_CURRENT_SUPPLY_NO, cur_mercenaries);
                cache->flush();
                if (this->_curLisntener){
                        _curLisntener();
                }
                this->removeFromParentAndCleanup(true);
        }
}
