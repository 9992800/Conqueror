#include "StartingScene.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"

USING_NS_CC;

#pragma makr - init scene
Scene* Starting::createScene()
{
        auto scene = Scene::create();
        auto layer = Starting::create();
        scene->addChild(layer);
        
        return scene;
}

bool Starting::init()
{
        if (!Layer::init()){
                return false;
        }
    
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto sprite = Sprite::create("StartingLogo.png");
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(sprite, 0);
        
        _count = 0;
        _loadingBar = LoadingBar::create("sliderProgress.png");
        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
        _loadingBar->setPercent(0);
        
        Vec2 pos = Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 6);
        _loadingBar->setPosition(pos);
        this->addChild(_loadingBar, 1);
        
        Size bar_size = _loadingBar->getContentSize();
        auto label = Label::createWithTTF("Loading", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(pos.x, pos.y + bar_size.height / 2));
        this->addChild(label, 2);
        
       
    
        return true;
}


#pragma makr - refresh loading bars

void Starting::onEnter(){
        Layer::onEnter();
        
        scheduleUpdate();
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE);
        _count += 10;
}

void Starting::update(float delta){
        
        if (_count < 100)
                _count++;
        else{
                _count = 0;
        }
        
        _loadingBar->setPercent(_count);
}

void Starting::onExit(){
        Layer::onExit();
        unscheduleUpdate();
}
