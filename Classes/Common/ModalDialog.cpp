//
//  ModalDialog.cpp
//  Conqueror
//
//  Created by wsli on 16/11/18.
//
//

#include "ModalDialog.hpp"
enum{
        kTagBackGrd = 1,
        kTagRing
};
bool ModalLayer::init(){
        
        if ( !LayerColor::init()) {
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        this->setContentSize(visibleSize);
        this->setPosition(center);
        this->setIgnoreAnchorPointForPosition(false);
        this->setAnchorPoint(Vec2(0.5f, 0.5f));
        
        auto _backGround = cocos2d::ui::Scale9Sprite::create("Modal_DIALOG_BG.png");
        Size  bg_size(visibleSize.width / 2, visibleSize.height / 2);
        _backGround->setContentSize(bg_size);
        _backGround->setCapInsets(Rect(8, 8, 32, 32));
        _backGround->setPosition(Vec2(visibleSize / 2));
        this->addChild(_backGround, 0, kTagBackGrd);
        
        _waitingTips = Sprite::create("waiting_ring.png");
        _waitingTips->setPosition(center);
        this->addChild(_waitingTips);
        
        return true;
}

void ModalLayer::onTouchesMoved(Touch*, Event* event){
        return;
}
bool ModalLayer::onTouchesBegan(Touch*, Event *event){
        return false;
}


void ModalLayer::afterAnimation(){
        this->removeFromParent();
}

void ModalLayer::onEnter(){
        Layer::onEnter();
        CallFunc* callback = CallFunc::create(std::bind(&ModalLayer::afterAnimation, this));
        Sequence*  s = Sequence::create(RotateBy::create(4, 360*4), callback, nullptr);
        _waitingTips->runAction(s);
}

void ModalLayer::onExit(){
        Layer::onExit();
        _waitingTips->stopAllActions();
}

#define PRIVILIEGE 10000
#define NODETAG 10000
void ModalLayer::showModalDialog(Node* parent){
        if (!parent) return;
        
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(parent, true);
        auto tips = ModalLayer::create();
        parent->addChild(tips, PRIVILIEGE, NODETAG);
}
void ModalLayer::dismissDialog(Node* parent){
        auto tips = parent->getChildByTag(NODETAG);
        if (!tips) return;
        
        tips->removeFromParent();
        
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(parent, true);
}

