#include "LevelSelectScene.hpp"
#include "SimpleAudioEngine.h"
#include "AppMacros.hpp"
#include "StartingScene.hpp"
#include "GameHelpScene.hpp"
#include "FindPlayerScene.hpp"
#include "UserSessionBean.hpp"

using namespace CocosDenshion;

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
        
        auto back_ground = Sprite::create("starting_back.png");
        back_ground->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(back_ground, 0);
        
        auto logo = Sprite::create("logo.png");
        logo->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(logo, 1);
        
        _helpBtn = MenuItemImage::create("help.png", "help_sel.png",
                                           CC_CALLBACK_1(Starting::menuHelp, this));
        _helpBtn->setPosition(Vec2(origin.x + visibleSize.width - 2 * _helpBtn->getContentSize().width,
                                     origin.y + visibleSize.height - _helpBtn->getContentSize().height));
        
        
        bool is_effect_on = UserDefault::getInstance()->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        
        if (is_effect_on){
                _soundCtrl = MenuItemImage::create("Sound_on.png", "Sound_on_sel.png",
                                           CC_CALLBACK_1(Starting::menuSoundCtrl, this));
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
        }else{
                _soundCtrl = MenuItemImage::create("Sound_off.png", "Sound_off_sel.png",
                                                   CC_CALLBACK_1(Starting::menuSoundCtrl, this));
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
        }
        _soundCtrl->setPosition(Vec2(2 * _soundCtrl->getContentSize().width,
                                     origin.y +  60));
        
        _startGame = MenuItemImage::create("start_game.png", "start_game_sel.png",
                                           CC_CALLBACK_1(Starting::menuStartGame, this));
        _startGame->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                     origin.y + 60));
        
        
        auto share_it = MenuItemImage::create("share.png", "share_sel.png",
                                         CC_CALLBACK_1(Starting::menuShareGame, this));
        share_it->setPosition(Vec2(share_it->getContentSize().width,
                                     origin.y + visibleSize.height - share_it->getContentSize().height));
        
        _onlineBattle = MenuItemImage::create("online_battle.png", "online_battle_sel.png",
                                              CC_CALLBACK_1(Starting::menuOnlineBattle, this));
        _onlineBattle->setPosition(Vec2(origin.x + visibleSize.width - 2 * _onlineBattle->getContentSize().width,
                                        origin.y +  _onlineBattle->getContentSize().height/ 2 + 20));
        
        auto menu = Menu::create(_startGame, _soundCtrl, _helpBtn, share_it, _onlineBattle, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 2);
        
        return true;
}

void Starting::menuStartGame(Ref* pSender){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        _count = 0;
        _loadingBar = LoadingBar::create("sliderProgress.png");
        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
        _loadingBar->setPercent(0);
        
        Vec2 pos = Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 6);
        _loadingBar->setPosition(pos);
        this->addChild(_loadingBar, 4);
        
        
        Size bar_size = _loadingBar->getContentSize();
        auto label = Label::createWithTTF("Loading", "fonts/arial.ttf", 24);
        label->setPosition(Vec2(pos.x, pos.y + bar_size.height / 2));
        this->addChild(label, 5);
        
        auto loading_back = Sprite::create("sliderProgress_back.png");
        loading_back->setPosition(pos);
        this->addChild(loading_back, 3);
        
        scheduleUpdate();
}

void Starting::afterCaptureScreen(bool yes, const std::string &outputFilename)
{
        if (!outputFilename.empty() && FileUtils::getInstance()->isFileExist(outputFilename)){
                CCLOG("##FB afterCaptureScreen: %s", outputFilename.c_str());
                if (yes) {
                        
                        sdkbox::FBShareInfo info;
                        info.type  = sdkbox::FB_PHOTO;
                        info.title = "capture screen";
                        info.link = "http://www.cocos2d-x.org";
                        info.image = outputFilename;
                        sdkbox::PluginFacebook::dialog(info);
                }else{
                        sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
                }
        }
        
}

void Starting::menuShareGame(Ref* pSender){
        CCLOG("##FB %s", __FUNCTION__);
        
        if (sdkbox::PluginFacebook::isLoggedIn()){
                utils::captureScreen(CC_CALLBACK_2(Starting::afterCaptureScreen, this), "screen.png");
        }else{
                UserSessionBean::getInstance()->initSession();
        }
}

void Starting::menuHelp(Ref* pSender){
        Scene* scene = GameHelp::createScene();
        Director::getInstance()->pushScene(scene);
}

void Starting::menuSoundCtrl(Ref* pSender){
        bool is_effect_on = UserDefault::getInstance()->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        UserDefault::getInstance()->setBoolForKey(SOUND_EFFECT_SWITCH_KEY, !is_effect_on);
        
        
        if (is_effect_on){
                _soundCtrl->setNormalImage(Sprite::create("Sound_off.png"));
                _soundCtrl->setSelectedImage(Sprite::create("Sound_off_sel.png"));
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
        }else{
                _soundCtrl->setNormalImage(Sprite::create("Sound_on.png"));
                _soundCtrl->setSelectedImage(Sprite::create("Sound_on_sel.png"));
                SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
        }
}

void Starting::menuOnlineBattle(Ref* pSender){
//        MessageBox("title", "fetch friends");
               
       Scene* scene = FindPlayer::createScene();
       Director::getInstance()->pushScene(scene);
}


void Starting::update(float delta){
        
        if (_count < 100)
                _count += 1;
        else{
                auto level = LevelSelect::createScene();
                Director::getInstance()->pushScene(level);
        }
        
        _loadingBar->setPercent(_count);
}

void Starting::onExit(){
        Layer::onExit();
        unscheduleUpdate();
}
