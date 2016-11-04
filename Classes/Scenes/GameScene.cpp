//
//  DiceGameScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//
#include "GameData.hpp"
#include "MapCreator.hpp"
#include "GameScene.hpp"
#include "ScreenCoordinate.hpp"
#include "SimpleAudioEngine.h"


enum{
        ZORDER_BACK_GROUND = 0,
        ZORDER_MAP_GROUND,
        ZORDER_CRTL_LAYERS,
        ZORDER_ANIM_LAYER,
        ZORDER_DIALOG_LAYER
};
enum{
        key_map_tag             = 1,
        key_ctrl_layer_tag,
        key_anim_layer_tag,
        key_dialog_layer_tag
};


#pragma mark - constructor
Scene* GameScene::createScene(int gameLevel)
{
        auto scene = Scene::create();
        auto layer = GameScene::create(gameLevel);
        
        scene->addChild(layer); 
        return scene;
}

bool GameScene::init()
{
        if (!Layer::init()){
                return false;
        }
        this->initMapLayer();
        
        this->initControlLayer();
        
        this->initAnimationLayer();
        
        int game_speed = UserDefault::getInstance()->getIntegerForKey(GAME_SPEED_KEY, 4);
        Director::getInstance()->getScheduler()->setTimeScale(game_speed);
        
        return true;
}

#pragma mark - initilization
void GameScene::initMapLayer(){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;

        auto back_ground = Sprite::create("starting_back.png");
        back_ground->setPosition(center);
        this->addChild(back_ground, ZORDER_BACK_GROUND);
        
        _theGameLogic = DiceGame::create();
        _theGameLogic->retain();        
        auto data = _theGameLogic->initGameData(_playerNumber);
        
        auto map = MapCreator::instance()->createMap(data);
        Size map_size = map->getContentSize();
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        data->reshDataByMapInfo(map);       
        
        this->addChild(map, ZORDER_MAP_GROUND, key_map_tag);
        
        _lowestPostion_y = visibleSize.height + origin.y - map_size.height - 6;//TODO::
}

void GameScene::initControlLayer(){
        
        _controlLayer = Layer::create();
        this->addChild(_controlLayer, ZORDER_CRTL_LAYERS, key_ctrl_layer_tag); 
        
        Director::getInstance()->setDepthTest(true);
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::initAnimationLayer(){
        _animationLayer = Layer::create();
        
        _tamara = Sprite::create("anim/grossinis_sister1.png");
        _tamara->setPosition(Vec2(100, 100));
        _animationLayer->addChild(_tamara);
        
        this->addChild(_animationLayer, ZORDER_ANIM_LAYER, key_anim_layer_tag);
}

#pragma mark - touch and menu event

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
        _isMoved = true;
        auto touch = touches[0];
        
        auto diff = touch->getDelta();
        diff.x = 0;
        auto map = this->getChildByTag(key_map_tag);
        auto currentPos = map->getPosition();
        auto origin = Director::getInstance()->getVisibleOrigin(); 
        
        if (origin.y < (currentPos.y + diff.y)){
                diff.y = origin.y - currentPos.y;
        }
        
        if ((currentPos.y + diff.y) < _lowestPostion_y){
                diff.y = _lowestPostion_y - currentPos.y;
        }
        
        map->setPosition(currentPos + diff);
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *event){        
        
        if (_isMoved) {
                _isMoved = false;
                return;
        }
        auto touch = touches[0];
        auto position = touch->getLocation();
        auto map = this->getChildByTag(key_map_tag);
        Vec2 pos_in_map = map->convertToNodeSpace(position);
        int cell_id = ScreenCoordinate::getInstance()->getSelectedCell(pos_in_map);
        
        int result = _theGameLogic->startPlayerAttack(cell_id);
        if (ATTACK_RES_WIN == result|| ATTACK_RES_DEFEATED == result){
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterPlayerBattle, this, result));
                this->playBattleAnimation(result, callback);
        }
}

//TODO:: need to test
void GameScene::tryAgain(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto old_map = this->getChildByTag(key_map_tag);
        old_map->removeFromParentAndCleanup(true);
        
        auto data = _theGameLogic->resetInitData();
        auto map = MapCreator::instance()->createMap(data);
        Size map_size = map->getContentSize();
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        data->reshDataByMapInfo(map);
        
        this->addChild(map, ZORDER_MAP_GROUND, key_map_tag);
        
        _lowestPostion_y = visibleSize.height + origin.y - map_size.height;
}

#pragma mark - animation 

void GameScene::afterPlayerBattle(int result){
        _theGameLogic->cleanUpBattleField(result);
}
void GameScene::playBattleAnimation(int res, CallFunc* callback){
        if (ATTACK_RES_WIN == res){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE);
        }else if (ATTACK_RES_DEFEATED == res){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE);
        }
        bool is_anim_on = UserDefault::getInstance()->getBoolForKey(ANIMATION_SWITCH_KEY, true);
        if (is_anim_on){
                _tamara->setVisible(true);
                
                auto cache = AnimationCache::getInstance();
                cache->addAnimationsWithFile("anim/animations-2.plist");
                auto animation2 = cache->getAnimation("dance_1");
                auto action2 = Animate::create(animation2);
                
                Sequence*  s = Sequence::create(action2, callback, nullptr);
                _tamara->runAction(s);
        }else{
                callback->execute();
        }
}
