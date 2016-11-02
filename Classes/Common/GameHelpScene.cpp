//
//  GameHelpScene.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/2.
//
//

#include "GameHelpScene.hpp"

#pragma makr - init scene
Scene* GameHelp::createScene()
{
        auto scene = Scene::create();
        auto layer = GameHelp::create();
        scene->addChild(layer);
        
        return scene;
}

bool GameHelp::init()
{
        if (!Layer::init()){
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto sprite = Sprite::create("loading_back.png");
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(sprite, 0);
        
        auto label = Label::createWithTTF("this is a help page.", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height / 2));
        this->addChild(label, 2);
        
        
        _okButton = MenuItemImage::create("start_game.png", "start_game_sel.png",
                                           CC_CALLBACK_1(GameHelp::menuOk, this));
        _okButton->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                     origin.y + 60));
        
        auto menu = Menu::create(_okButton, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
        
        return true;
}

void GameHelp::menuOk(Ref* pSender){
        Director::getInstance()->popScene();
}


