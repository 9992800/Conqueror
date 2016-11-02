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
        
        
        _loadingBar = LoadingBar::create("sliderProgress.png");
        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
        _loadingBar->setPercent(0);
        this->addChild(_loadingBar, 1);
        
        auto label = Label::createWithTTF("Loading", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));
        this->addChild(label, 2);
    
    return true;
}


#pragma makr - refresh loading bars
void Starting::onEnter(){
        _count = 0;
        
        scheduleUpdate();
        
        loadingResource();
}

void Starting::update(float delta){
        
        
        _loadingBar->setPercent(_count);
}

void Starting::onExit(){
        unscheduleUpdate();
}

#pragma makr - loading resources
void Starting::loadingResource(){
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE);
        _count = 50;
}
