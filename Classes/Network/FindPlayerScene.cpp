//
//  FindPlayerScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#include "FindPlayerScene.hpp"
Scene* FindPlayer::createScene()
{
        auto scene = Scene::create();
        auto layer = FindPlayer::create();
        scene->addChild(layer);
        
        return scene;
}

FindPlayer::~FindPlayer(){
        _network->release();
}

bool FindPlayer::init(){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        
        _waitingTips = Sprite::create("anim/grossinis_sister1.png");
        _waitingTips->setPosition(center);
        _waitingTips->setVisible(false);
        
        this->addChild(_waitingTips);
        
        wsCallBack callback = CC_CALLBACK_1(FindPlayer::onMessage, this);
        _network = WebSocktUtil::create(callback);
        _network->retain();
        
        _network->startConnect();
        
        return true;
}

void FindPlayer::onMessage(std::string message){
        CCLOGWARN("---online---=%s", message.c_str());
}

void FindPlayer::afterAnimation(){
        
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
