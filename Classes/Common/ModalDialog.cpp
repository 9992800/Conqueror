//
//  ModalDialog.cpp
//  Conqueror
//
//  Created by wsli on 16/11/18.
//
//

#include "ModalDialog.hpp"
#include "AppMacros.hpp"

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
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        auto frame = frameCache->getSpriteFrameByName("loding0001.png");
        _waitingTips = Sprite::create();
        _waitingTips->setPosition(center);
        _waitingTips->setSpriteFrame(frame);
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
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this->getParent(), true);
        this->removeFromParent();
}

void ModalLayer::onEnter(){
        Layer::onEnter();
        auto cache = AnimationCache::getInstance();
        auto waiting_anim = cache->getAnimation("common_waiting")->clone();
        waiting_anim->setLoops(120);
        waiting_anim->setRestoreOriginalFrame(true);
        
        CallFunc* callback = CallFunc::create(std::bind(&ModalLayer::afterAnimation, this));
        Sequence*  s = Sequence::create(Animate::create(waiting_anim) , callback, nullptr);
        _waitingTips->runAction(s);
}

void ModalLayer::onExit(){
        Layer::onExit();
        _waitingTips->stopAllActions();
}
void ModalLayer::showModalDialog(Node* parent){
        if (!parent)
                return;
        
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(parent, true);
        auto tips = ModalLayer::create();
        parent->addChild(tips, SUPER_LAYER_PRIVILIEGE, MODAL_DIALOG_NODETAG);
}
void ModalLayer::dismissDialog(Node* parent){
        auto tips = parent->getChildByTag(MODAL_DIALOG_NODETAG);
        if (!tips) return;
        
        tips->removeFromParent();
        
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(parent, true);
}

