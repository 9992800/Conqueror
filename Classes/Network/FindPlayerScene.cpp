//
//  FindPlayerScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#include "FindPlayerScene.hpp"
#include "PopUpOkCancelDialog.hpp"

Scene* FindPlayer::createScene()
{
        auto scene = Scene::create();
        auto layer = FindPlayer::create();
        scene->addChild(layer);
        
        return scene;
}


FindPlayer::~FindPlayer(){
}


bool FindPlayer::init(){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        
        _waitingTips = Sprite::create("anim/grossinis_sister1.png");
        _waitingTips->setPosition(center);
        _waitingTips->setVisible(false);
        
        this->addChild(_waitingTips);
        
        auto map_1 = MenuItemImage::create("online_map1.png", "", CC_CALLBACK_1(FindPlayer::menuMapSelect, this, 1));
        Size map_1_size = map_1->getContentSize();
        Vec2 level_1_pos  = Vec2(map_1_size.width, visibleSize.height - map_1_size.height);
        map_1->setPosition(level_1_pos);
        auto bat_fee1 = Label::createWithSystemFont("参赛费用:100金币\r\n获胜奖励:500金币", "fonts/Marker Felt.ttf", 16);//
        bat_fee1->setPosition(Vec2(map_1_size/2));
        bat_fee1->setColor(Color3B::RED);
        map_1->addChild(bat_fee1);
                                                 
        auto map_2 = MenuItemImage::create("online_map2.png", "", CC_CALLBACK_1(FindPlayer::menuMapSelect, this, 2));
        Size map_2_size = map_2->getContentSize();
        Vec2 level_2_pos  = Vec2(level_1_pos.x  + 2 * map_2_size.width, level_1_pos.y);
        map_2->setPosition(level_2_pos);
        auto bat_fee2 = Label::createWithSystemFont("参赛费用:200金币\r\n获胜奖励:1000金币", "fonts/Marker Felt.ttf", 16);//
        bat_fee2->setColor(Color3B::RED);
        bat_fee2->setPosition(Vec2(map_2_size/2));
        map_2->addChild(bat_fee2);
        
        auto map_3 = MenuItemImage::create("online_map3.png", "", CC_CALLBACK_1(FindPlayer::menuMapSelect, this, 3));
        Size map_3_size = map_3->getContentSize();
        Vec2 level_3_pos  = Vec2(level_2_pos.x  + 2 * map_3_size.width, level_1_pos.y);
        map_3->setPosition(level_3_pos);
        auto bat_fee3 = Label::createWithSystemFont("参赛费用:300金币\r\n获胜奖励:1800金币", "fonts/Marker Felt.ttf", 16);//
        bat_fee3->setColor(Color3B::RED);
        bat_fee3->setPosition(Vec2(map_3_size/2));
        map_3->addChild(bat_fee3);
        
        auto map_4 = MenuItemImage::create("online_map4.png", "", CC_CALLBACK_1(FindPlayer::menuMapSelect, this, 4));
        Size map_4_size = map_4->getContentSize();
        Vec2 level_4_pos  = Vec2(map_4_size.width, visibleSize.height - 4 * map_4_size.height);
        map_4->setPosition(level_4_pos);
        auto bat_fee4 = Label::createWithSystemFont("参赛费用:400金币\r\n获胜奖励:2800金币", "fonts/Marker Felt.ttf", 16);//
        bat_fee4->setPosition(Vec2(map_4_size/2));
        bat_fee4->setColor(Color3B::RED);
        map_4->addChild(bat_fee4);
        
        
        auto map_5 = MenuItemImage::create("online_map5.png", "", CC_CALLBACK_1(FindPlayer::menuMapSelect, this, 5));
        Size map_5_size = map_5->getContentSize();
        Vec2 level_5_pos  = Vec2(level_4_pos.x  + 2 * map_5_size.width, level_4_pos.y);
        map_5->setPosition(level_5_pos);
        auto bat_fee5 = Label::createWithSystemFont("参赛费用:500金币\r\n获胜奖励:4000金币", "fonts/Marker Felt.ttf", 16);//
        bat_fee5->setColor(Color3B::RED);
        bat_fee5->setPosition(Vec2(map_5_size/2));
        map_5->addChild(bat_fee5);
        
        
        auto map_6 = MenuItemImage::create("online_map6.png", "", CC_CALLBACK_1(FindPlayer::menuMapSelect, this, 6));
        Size map_6_size = map_6->getContentSize();
        Vec2 level_6_pos  = Vec2(level_5_pos.x  + 2 * map_6_size.width, level_4_pos.y);
        map_6->setPosition(level_6_pos);
        auto bat_fee6 = Label::createWithSystemFont("参赛费用:600金币\r\n获胜奖励:5400金币", "fonts/Marker Felt.ttf", 16);//
        bat_fee6->setColor(Color3B::RED);
        bat_fee6->setPosition(Vec2(map_6_size/2));
        map_6->addChild(bat_fee6);
        
        auto menu = Menu::create(map_1, map_2, map_3, map_4,
                          map_5, map_6, NULL);

        menu->setPosition(Vec2::ZERO);
        this->addChild(menu);
        return true;
}

void FindPlayer::onMessage(std::string message){
        CCLOGWARN("---online---=%s", message.c_str());
}

void FindPlayer::afterAnimation(){
        
}

void FindPlayer::menuSearching(Ref* pSender){
}


void FindPlayer::menuMapSelect(Ref* menuItem, int mapIdx){
        _curMapSel = mapIdx;
        BaseDialogConfig config("经典联赛",
                                "胜场越多，奖励越多");
        PopUpOkCancelDialog *dialog = PopUpOkCancelDialog::create(config,
                                                                  CC_CALLBACK_1(FindPlayer::startGame, this, 1),
                                                                  CC_CALLBACK_1(FindPlayer::startGame, this, 0));
        this->addChild(dialog, 100);
}

void FindPlayer::startGame(Ref* btn, int result){
        if (result){
                
        }else{
                ((Node*)btn)->removeFromParentAndCleanup(true);
        }
}

void FindPlayer::onEnter(){
        Layer::onEnter();
        _waitingTips->setVisible(true);
        
        auto cache = AnimationCache::getInstance();
        cache->addAnimationsWithFile("anim/animations-2.plist");
        auto animation2 = cache->getAnimation("dance_1");
        animation2->setLoops(10);
        auto action2 = Animate::create(animation2);
        
        CallFunc* callback = CallFunc::create(std::bind(&FindPlayer::afterAnimation, this));
        
        Sequence*  s = Sequence::create(action2, callback, nullptr);
        _waitingTips->runAction(s);
}

void FindPlayer::onExit(){
        Layer::onExit();
        _waitingTips->stopAllActions();
}
