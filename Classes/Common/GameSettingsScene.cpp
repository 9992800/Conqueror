//
//  GameSettings.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#include "GameSettingsScene.hpp"

#pragma makr - init scene
bool GameSettings::init()
{
        if (!LayerColor::initWithColor(Color4B::BLACK)){
                return false;
        }
        this->setOpacity(180);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto setting_back = Sprite::create("settings/system_setting_back.png");
        setting_back->setPosition(visibleSize / 2);
        this->addChild(setting_back);
        auto setting_back_size = setting_back->getContentSize();

        
        auto exit_btn = ui::Button::create("DIALOG2_OK.png", "DIALOG2_OK_SEL.png");
        exit_btn->setPosition(Vec2(setting_back_size.width * 0.5f,
                                   exit_btn->getContentSize().height * 0.7f));
        
        exit_btn->addClickEventListener([this](Ref*){
                this->removeFromParentAndCleanup(true);
        });
        
        exit_btn->setTitleText("OK");
        exit_btn->setTitleFontName("fonts/arial.ttf");
        exit_btn->setTitleFontSize(44);
        setting_back->addChild(exit_btn);
        
        auto scene_title = ui::ImageView::create("settings/scene_title.png");
        scene_title->setPosition(Vec2(setting_back_size.width * 0.5,
                                 setting_back_size.height -
                                      scene_title->getContentSize().height * 0.5f));
        setting_back->addChild(scene_title);
        auto title_pos = scene_title->getPosition();
        
        auto effect_size_back = Sprite::create("settings/size_back.png");
        auto volumn_size = effect_size_back->getContentSize();
        effect_size_back->setPosition(setting_back_size.width / 2 ,
                                      title_pos.y - 2 * volumn_size.height);
        setting_back->addChild(effect_size_back);
        
        auto effect_title = Label::createWithSystemFont("Sound Effect", "fonts/arial.ttf", 32);
        effect_title->setPosition(Vec2(setting_back_size.width / 2,
                                       title_pos.y - volumn_size.height));
        setting_back->addChild(effect_title);
        
        return true;
}
