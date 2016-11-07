#include "LevelSelectScene.hpp"
#include "SimpleAudioEngine.h"
#include "AppMacros.hpp"
#include "StartingScene.hpp"
#include "GameHelpScene.hpp"

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
        
        
        _shareIt = MenuItemImage::create("share.png", "share_sel.png",
                                         CC_CALLBACK_1(Starting::menuShareGame, this));
        _shareIt->setPosition(Vec2(_shareIt->getContentSize().width,
                                     origin.y + visibleSize.height - _shareIt->getContentSize().height));
        
        auto menu = Menu::create(_startGame, _soundCtrl, _helpBtn, _shareIt, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 2);
        
        
        
        PluginFacebook::setListener(this);
        sdkbox::PluginFacebook::init();
        
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
        this->addChild(_loadingBar, 3);
        
        Size bar_size = _loadingBar->getContentSize();
        auto label = Label::createWithTTF("Loading", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(pos.x, pos.y + bar_size.height / 2));
        this->addChild(label, 2);
        
        scheduleUpdate();
}

void Starting::afterCaptureScreen(bool yes, const std::string &outputFilename)
{
        if (!outputFilename.empty() && FileUtils::getInstance()->isFileExist(outputFilename)){
                CCLOG("##FB afterCaptureScreen: %s", outputFilename.c_str());
                if (yes) {
                        
                        FBShareInfo info;
                        info.type  = FB_PHOTO;
                        info.title = "capture screen";
                        info.link = "http://www.cocos2d-x.org";
                        info.image = outputFilename;
                        PluginFacebook::dialog(info);
                }
        }
        
}

void Starting::menuShareGame(Ref* pSender){
        CCLOG("##FB %s", __FUNCTION__);
        
        std::vector<std::string> permation = PluginFacebook::getPermissionList();
        if (permation.size() == 0){
                std::vector<std::string> permissions;
                permissions.push_back(sdkbox::FB_PERM_READ_EMAIL);
                permissions.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
                permissions.push_back(sdkbox::FB_PERM_PUBLISH_POST);

                PluginFacebook::login(permissions);
        }else{
                utils::captureScreen(CC_CALLBACK_2(Starting::afterCaptureScreen, this), "screen.png");
                /*
                FBShareInfo info;
                info.type  = FB_LINK;
                info.link  = "http://www.cocos2d-x.org";
                info.title = "cocos2d-x";
                info.text  = "Best Game Engine";
                info.image = "http://cocos2d-x.org/images/logo.png";
                PluginFacebook::dialog(info);
                 */
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
                SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE);
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
        }
}


void Starting::update(float delta){
        
        if (_count < 100)
                _count += 10;
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


#pragma mark - facebook callback


/*********************
 * Facebook callbacks
 *********************/
void Starting::onLogin(bool isLogin, const std::string& error)
{
        CCLOG("##FB isLogin: %d, error: %s", isLogin, error.c_str());
}

void Starting::onAPI(const std::string& tag, const std::string& jsonData)
{
        CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
}

void Starting::onSharedSuccess(const std::string& message)
{
        CCLOG("##FB onSharedSuccess:%s", message.c_str());
}

void Starting::onSharedFailed(const std::string& message)
{
        CCLOG("##FB onSharedFailed:%s", message.c_str());
}

void Starting::onSharedCancel()
{
        CCLOG("##FB onSharedCancel");
}

void Starting::onPermission(bool isLogin, const std::string& error)
{
        CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str()); }

void Starting::onFetchFriends(bool ok, const std::string& msg)
{
        CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
        
        MenuItemFont::setFontSize(20);
        static Menu *menu = Menu::create();
        menu->setPositionY(20);
        menu->cleanup();
        
        const std::vector<sdkbox::FBGraphUser>& friends = PluginFacebook::getFriends();
        for (int i = 0; i < friends.size(); i++)
                {
                const sdkbox::FBGraphUser& user = friends.at(i);
                CCLOG("##FB> -------------------------------");
                CCLOG("##FB>> %s", user.uid.data());
                CCLOG("##FB>> %s", user.firstName.data());
                CCLOG("##FB>> %s", user.lastName.data());
                CCLOG("##FB>> %s", user.name.data());
                CCLOG("##FB>> %s", user.isInstalled ? "app is installed" : "app is not installed");
                CCLOG("##FB");
                
                MenuItemFont *item = MenuItemFont::create(user.name, [=](Ref*) {
                        sdkbox::FBAPIParam params;
                        params["redirect"] = "false";
                        params["type"] = "small";
                        std::string url(user.uid + "/picture");
                        PluginFacebook::api(url, "GET", params, "__fetch_picture_tag__");
                });
                menu->addChild(item);
                }
        if (!menu->getParent()) {
                menu->alignItemsHorizontally();
                addChild(menu);
        }
        
        MessageBox("", "fetch friends");
}

void Starting::onRequestInvitableFriends( const FBInvitableFriendsInfo& friends )
{
        CCLOG("Request Inviteable Friends Begin");
        for (auto it = friends.begin(); it != friends.end(); ++it) {
                CCLOG("Invitable friend: %s", it->getName().c_str());
        }
        CCLOG("Request Inviteable Friends End");
}

void Starting::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
        CCLOG("on invite friends with invite ids %s= '%s'", result?"ok":"error", msg.c_str());
}

void Starting::onInviteFriendsResult( bool result, const std::string& msg )
{
        CCLOG("on invite friends %s= '%s'", result?"ok":"error", msg.c_str());
}

void Starting::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
        CCLOG("Facebook id:'%s' name:'%s' last_name:'%s' first_name:'%s' email:'%s' installed:'%d'",
              userInfo.getUserId().c_str(),
              userInfo.getName().c_str(),
              userInfo.getFirstName().c_str(),
              userInfo.getLastName().c_str(),
              userInfo.getEmail().c_str(),
              userInfo.isInstalled ? 1 : 0
              );
}
