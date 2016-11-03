//
//  LevelSelecteScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//
#include "LevelSelectScene.hpp"
enum{
        kLevelShowLevel1Tag = 2,
        kLevelShowLevel2Tag = 3,
        kLevelShowLevel3Tag = 4,
        kLevelShowLevel4Tag = 5,
        kLevelShowLevel5Tag = 6,
        kLevelShowLevel6Tag = 7,
        kLevelShowLevel7Tag = 8
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
        
        return true;
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
        _levelShowBackGround->setPosition(Vec2(center.x + (back_size.width - visibleSize.width) / 2 ,center.y));
        
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
                                                              CC_CALLBACK_1(LevelSelect::menuBuyLevel, this, GAME_LEVEL_INDEX_4));
        }
        level_4_lock_btn->setPosition(Vec2(level_4_pos.x, level_4_pos.y - level_4_size.height / 2 - btn_top_gap));
        
        
        
        bool level_5_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_5_LOCK_STATE_KEY, false);
        if (level_5_opened){
                level_5_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_5));
        }else{
                level_5_lock_btn = MenuItemImage::create("level/level_lock.png", "level/level_lock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuBuyLevel, this, GAME_LEVEL_INDEX_5));
        }
        level_5_lock_btn->setPosition(Vec2(level_5_pos.x, level_5_pos.y - level_5_size.height / 2 - btn_top_gap));
        

        bool level_6_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_4_LOCK_STATE_KEY, false);
        if (level_6_opened){
                level_6_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_6));
        }else{
                level_6_lock_btn = MenuItemImage::create("level/level_lock.png", "level/level_lock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuBuyLevel, this, GAME_LEVEL_INDEX_6));
        }
        level_6_lock_btn->setPosition(Vec2(level_6_pos.x, level_6_pos.y - level_6_size.height / 2 - btn_top_gap));
        
        bool level_7_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_4_LOCK_STATE_KEY, false);
        if (level_7_opened){
                level_7_lock_btn = MenuItemImage::create("level/level_unlock.png", "level/level_unlock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuSelectLevel, this, GAME_LEVEL_INDEX_7));
        }else{
                level_7_lock_btn = MenuItemImage::create("level/level_lock.png", "level/level_lock_sel.png",
                                                         CC_CALLBACK_1(LevelSelect::menuBuyLevel, this, GAME_LEVEL_INDEX_7));
        }
        level_7_lock_btn->setPosition(Vec2(level_7_pos.x, level_7_pos.y - level_7_size.height / 2 - btn_top_gap));
        
        auto menu = Menu::create(level_1_lock_btn, level_2_lock_btn, level_3_lock_btn, level_4_lock_btn,
                                 level_5_lock_btn, level_6_lock_btn, level_7_lock_btn, NULL);
        
        menu->setPosition(Vec2::ZERO);
        _levelShowBackGround->addChild(menu, ZORDER_ITEM_CONTROL);
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
        _selectedBackGround->setPosition(pos - Vec2(0, 30));
}

void LevelSelect::loadLevelSelectedBackGround(){
        
        Size back_size = _levelShowBackGround->getContentSize();
        
        _selectedBackGround = LayerColor::create(Color4B::GRAY);
        _selectedBackGround->setContentSize(Size(150, back_size.height / 2 +40));
        _selectedBackGround->setIgnoreAnchorPointForPosition(false);
        _selectedBackGround->setAnchorPoint(Vec2(0.5, 0.5));
        
        _lastLevel = UserDefault::getInstance()->getIntegerForKey(LAST_GAME_LEVEL_PLAYED_KEY, GAME_LEVEL_INDEX_1);
        _levelShowBackGround->addChild(_selectedBackGround, ZORDER_SEL_BACK_LAYER);
       
        this->setSelectLevelBackPos();
}


#pragma mark - button action callback
void LevelSelect::menuSelectLevel(Ref* btn, int bt_indx){
        UserDefault::getInstance()->getIntegerForKey(LAST_GAME_LEVEL_PLAYED_KEY, bt_indx);
        _lastLevel = bt_indx;
        
        this->setSelectLevelBackPos();
}

void LevelSelect::menuBuyLevel(Ref* btn, int num){
        
}

void LevelSelect::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
        auto touch = touches[0];
        
        auto diff = touch->getDelta();
        diff.y = 0;
        auto currentPos = _levelShowBackGround->getPosition();
        auto origin = Director::getInstance()->getVisibleOrigin();
        
        auto layer_size = _levelShowBackGround->getContentSize();
        
//        if (origin.y < (currentPos.y + diff.y)){
//                diff.y = origin.y - currentPos.y;
//        }
//        
//        if ((currentPos.y + diff.y) < _lowestPostion_y){
//                diff.y = _lowestPostion_y - currentPos.y;
//        }
        
//        _randomMap->setPosition(currentPos + diff);
}


#pragma mark - loading bar
void LevelSelect::update(float delta){
        
        if (_count < 100)
                _count++;
        else{
                _count = 0;
        }
        
        _loadingBar->setPercent(_count);
}

void LevelSelect::onExit(){
        Layer::onExit();
        unscheduleUpdate();
}
