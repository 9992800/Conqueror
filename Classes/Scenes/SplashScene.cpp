//
//  SplashScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//

#include "SplashScene.hpp"
#include "StartingScene.hpp"
#include "SimpleAudioEngine.h"
#include "AppMacros.hpp"
#include "GamePayUtil.hpp"
#include "PopUpOkCancelDialog.hpp"
#include "LevelSelectScene.hpp"

#pragma makr - init scene
Scene* Splash::createScene()
{
        auto scene = Scene::create();
        auto layer = Splash::create();
        scene->addChild(layer);
        
        return scene;
}

bool Splash::init()
{
        if (!Layer::init()){
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto sprite = Sprite::create("loading_back.png");
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(sprite, 0);
        
        _count = 0;
        _loadingBar = LoadingBar::create("sliderProgress.png");
        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
        _loadingBar->setPercent(0);
        
        Vec2 pos = Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 6);
        _loadingBar->setPosition(pos);
        this->addChild(_loadingBar, 2);
        
        _loadingBarBack = Sprite::create("sliderProgress_back.png");
        _loadingBarBack->setPosition(pos);
        this->addChild(_loadingBarBack, 1);
        
        Size bar_size = _loadingBar->getContentSize();
        auto label = Label::createWithTTF("Loading", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(pos.x, pos.y + bar_size.height / 2));
        this->addChild(label, 3);
        
        return true;
}

void Splash::gameOver(Ref* btn, int result){
        
}


#pragma makr - refresh loading bars

void Splash::onEnter(){
        Layer::onEnter();
        
        scheduleUpdate();
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE_START_GAME);
        _count += 1;
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE_WIN);
        _count += 1;
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE_DEFEAT);
        _count += 1;
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE_SELECTED);
        _count += 1;

        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE_DROP_DICE);
        _count += 1;
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE_SUPPLY);
        _count += 1;
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE_FINISH_LOSE);
        _count += 1;
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE_FINISH_WIN);
        _count += 1;
        
        bool is_effect_on = UserDefault::getInstance()->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        if (is_effect_on){
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.f);
        }else{
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.f);
        }
        
        _count += 1;
}

void Splash::update(float delta){
        
        if (_count < 100)
                _count += 1 ;
        else{
//                auto starting = Starting::createScene();
//                Director::getInstance()->replaceScene(starting);
                
                auto main = LevelSelect::createScene();
                Director::getInstance()->replaceScene(main);
        }
        
        _loadingBar->setPercent(_count);
}

void Splash::onExit(){
        Layer::onExit();
        unscheduleUpdate();
}
