//
//  GameSettings.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#include "GameSettingsScene.hpp"

#pragma makr - init scene
#define sound_effect_interv (100 / 16)
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

        //Exit button
        auto exit_btn = ui::Button::create("DIALOG2_OK.png", "DIALOG2_OK_SEL.png");
        exit_btn->setPosition(Vec2(setting_back_size.width * 0.5f,
                                   exit_btn->getContentSize().height * 0.7f));
        
        exit_btn->addClickEventListener(CC_CALLBACK_1(GameSettings::menuExit, this));
        
        exit_btn->setTitleText("OK");
        exit_btn->setTitleFontName("fonts/arial.ttf");
        exit_btn->setTitleFontSize(44);
        setting_back->addChild(exit_btn);
        
        //Titile
        auto scene_title = ui::ImageView::create("settings/scene_title.png");
        scene_title->setPosition(Vec2(setting_back_size.width * 0.5,
                                 setting_back_size.height -
                                      scene_title->getContentSize().height * 0.5f));
        setting_back->addChild(scene_title);
        auto title_pos = scene_title->getPosition();
        
        
        //sound effect
        auto effect_size_back = Sprite::create("settings/size_back.png");
        auto volumn_size = effect_size_back->getContentSize();
        effect_size_back->setPosition(setting_back_size.width / 2 ,
                                      title_pos.y - 2 * volumn_size.height);
        setting_back->addChild(effect_size_back, 1);
        
        auto effect_title = Label::createWithSystemFont("Sound Effect", "fonts/arial.ttf", 28);
        effect_title->setPosition(Vec2(setting_back_size.width / 2,
                                       title_pos.y - volumn_size.height));
        setting_back->addChild(effect_title);
        effect_title->setColor(Color3B::BLACK);
        
        auto cache = UserDefault::getInstance();
        
        _soundEffectV = cache->getIntegerForKey(SOUND_EFFECT_VALUE_KEY, sound_effect_interv * 2);
        _soundEffectB = cache->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        _soundEffect = ui::LoadingBar::create("settings/size_bar.png");
        _soundEffect->setDirection(ui::LoadingBar::Direction::LEFT);
        _soundEffect->setPercent(_soundEffectV);
        _soundEffect->setPosition(effect_size_back->getPosition());
        setting_back->addChild(_soundEffect, 2);
        
        auto sound_minus_btn = ui::Button::create("settings/minus_btn.png",
                                                  "settings/minus_btn_sel.png");
        sound_minus_btn->setPosition(Vec2(sound_minus_btn->getContentSize().width * 0.8f,
                                     _soundEffect->getPosition().y));
        setting_back->addChild(sound_minus_btn);
        sound_minus_btn->addClickEventListener(CC_CALLBACK_1(GameSettings::menuSoundEffect, this, 0));
        
        
        auto sound_add_btn = ui::Button::create("settings/add_btn.png",
                                                  "settings/add_btn_sel.png");
        sound_add_btn->setPosition(Vec2(setting_back_size.width - sound_add_btn->getContentSize().width * 0.8f,
                                          _soundEffect->getPosition().y));
        setting_back->addChild(sound_add_btn);
        sound_add_btn->addClickEventListener(CC_CALLBACK_1(GameSettings::menuSoundEffect, this, 1));
        
        //music back
        auto music_back = Sprite::create("settings/size_back.png");
        auto music_size = music_back->getContentSize();
        music_back->setPosition(setting_back_size.width / 2 ,
                                      effect_size_back->getPosition().y - 2 * music_size.height);
        setting_back->addChild(music_back, 1);
        
        auto music_title = Label::createWithSystemFont("Back Music", "fonts/arial.ttf", 28);
        music_title->setPosition(Vec2(setting_back_size.width / 2,
                                       effect_size_back->getPosition().y - music_size.height));
        setting_back->addChild(music_title);
        music_title->setColor(Color3B::BLACK);
        
        _backMusicV = cache->getIntegerForKey(BACK_MUSIC_VALUE_KEY, sound_effect_interv * 2);
        _backMusicB = cache->getBoolForKey(BACK_MUSIC_SWITCH_KEY, true);
        _backMusic  = ui::LoadingBar::create("settings/size_bar.png");
        _backMusic->setDirection(ui::LoadingBar::Direction::LEFT);
        _backMusic->setPercent(_backMusicV);
        _backMusic->setPosition(music_back->getPosition());
        setting_back->addChild(_backMusic, 2);
        
        
        auto music_minus_btn = ui::Button::create("settings/minus_btn.png",
                                                  "settings/minus_btn_sel.png");
        music_minus_btn->setPosition(Vec2(music_minus_btn->getContentSize().width * 0.8f,
                                          _backMusic->getPosition().y));
        setting_back->addChild(music_minus_btn);
        music_minus_btn->addClickEventListener(CC_CALLBACK_1(GameSettings::menuBackMusic, this, 0));
        
        
        auto music_add_btn = ui::Button::create("settings/add_btn.png",
                                                "settings/add_btn_sel.png");
        music_add_btn->setPosition(Vec2(setting_back_size.width - music_add_btn->getContentSize().width * 0.8f,
                                        _backMusic->getPosition().y));
        setting_back->addChild(music_add_btn);
        music_add_btn->addClickEventListener(CC_CALLBACK_1(GameSettings::menuBackMusic, this, 1));
        
        
        //animation settings.
        auto anim_back = Sprite::create("settings/anim_set_back.png");
        auto anim_back_size = anim_back->getContentSize();
        anim_back->setPosition(Vec2(setting_back_size.width * 0.5f, music_back->getPosition().y - music_size.height - anim_back_size.height * 0.5f));
        setting_back->addChild(anim_back);
        
        auto anim_switch = ui::Button::create("maps/open_anim.png",
                                             "maps/open_anim_sel.png");
        auto anim_btn_size = anim_switch->getContentSize();
        anim_switch->setPosition(Vec2(anim_btn_size.width * 0.8f,
                                     anim_back_size.height * 0.5f));
        anim_back->addChild(anim_switch);
        anim_switch->addClickEventListener(CC_CALLBACK_1(GameSettings::menuAnimSwitch, this));
        
        int game_speed = UserDefault::getInstance()->getIntegerForKey(GAME_SPEED_KEY, 1);
        
        _speedBtn2 = ui::Button::create("settings/game_speed_2d.png");
        auto speed_btn_size = _speedBtn2->getContentSize();
        auto middle_btn_pos = Vec2(0.5f * (anim_back_size.width + anim_btn_size.width), anim_switch->getPosition().y);
        _speedBtn2->setPosition(middle_btn_pos);
        anim_back->addChild(_speedBtn2);
        _speedBtn2->addClickEventListener(CC_CALLBACK_1(GameSettings::menuSpeedChange, this, 2));
        
        
        
        _speedBtn1 = ui::Button::create("settings/game_speed_1d.png");
        _speedBtn1->setPosition(Vec2(middle_btn_pos.x - speed_btn_size.width,
                                middle_btn_pos.y));
        anim_back->addChild(_speedBtn1);
        _speedBtn1->addClickEventListener(CC_CALLBACK_1(GameSettings::menuSpeedChange, this, 1));
        
        _speedBtn3 = ui::Button::create("settings/game_speed_3d.png");
        _speedBtn3->setPosition(Vec2(middle_btn_pos.x + speed_btn_size.width,
                                     middle_btn_pos.y));
        anim_back->addChild(_speedBtn3);
        _speedBtn3->addClickEventListener(CC_CALLBACK_1(GameSettings::menuSpeedChange, this, 3));
        
        
        if (game_speed == 1){
                _speedBtn1->loadTextureNormal("settings/game_speed_1.png");
        }else if (game_speed == 2){
                _speedBtn2->loadTextureNormal("settings/game_speed_2.png");
        }else if (game_speed == 3){
                _speedBtn3->loadTextureNormal("settings/game_speed_3.png");
        }
        
        return true;
}

void GameSettings::menuExit(Ref*){
        this->removeFromParentAndCleanup(true);
        auto cache = UserDefault::getInstance();
        
        cache->setBoolForKey(SOUND_EFFECT_SWITCH_KEY, _soundEffectB);
        cache->setIntegerForKey(SOUND_EFFECT_VALUE_KEY, _soundEffectV);
        
        cache->setBoolForKey(BACK_MUSIC_SWITCH_KEY, _backMusicB);
        cache->setIntegerForKey(BACK_MUSIC_VALUE_KEY, _backMusicV);
        cache->flush();
}


void  GameSettings::menuSoundEffect(Ref* btn, int action){
        if (0 == action){
                _soundEffectV -= sound_effect_interv;
                if (_soundEffectV <= 0){
                        _soundEffectV = 0;
                        _soundEffectB = false;
                        CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
                }
        }else if (1 == action){
                _soundEffectV += sound_effect_interv;
                if (_soundEffectV >= 100){
                        _soundEffectV = 100;
                }
                _soundEffectB = true;
                CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
        }
        
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(_soundEffectV * 0.01f);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
        _soundEffect->setPercent(_soundEffectV);
}


void GameSettings::menuBackMusic(Ref*, int action){
        if (0 == action){
                _backMusicV -= sound_effect_interv;
                if (_backMusicV <= 0){
                        _backMusicV = 0;
                        _backMusicB = false;
                        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
                }
        }else if (1 == action){
                _backMusicV += sound_effect_interv;
                if (_backMusicV >= 100){
                        _backMusicV = 100;
                }
                _backMusicB = true;
                bool is_playing = CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
                if (!is_playing){
                        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
                }
        }
        
         CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(_backMusicV * 0.01f);
        _backMusic->setPercent(_backMusicV);
}

void GameSettings::menuAnimSwitch(Ref* btn){
        auto cache =  UserDefault::getInstance();
        ui::Button* anim_btn = (ui::Button*)btn;
        bool anim_ison = cache->getBoolForKey(ANIMATION_SWITCH_KEY, true);
        anim_ison = !anim_ison;
        if (anim_ison){
                cache->setBoolForKey(ANIMATION_SWITCH_KEY, true);
                anim_btn->loadTextureNormal("maps/open_anim.png");
        }else{
                cache->setBoolForKey(ANIMATION_SWITCH_KEY, false);
                anim_btn->loadTextureNormal("maps/close_anim.png");
        }
        cache->flush();
}


void GameSettings::menuSpeedChange(Ref* btn, int game_speed){
        
        _speedBtn1->loadTextureNormal("settings/game_speed_1d.png");
        _speedBtn2->loadTextureNormal("settings/game_speed_2d.png");
        _speedBtn3->loadTextureNormal("settings/game_speed_3d.png");
        
        if (game_speed == 1){
                _speedBtn1->loadTextureNormal("settings/game_speed_1.png");
        }else if (game_speed == 2){
                _speedBtn2->loadTextureNormal("settings/game_speed_2.png");
        }else if (game_speed == 3){
                _speedBtn3->loadTextureNormal("settings/game_speed_3.png");
        }
        
        auto cache =  UserDefault::getInstance();
        cache->setIntegerForKey(GAME_SPEED_KEY, game_speed);
        cache->flush();
        Director::getInstance()->getScheduler()->setTimeScale(game_speed);
}

