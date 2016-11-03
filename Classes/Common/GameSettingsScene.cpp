//
//  GameSettings.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#include "GameSettingsScene.hpp"

#pragma makr - init scene
Scene* GameSettings::createScene()
{
        auto scene = Scene::create();
        auto layer = GameSettings::create();
        scene->addChild(layer);
        
        return scene;
}

bool GameSettings::init()
{
        if (!Layer::init()){
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto sprite = Sprite::create("loading_back.png");
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(sprite, 0);
        
        auto label = Label::createWithTTF("this is a game settings page.", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height / 2));
        label->setColor(Color3B::RED);
        this->addChild(label, 2);
        
        
        _okButton = MenuItemImage::create("start_game.png", "start_game_sel.png",
                                          CC_CALLBACK_1(GameSettings::menuOk, this));
        _okButton->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                    origin.y + 60));
        
        auto menu = Menu::create(_okButton, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
        
        return true;
}

void GameSettings::menuOk(Ref* pSender){
        Director::getInstance()->popScene();
}
