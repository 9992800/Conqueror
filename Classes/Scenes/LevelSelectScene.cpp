//
//  LevelSelecteScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//
#include "SimpleAudioEngine.h"
#include "GameSettingsScene.hpp"
#include "LevelSelectScene.hpp"
#include "GameScene.hpp"

enum {
        GAME_LEVEL_INDEX_1 = 1,
        GAME_LEVEL_INDEX_2,
        GAME_LEVEL_INDEX_3,
        GAME_LEVEL_INDEX_4,
        GAME_LEVEL_INDEX_5,
        GAME_LEVEL_INDEX_6,
        GAME_LEVEL_INDEX_7,
};

enum{
        kLevelShowLevel1Tag = 2,
        kLevelShowLevel2Tag = 3,
        kLevelShowLevel3Tag = 4,
        kLevelShowLevel4Tag = 5,
        kLevelShowLevel5Tag = 6,
        kLevelShowLevel6Tag = 7,
        kLevelShowLevel7Tag = 8,
        key_loading_bar1,
        key_loading_bar2,
        kLevelBuyMenuTag,
        kLevel4BuyBtnTag,
        kLevel5BuyBtnTag,
        kLevel6BuyBtnTag,
        kLevel7BuyBtnTag
};

enum{
        ZORDER_BACK_GROUND = 0,
        ZORDER_BACK_LAYERS,
        ZORDER_SEL_BACK_LAYER,
        ZORDER_ITEM_SHOW,
        ZORDER_ITEM_CONTROL
};

Scene* LevelSelect::createScene()
{
        auto scene = Scene::create();
        auto layer = LevelSelect::create();
        scene->addChild(layer);
        
        return scene;
}

bool LevelSelect::init()
{
        if (!Layer::init()){
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto back_ground = Sprite::create("level_select_back.png");
        back_ground->setPosition(center);
        this->addChild(back_ground, ZORDER_BACK_GROUND);
        
        
        this->loadLevelShow(center, visibleSize);
        
        this->loadLevelSelectedBackGround();
        
        this->initActionListener();
        
        this->initButtons(origin, visibleSize);
        
        IAP::setDebug(true);
        IAP::setListener(this);
        IAP::init();        
        
        return true;
}
void LevelSelect::onEnter(){
        Layer::onEnter();
        IAP::refresh();
}

void LevelSelect::loadLevelShow(Vec2 center, Size visibleSize){
       
        
        _levelShowBackGround = LayerColor::create(Color4B::ORANGE);
        _levelShowBackGround->setIgnoreAnchorPointForPosition(false);
        _levelShowBackGround->setAnchorPoint(Vec2(0.5, 0.5));
        
        auto leve1_1 = Sprite::create("level/2player.png");
        Size leve1_1_size = leve1_1->getContentSize();
        float left_tap = leve1_1_size.width;
        
        Size back_size = Size(left_tap * 8 + leve1_1_size.width * 7, visibleSize.height / 2);
        _levelShowBackGround->setContentSize(back_size);
        _mostRight = Vec2(center.x + (back_size.width - visibleSize.width) / 2 ,center.y);
        _mostLeft  = Vec2(center.x - (back_size.width - visibleSize.width) / 2 ,center.y);
        _levelShowBackGround->setPosition(_mostRight);
        
        float c = back_size.height / 2;
        
        Vec2 level_1_pos  = Vec2(left_tap + leve1_1_size.width / 2, c);
        leve1_1->setPosition(level_1_pos);
        _levelShowBackGround->addChild(leve1_1, ZORDER_ITEM_SHOW, kLevelShowLevel1Tag);
        
        auto leve1_2 = Sprite::create("level/3player.png");
        Size level_2_size = leve1_2->getContentSize();
        Vec2 level_2_pos  = Vec2(level_1_pos.x + left_tap + level_2_size.width, c);
        leve1_2->setPosition(level_2_pos);
        _levelShowBackGround->addChild(leve1_2, ZORDER_ITEM_SHOW, kLevelShowLevel2Tag);
        
        auto level_3 = Sprite::create("level/4player.png");
        Size level_3_size = level_3->getContentSize();
        Vec2 level_3_pos  = Vec2(level_2_pos.x + left_tap + level_3_size.width, c);
        level_3->setPosition(level_3_pos);
        _levelShowBackGround->addChild(level_3, ZORDER_ITEM_SHOW, kLevelShowLevel3Tag);
        
        auto level_4 = Sprite::create("level/5player.png");
        Size level_4_size = level_4->getContentSize();
        Vec2 level_4_pos  = Vec2(level_3_pos.x + left_tap + level_4_size.width, c);
        level_4->setPosition(level_4_pos);
        _levelShowBackGround->addChild(level_4, ZORDER_ITEM_SHOW, kLevelShowLevel4Tag);
        
        auto level_5 = Sprite::create("level/6player.png");
        Size level_5_size = level_5->getContentSize();
        Vec2 level_5_pos  = Vec2(level_4_pos.x + left_tap + level_5_size.width, c);
        level_5->setPosition(level_5_pos);
        _levelShowBackGround->addChild(level_5, ZORDER_ITEM_SHOW, kLevelShowLevel5Tag);
        
        auto level_6 = Sprite::create("level/7player.png");
        Size level_6_size = level_6->getContentSize();
        Vec2 level_6_pos  = Vec2(level_5_pos.x + left_tap + level_6_size.width, c);
        level_6->setPosition(level_6_pos);
        _levelShowBackGround->addChild(level_6, ZORDER_ITEM_SHOW, kLevelShowLevel6Tag);
        
        auto level_7 = Sprite::create("level/8player.png");
        Size level_7_size = level_7->getContentSize();
        Vec2 level_7_pos  = Vec2(level_6_pos.x + left_tap + level_7_size.width, c);
        level_7->setPosition(level_7_pos);
        _levelShowBackGround->addChild(level_7, ZORDER_ITEM_SHOW, kLevelShowLevel7Tag);
        
        
        this->addChild(_levelShowBackGround, ZORDER_BACK_LAYERS);
        
        //TODO::load receipents from apple store
        
        float btn_top_gap = 30.f;
        auto level_1_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                           CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_1));
        level_1_lock_btn->setPosition(Vec2(level_1_pos.x, level_1_pos.y - leve1_1_size.height / 2 - btn_top_gap));
        
        
        
        auto level_2_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                      CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_2));
        level_2_lock_btn->setPosition(Vec2(level_2_pos.x, level_2_pos.y - level_2_size.height / 2 - btn_top_gap));
        
        
        
        auto level_3_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                      CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_3));
        level_3_lock_btn->setPosition(Vec2(level_3_pos.x, level_3_pos.y - level_3_size.height / 2 - btn_top_gap));
        
        
        MenuItemImage *level_4_lock_btn, *level_5_lock_btn, *level_6_lock_btn, *level_7_lock_btn;

        bool level_4_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_4_LOCK_STATE_KEY, false);
        if (level_4_opened){
                level_4_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                              CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_4));
        }else{
                level_4_lock_btn = MenuItemImage::create("level/level_lock.png", "level/level_lock_sel.png",
                                                              CC_CALLBACK_1(LevelSelect::menuBuyLevel, this, LEVEL_4_PRODUCT_NAME_KEY));
                level_4_lock_btn->setTag(kLevel4BuyBtnTag);
        }
        level_4_lock_btn->setPosition(Vec2(level_4_pos.x, level_4_pos.y - level_4_size.height / 2 - btn_top_gap));
        
        
        
        bool level_5_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_5_LOCK_STATE_KEY, false);
        if (level_5_opened){
                level_5_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_5));
        }else{
                level_5_lock_btn = MenuItemImage::create("level/level_lock.png", "level/level_lock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuBuyLevel, this, LEVEL_5_PRODUCT_NAME_KEY));
        }
        level_5_lock_btn->setPosition(Vec2(level_5_pos.x, level_5_pos.y - level_5_size.height / 2 - btn_top_gap));
        

        bool level_6_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_6_LOCK_STATE_KEY, false);
        if (level_6_opened){
                level_6_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_6));
        }else{
                level_6_lock_btn = MenuItemImage::create("level/level_lock.png", "level/level_lock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuBuyLevel, this, LEVEL_6_PRODUCT_NAME_KEY));
        }
        level_6_lock_btn->setPosition(Vec2(level_6_pos.x, level_6_pos.y - level_6_size.height / 2 - btn_top_gap));
        
        bool level_7_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_7_LOCK_STATE_KEY, false);
        if (level_7_opened){
                level_7_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_7));
        }else{
                level_7_lock_btn = MenuItemImage::create("level/level_lock.png", "level/level_lock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuBuyLevel, this, LEVEL_7_PRODUCT_NAME_KEY));
        }
        level_7_lock_btn->setPosition(Vec2(level_7_pos.x, level_7_pos.y - level_7_size.height / 2 - btn_top_gap));
        
        auto menu = Menu::create(level_1_lock_btn, level_2_lock_btn, level_3_lock_btn, level_4_lock_btn,
                                 level_5_lock_btn, level_6_lock_btn, level_7_lock_btn, NULL);
        
        menu->setPosition(Vec2::ZERO);
        _levelShowBackGround->addChild(menu, ZORDER_ITEM_CONTROL, kLevelBuyMenuTag);
}

void LevelSelect::setSelectLevelBackPos(){
        
        Vec2 pos;
        switch (_lastLevel) {
                case GAME_LEVEL_INDEX_2:
                        pos = _levelShowBackGround->getChildByTag(kLevelShowLevel2Tag)->getPosition();
                        break;
                case GAME_LEVEL_INDEX_3:
                        pos = _levelShowBackGround->getChildByTag(kLevelShowLevel3Tag)->getPosition();
                        break;
                case GAME_LEVEL_INDEX_4:
                        pos = _levelShowBackGround->getChildByTag(kLevelShowLevel4Tag)->getPosition();
                        break;
                case GAME_LEVEL_INDEX_5:
                        pos = _levelShowBackGround->getChildByTag(kLevelShowLevel5Tag)->getPosition();
                        break;
                case GAME_LEVEL_INDEX_6:
                        pos = _levelShowBackGround->getChildByTag(kLevelShowLevel6Tag)->getPosition();
                        break;
                case GAME_LEVEL_INDEX_7:
                        pos = _levelShowBackGround->getChildByTag(kLevelShowLevel7Tag)->getPosition();
                        break;
                case GAME_LEVEL_INDEX_1:
                default:
                        pos = _levelShowBackGround->getChildByTag(kLevelShowLevel1Tag)->getPosition();
                        break;
        }
        _selectedBackGround->setPosition(pos);
}

void LevelSelect::loadLevelSelectedBackGround(){
        
        Size back_size = _levelShowBackGround->getContentSize();
        
        _selectedBackGround = LayerColor::create(Color4B::GRAY);
        _selectedBackGround->setContentSize(Size(200, back_size.height / 2 + 20));
        _selectedBackGround->setIgnoreAnchorPointForPosition(false);
        _selectedBackGround->setAnchorPoint(Vec2(0.5, 0.5));
        
        _lastLevel = UserDefault::getInstance()->getIntegerForKey(LAST_GAME_LEVEL_PLAYED_KEY, GAME_LEVEL_INDEX_1);
        _levelShowBackGround->addChild(_selectedBackGround, ZORDER_SEL_BACK_LAYER);
       
        this->setSelectLevelBackPos();
}


void LevelSelect::initActionListener(){
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesMoved = CC_CALLBACK_2(LevelSelect::onTouchesMoved, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LevelSelect::initButtons(Vec2 origin, Size visibleSize){
        
        bool is_effect_on = UserDefault::getInstance()->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        if (is_effect_on){
                _soundCtrl = MenuItemImage::create("Sound_on.png", "Sound_on_sel.png",
                                                   CC_CALLBACK_1(LevelSelect::menuSoundControl, this));
        }else{
                _soundCtrl = MenuItemImage::create("Sound_off.png", "Sound_off_sel.png",
                                                   CC_CALLBACK_1(LevelSelect::menuSoundControl, this));
        }
        _soundCtrl->setPosition(Vec2(2 * _soundCtrl->getContentSize().width,
                                     origin.y + 60));
        
        
        
        auto start_game = MenuItemImage::create("start_game.png", "start_game_sel.png",
                                                  CC_CALLBACK_1(LevelSelect::menuStartGame, this));
        start_game->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                     origin.y + 60));
        
        
        
        auto system_setting = MenuItemImage::create("level/game_setting.png", "level/game_setting_sel.png",
                                                  CC_CALLBACK_1(LevelSelect::menuShowSettigns, this));
        system_setting->setPosition(Vec2(origin.x + visibleSize.width -  system_setting->getContentSize().width,
                                     origin.y + 60));
        
        
        /*
         /////////////////////////////////////////////////////////
                        coins buttons
         /////////////////////////////////////////////////////////
         */
        auto coins_show = MenuItemImage::create("level/coins_show.png", "");
        Vec2 coins_pos = Vec2(origin.x + visibleSize.width - 2 * coins_show->getContentSize().width,
                              origin.y + visibleSize.height - coins_show->getContentSize().height);
        coins_show->setPosition(coins_pos);
        
        auto coins_back = LayerColor::create(Color4B::GRAY);
        coins_back->setIgnoreAnchorPointForPosition(false);
        coins_back->setAnchorPoint(Vec2(0.5, 0.5));
        coins_back->setContentSize(Size(100, 40));
        Vec2 coins_back_pos = Vec2(coins_pos.x - 60, coins_pos.y);
        coins_back->setPosition(coins_back_pos);
        _coinsNum = 100;//查询消费记录 apple
        auto coins_label = Label::createWithSystemFont(tostr(_coinsNum), "fonts/Marker Felt.ttf", 24);//
        coins_back->addChild(coins_label);
        coins_label->setPosition(coins_back->getContentSize() / 2);
        this->addChild(coins_back, ZORDER_BACK_LAYERS);
        
        
        auto add_coins = MenuItemImage::create("level/add_icon.png", "",CC_CALLBACK_1(LevelSelect::menuGetMoreCoins, this));
        Vec2 add_coins_pos = Vec2(coins_back_pos.x - 100, coins_pos.y);
        add_coins->setPosition(add_coins_pos);
        
        
        /*
         /////////////////////////////////////////////////////////
                        dices buttons
         /////////////////////////////////////////////////////////
         */
        auto dices_show = MenuItemImage::create("level/dice_show.png", "");
        Vec2 dices_pos = Vec2(add_coins_pos.x - 100, coins_pos.y);
        dices_show->setPosition(dices_pos);
        
        auto dices_back = LayerColor::create(Color4B::GRAY);
        dices_back->setIgnoreAnchorPointForPosition(false);
        dices_back->setAnchorPoint(Vec2(0.5, 0.5));
        dices_back->setContentSize(Size(100, 40));
        Vec2 dices_back_pos = Vec2(dices_pos.x - 60, dices_pos.y);
        dices_back->setPosition(dices_back_pos);
        _dicesNum = 100;//查询消费记录 apple
        auto dices_label = Label::createWithSystemFont(tostr(_coinsNum), "fonts/Marker Felt.ttf", 24);//
        dices_back->addChild(dices_label);
        dices_label->setPosition(dices_back->getContentSize() / 2);
        this->addChild(dices_back, ZORDER_BACK_LAYERS);
        
        auto add_dices = MenuItemImage::create("level/add_icon.png", "", CC_CALLBACK_1(LevelSelect::menuGetMoreDices, this));
        Vec2 add_dices_pos(dices_back_pos.x - 100, dices_pos.y);
        add_dices->setPosition(add_dices_pos);
        
        auto menu = Menu::create(start_game, _soundCtrl, system_setting, coins_show,
                                 add_coins, dices_show, add_dices, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, ZORDER_ITEM_CONTROL);
}

#pragma mark - button action callback
void LevelSelect::menuSelectLevel(Ref* btn, int bt_indx){
        
        _lastLevel = bt_indx;
        
        this->setSelectLevelBackPos();
        
        UserDefault::getInstance()->getIntegerForKey(LAST_GAME_LEVEL_PLAYED_KEY, bt_indx);
}

void LevelSelect::purchaseCallBack(sdkbox::Product& p){
        
}
void LevelSelect::menuBuyLevel(Ref* btn, std::string name){
        IAP::purchase(name);
}

void LevelSelect::menuStartGame(Ref* btn){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        _count = 0;
        _loadingBar = LoadingBar::create("sliderProgress.png");
        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
        _loadingBar->setPercent(0);
        
        Vec2 pos = Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 6);
        _loadingBar->setPosition(pos);
        this->addChild(_loadingBar, 3, key_loading_bar1);
        
        Size bar_size = _loadingBar->getContentSize();
        auto label = Label::createWithTTF("Loading", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(pos.x, pos.y + bar_size.height / 2));
        this->addChild(label, 4, key_loading_bar2);
        
        scheduleUpdate();
}

void LevelSelect::menuShowSettigns(Ref* btn){
        auto scene = GameSettings::createScene();
        Director::getInstance()->pushScene(scene);
}


void LevelSelect::menuGetMoreCoins(Ref* btn){
        IAP::restore();
}
void LevelSelect::menuGetMoreDices(Ref* btn){
        
}

void LevelSelect::menuSoundControl(Ref* btn){
        bool is_effect_on = UserDefault::getInstance()->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        UserDefault::getInstance()->setBoolForKey(SOUND_EFFECT_SWITCH_KEY, !is_effect_on);
        
        if (is_effect_on){
                _soundCtrl->setNormalImage(Sprite::create("Sound_off.png"));
                _soundCtrl->setSelectedImage(Sprite::create("Sound_off_sel.png"));
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
        }else{
                _soundCtrl->setNormalImage(Sprite::create("Sound_on.png"));
                _soundCtrl->setSelectedImage(Sprite::create("Sound_on_sel.png"));
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE);
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
        }
}

void LevelSelect::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
        auto touch = touches[0];
        
        auto diff = touch->getDelta();
        diff.y = 0;
        auto current_pos = _levelShowBackGround->getPosition();
        auto want_to_be = current_pos + diff;
        
        if (want_to_be.x < _mostLeft.x || want_to_be.x > _mostRight.x){
                return;
        }
        _levelShowBackGround->setPosition(want_to_be);
}


#pragma mark - loading bar
void LevelSelect::update(float delta){
        
        if (_count < 100)
                _count += 3;
        else{
                _count = 0;
                _loadingBar->setPercent(0);
                auto scene = GameScene::createScene(_lastLevel);
                Director::getInstance()->pushScene(scene);
        }
        
        _loadingBar->setPercent(_count);
}

void LevelSelect::onExit(){
        Layer::onExit();
        this->removeChildByTag(key_loading_bar1);
        this->removeChildByTag(key_loading_bar2);
        unscheduleUpdate();
}


#pragma mark - payment callback
void LevelSelect::onSuccess(const Product& p){
        auto menu = _levelShowBackGround->getChildByTag(kLevelBuyMenuTag);
        
        if (LEVEL_4_PRODUCT_NAME_KEY == p.name){
                
                MenuItemImage* btn = (MenuItemImage*)menu->getChildByTag(kLevel4BuyBtnTag);
                btn->setNormalImage(Sprite::create("level/level_lock.png"));
                btn->setSelectedImage(Sprite::create("level/level_lock_sel.png"));
                UserDefault::getInstance()->setBoolForKey(LEVEL_4_LOCK_STATE_KEY, true);
                btn->setCallback(CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_4));
                
                
        }else if(LEVEL_5_PRODUCT_NAME_KEY == p.name){
                
                MenuItemImage* btn = (MenuItemImage*)menu->getChildByTag(kLevel5BuyBtnTag);
                btn->setNormalImage(Sprite::create("level/level_lock.png"));
                btn->setSelectedImage(Sprite::create("level/level_lock_sel.png"));
                UserDefault::getInstance()->setBoolForKey(LEVEL_5_LOCK_STATE_KEY, true);
                btn->setCallback(CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_5));
                
        }else if(LEVEL_6_PRODUCT_NAME_KEY == p.name){
                
                MenuItemImage* btn = (MenuItemImage*)menu->getChildByTag(kLevel6BuyBtnTag);
                btn->setNormalImage(Sprite::create("level/level_lock.png"));
                btn->setSelectedImage(Sprite::create("level/level_lock_sel.png"));
                UserDefault::getInstance()->setBoolForKey(LEVEL_6_LOCK_STATE_KEY, true);
                btn->setCallback(CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_6));
                
        }else if(LEVEL_7_PRODUCT_NAME_KEY == p.name){
                
                MenuItemImage* btn = (MenuItemImage*)menu->getChildByTag(kLevel7BuyBtnTag);
                btn->setNormalImage(Sprite::create("level/level_lock.png"));
                btn->setSelectedImage(Sprite::create("level/level_lock_sel.png"));
                UserDefault::getInstance()->setBoolForKey(LEVEL_7_LOCK_STATE_KEY, true);
                btn->setCallback(CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_7));
                
        }else if (REMOVE_ADS_PRODUCT_NAME_KEY == p.name){
                
                UserDefault::getInstance()->setBoolForKey(ADS_LOCK_STATE_KEY, false);
        }
        
        UserDefault::getInstance()->flush();       
}
void LevelSelect::onFailure(const Product& p, const std::string& msg){
         CCLOG("Purchase Failed: %s", msg.c_str());
}
void LevelSelect::onCanceled(const Product& p){
        CCLOG("Purchase Canceled: %s", p.id.c_str());
}

void LevelSelect::onProductRequestSuccess(const std::vector<Product>& products){
        _productsMap.clear();
        for (int i=0; i < products.size(); i++){
                CCLOG("IAP: ========= IAP Item =========");
                CCLOG("IAP: Name: %s", products[i].name.c_str());
                CCLOG("IAP: ID: %s", products[i].id.c_str());
                CCLOG("IAP: Title: %s", products[i].title.c_str());
                CCLOG("IAP: Desc: %s", products[i].description.c_str());
                CCLOG("IAP: Price: %s", products[i].price.c_str());
                CCLOG("IAP: Price Value: %f", products[i].priceValue);
                _productsMap.insert(pair<string, Product>(products[i].name, products[i]));
        }
}

void LevelSelect::onProductRequestFailure(const std::string& msg){
        CCLOG("onProductRequestFailure Restored: %s", msg.c_str());
}
void LevelSelect::onRestoreComplete(bool ok, const std::string &msg){
        CCLOG("%s:%d:%s", __func__, ok, msg.data());
}

