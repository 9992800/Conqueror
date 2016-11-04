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
#include "PopUpOkCancelDialog.hpp"
#include "PopUpOkDialog.hpp"

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
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        
        _controlLayer = Layer::create();
        _endTurnMenuItem = MenuItemImage::create("NextButton.png", "NextButton.png",
                                             CC_CALLBACK_1(GameScene::menuEndTurn, this));
        _endTurnMenuItem->setPosition(Vec2(origin.x + visibleSize.width - _endTurnMenuItem->getContentSize().width,
                                       origin.y + _endTurnMenuItem->getContentSize().height));
        _endTurnMenuItem->setVisible(false);
        
        auto start_play = MenuItemImage::create("start.png", "start.png",
                                             CC_CALLBACK_1(GameScene::menuStartGame, this));
        start_play->setPosition(Vec2(origin.x + visibleSize.width - start_play->getContentSize().width,
                                       origin.y +start_play->getContentSize().height));
        
        auto return_back = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                             CC_CALLBACK_1(GameScene::menuExit, this));
        return_back->setPosition(Vec2(origin.x + return_back->getContentSize().width + 10,
                                       origin.y + visibleSize.height - return_back->getContentSize().height - 10));
        
        auto menu = Menu::create(_endTurnMenuItem, start_play, return_back, NULL);
        menu->setPosition(Vec2::ZERO);
        _controlLayer->addChild(menu);
        
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
        
        if (_isMoved || _gameStatus != GAME_STATUS_INUSERTURN) {
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

void GameScene::afterRobootBattle(int result){
        _theGameLogic->cleanUpBattleField(result);
}


void GameScene::afterPlayerSupply(int result){
        
}
void GameScene::afterRobootSupply(int result){
        
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

void GameScene::playSupplyAnimation(int res, CallFunc* callback){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE);
        _theGameLogic->starSupplyDice(callback);
}

#pragma mark - menu callback actions
void GameScene::menuEndTurn(Ref* pSender){
        ((MenuItemImage*)pSender)->setVisible(false);
        _gameStatus = GAME_STATUS_AIRUNNING;
        _theGameLogic->clearManulAction();
        _theGameLogic->startRobootAttack();
}

void GameScene::menuStartGame(Ref* pSender){
        ((MenuItemImage*)pSender)->setVisible(false);
        
        _gameStatus = GAME_STATUS_AIRUNNING;
        int res = _theGameLogic->startRobootAttack();
        
        if (ATTACK_RES_WIN == res || ATTACK_RES_DEFEATED == res){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootBattle, this, res));
                this->playBattleAnimation(res, callback);
                
        }else if(ATTACK_RES_GOTSUPPLY == res){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootSupply, this, res));
                this->playSupplyAnimation(res, callback);
                
        }else if (ATTACK_RES_NONE == res){
                
                 _gameStatus = GAME_STATUS_INUSERTURN;
                _endTurnMenuItem->setVisible(true);
                
        }
}

void GameScene::menuExit(Ref* pSender){
        
        Director::getInstance()->pause();
        BaseDialogConfig config("失败", "娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香");
        PopUpOkCancelDialog *dialog = PopUpOkCancelDialog::create(config,
                                                                  CC_CALLBACK_1(GameScene::gameOver, this, 1),
                                                                  CC_CALLBACK_1(GameScene::gameOver, this, 0));
        dialog->retain();
        this->addChild(dialog, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
}

void GameScene::gameOver(Ref* btn, int result){
        if (result == 1){
                Director::getInstance()->popScene();
        }else{
                this->removeChildByTag(key_dialog_layer_tag);
        }
        
        Director::getInstance()->resume();        
}

