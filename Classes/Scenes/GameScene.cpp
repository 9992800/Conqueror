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



Vec2 invader_fight_pos[] = FIGHT_POS_INVADER;
Vec2 keeper_fight_pos[]  = FIGHT_POS_KEEPER;
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
        
        auto frameCache = SpriteFrameCache::getInstance();
        frameCache->addSpriteFramesWithFile("anim/hanshirun.plist", "anim/hanshirun.png");
        frameCache->addSpriteFramesWithFile("anim/hanshisd.plist", "anim/hanshisd.png");
        frameCache->addSpriteFramesWithFile("anim/xunshoushirun.plist", "anim/xunshoushirun.png");
        frameCache->addSpriteFramesWithFile("anim/xunshoushisd.plist", "anim/xunshoushisd.png");
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        for (int i = 0; i < 9; i++){
                keeper_fight_pos[i].x = visibleSize.width + keeper_fight_pos[i].x;
        }
        
        this->initMapLayer();
        
        this->initControlLayer();
        
        this->initAnimationLayer();
        
        int game_speed = UserDefault::getInstance()->getIntegerForKey(GAME_SPEED_KEY, 1);
        Director::getInstance()->getScheduler()->setTimeScale(game_speed);
        return true;
}

#pragma mark - initilization
void GameScene::initMapLayer(){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        _theGameLogic = DiceGame::create();
        _theGameLogic->retain();
        
        auto data = _theGameLogic->initGameData(_playerNumber);
#if DONT_USER_TILE_MAP
        
        auto back_layer = LayerColor::create(TILE_COLOR_BACKGRUND, visibleSize.width + 10, visibleSize.height + 20);
        //TILE_COLOR_BACKGRUND  //Color4B::WHITE
        ScreenCoordinate::getInstance()->configScreen(visibleSize);
        data->reshDataByBackGrnd(back_layer);
        this->addChild(back_layer, ZORDER_MAP_GROUND, key_map_tag);
        _lowestPostion_y = visibleSize.height + origin.y - visibleSize.height - 6;//TODO::         
#else
        auto map = MapCreator::instance()->createMap(data->getMapData());
        Size map_size = map->getContentSize();
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        data->reshDataByMapInfo(map);
        
        this->addChild(map, ZORDER_MAP_GROUND, key_map_tag);
        
        _lowestPostion_y = visibleSize.height + origin.y - map_size.height - 6;//TODO::
#endif
       
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
        
        _startPlayMenuItem = MenuItemImage::create("start.png", "start.png",
                                             CC_CALLBACK_1(GameScene::menuStartGame, this));
        _startPlayMenuItem->setPosition(Vec2(origin.x + visibleSize.width - _startPlayMenuItem->getContentSize().width,
                                       origin.y +_startPlayMenuItem->getContentSize().height));
        
        auto return_back = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                             CC_CALLBACK_1(GameScene::menuExit, this));
        return_back->setPosition(Vec2(origin.x + return_back->getContentSize().width + 10,
                                       origin.y + visibleSize.height - return_back->getContentSize().height - 10));
        
        auto menu = Menu::create(_endTurnMenuItem, _startPlayMenuItem, return_back, NULL);
        menu->setPosition(Vec2::ZERO);
        _controlLayer->addChild(menu);
        
        this->addChild(_controlLayer, ZORDER_CRTL_LAYERS, key_ctrl_layer_tag);
        
        Director::getInstance()->setDepthTest(true);
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::loadZhanshi(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        for(int i = 1; i <= 12; i++){
                sprintf(str, "renwurun%04D.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "zhanshi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "renwu_sd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "zhanshi_sd");
        
        
        
        auto frame = frameCache->getSpriteFrameByName("renwurun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto zhanshi = Sprite::create();
                zhanshi->setSpriteFrame(frame);
                _allFightingCharacters[0][i] = zhanshi;
                zhanshi->setVisible(false);
                _animationLayer->addChild(zhanshi);
        }
//        frame = frameCache->getSpriteFrameByName("renwu_sd0001.png");
//        zhanshi = Sprite::create();
//        zhanshi->setSpriteFrame(frame);
//        _playerZhanshi[MAX_DICE_PER_AREA] = zhanshi;
}

void GameScene::loadXunShouShi(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
                 
        auto frame = frameCache->getSpriteFrameByName("xssrun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto xunshoushi = Sprite::create();
                xunshoushi->setSpriteFrame(frame);
                _allFightingCharacters[1][i] = xunshoushi;
                xunshoushi->setVisible(false);
                _animationLayer->addChild(xunshoushi);
        }
//        frame = frameCache->getSpriteFrameByName("xsssd0001.png");
//        xunshoushi = Sprite::create();
//        xunshoushi->setSpriteFrame(frame);
//        _playerXunshouShi[MAX_DICE_PER_AREA] = xunshoushi;
}


void GameScene::initAnimationLayer(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        _animationLayer = Sprite::create("zhandou_beijing.png");
        _animationLayer->setScale(0.1f, 0.1f);
        
//        _animationLayer->setIgnoreAnchorPointForPosition(false);
//        _animationLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
        
        auto anim_back_size = _animationLayer->getContentSize();
        _animationLayer->setPosition(Vec2(visibleSize - anim_back_size / 2));
//        _animationLayer->setVisible(false);
        
        this->addChild(_animationLayer, ZORDER_ANIM_LAYER, key_anim_layer_tag);
        
        this->loadZhanshi();
        
        this->loadXunShouShi();
}


GameScene::~GameScene(){
        _theGameLogic->release();
        
        auto frameCache = SpriteFrameCache::getInstance();
        frameCache->removeSpriteFramesFromFile("anim/hanshirun.plist");
        frameCache->removeSpriteFramesFromFile("anim/hanshisd.plist");
        frameCache->removeSpriteFramesFromFile("anim/xunshoushirun.plist");
        frameCache->removeSpriteFramesFromFile("anim/xunshoushisd.plist");
}

#pragma mark - touch and menu event

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
        
        auto touch = touches[0];
        auto diff = touch->getDelta();

        if (diff.x >= 0.01f || diff.y >= 0.01
            || diff.x <= -0.01f ||diff.y <= -0.01f){
                _isMoved = true;
        }
        
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
        
        if (_isMoved || _isPalyingAnim
            ||_gameStatus != GAME_STATUS_INUSERTURN) {
                _isMoved = false;
                return;
        }
        
        auto touch = touches[0];
        auto position = touch->getLocation();
        auto map = this->getChildByTag(key_map_tag);
        Vec2 pos_in_map = map->convertToNodeSpace(position);
        
        if (pos_in_map.x < 0 || pos_in_map.y < 0){
                return;
        }
        int cell_id = ScreenCoordinate::getInstance()->getSelectedCell(pos_in_map);
        if (cell_id < 0){
                return;
        }
        
        int result = _theGameLogic->startPlayerAttack(cell_id);
        if (ATTACK_RES_WIN == result|| ATTACK_RES_DEFEATED == result){
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterPlayerBattle, this, result));
                this->playManualBattleAnimation(result, callback);
        }
}

void GameScene::tryAgain(){
        _gameStatus = GAME_STATUS_INIT;
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto layer = this->getChildByTag(key_map_tag);
        layer->removeFromParentAndCleanup(true);
        
        auto data = _theGameLogic->resetInitData();
        
#if DONT_USER_TILE_MAP
        auto back_layer = LayerColor::create(TILE_COLOR_BACKGRUND, visibleSize.width + 10, visibleSize.height + 20);//TILE_COLOR_BACKGRUND  //Color4B::WHITE 
        ScreenCoordinate::getInstance()->configScreen(visibleSize);
        data->reshDataByBackGrnd(back_layer);
        this->addChild(back_layer, ZORDER_MAP_GROUND, key_map_tag);

#else        
        auto map = MapCreator::instance()->createMap(data->getMapData());
        Size map_size = map->getContentSize();
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        data->reshDataByMapInfo(map);
        
        this->addChild(map, ZORDER_MAP_GROUND, key_map_tag);

#endif
        _startPlayMenuItem->setVisible(true);
        _endTurnMenuItem->setVisible(false);
}

#pragma mark - animation 

void GameScene::afterPlayerBattle(int result){
        _isPalyingAnim = false;
//        _tamara->setVisible(false);
        std::map<int, int> survival = _theGameLogic->cleanUpBattleField(result);
        if (survival.size() == 1){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_FINISH_WIN);
                Director::getInstance()->pause();
                BaseDialogConfig config("胜利了!",
                                        "娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香");
                PopUpOkCancelDialog *dialog = PopUpOkCancelDialog::create(config,
                                                                          CC_CALLBACK_1(GameScene::gameOver, this, 1),
                                                                          CC_CALLBACK_1(GameScene::gameOver, this, 0));
                this->addChild(dialog, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
                return;
        }
}

void GameScene::afterRobootBattle(int result){
        _isPalyingAnim = false;
        _animationLayer->setVisible(false);
        _theGameLogic->cleanUpBattleField(result);
        
        int user_tc = _theGameLogic->getUserTC();
        if (0 == user_tc){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_FINISH_LOSE);
                Director::getInstance()->pause();
                BaseDialogConfig config("失败，需要重试吗？",
                                        "娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香");
                PopUpOkCancelDialog *dialog = PopUpOkCancelDialog::create(config,
                                                                          CC_CALLBACK_1(GameScene::gameOver, this, 1),
                                                                          CC_CALLBACK_1(GameScene::gameOver, this, 0));
                this->addChild(dialog, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
                return;
        }
        
        this->gameAction();
}


void GameScene::afterPlayerSupply(){
        _theGameLogic->next_player();
        this->gameAction();
}
void GameScene::afterRobootSupply(){
        _theGameLogic->next_player();
        this->gameAction();
}

void GameScene::gameAction(){
        int res = _theGameLogic->startRobootAttack();
        
        if (ATTACK_RES_WIN == res || ATTACK_RES_DEFEATED == res){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootBattle, this, res));
//                this->playRobbotBattleAnimation(res, callback);
                this->playManualBattleAnimation(res, callback);
                
        }else if(ATTACK_RES_GOTSUPPLY == res){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootSupply, this));
                this->playSupplyAnimation(callback);
                
        }else if (ATTACK_RES_NONE == res){
                
                _gameStatus = GAME_STATUS_INUSERTURN;
                _endTurnMenuItem->setVisible(true);
                
        }
}

void GameScene::afterFightFinished(CallFunc* cb){
        auto hide = ScaleBy::create(1.0f, .1f);
        _animationLayer->runAction(Sequence::create(hide, cb, NULL));
        cb->release();
}

void GameScene::afterShowFightBg(CallFunc*cb){
        Size visible_size = Director::getInstance()->getVisibleSize();
        auto cc = CallFunc::create(std::bind(&GameScene::afterFightFinished, this, cb));
        
        
        auto p_test = _allFightingCharacters[0][0];
        p_test->setVisible(true);
        p_test->setPosition(invader_fight_pos[0] - Vec2(READY_DISTANCE_POS, 0));
        
        
        auto cache = AnimationCache::getInstance();
        auto animation = cache->getAnimation("zhanshi_run");
        animation->setRestoreOriginalFrame(true);
        auto action = Animate::create(animation);
        
        auto move = MoveBy::create(2, Vec2(READY_DISTANCE_POS,0));
        Spawn* get_ready = Spawn::create(action, move, NULL);
        
        animation = cache->getAnimation("zhanshi_sd");
        animation->setRestoreOriginalFrame(true);
        auto fight_wait = Animate::create(animation);
        
        
        auto moveby = MoveBy::create(2, Vec2(visible_size.width / 2 -READY_DISTANCE_POS,0));
        auto run_to_fight = Spawn::create(action->clone(), moveby, NULL);
        
        //TODO:: add clound
        Sequence* invade_seq = Sequence::create(get_ready,
                                                      fight_wait,
                                                      run_to_fight,
                                                      run_to_fight->reverse(),
                                                      fight_wait,
                                                      get_ready->reverse(),
                                                      cc, NULL);
        p_test->runAction(invade_seq);
        
        
        auto p_test2 = _allFightingCharacters[1][0];
        p_test2->setVisible(true);
        p_test2->setPosition(keeper_fight_pos[0] + Vec2(READY_DISTANCE_POS, 0));
        
        auto animation2 = cache->getAnimation("xunshoushi_run");
        animation2->setRestoreOriginalFrame(true);
        auto action2 = Animate::create(animation2);
        
        auto move2 = MoveBy::create(2, Vec2(-READY_DISTANCE_POS,0));
        Spawn* get_ready2 = Spawn::create(action2, move2, NULL);
        
        
        animation2 = cache->getAnimation("xunshoushi_sd");
        animation2->setRestoreOriginalFrame(true);
        auto fight_wait2 = Animate::create(animation2);
        
        
        auto moveby2 = MoveBy::create(2, Vec2(READY_DISTANCE_POS - visible_size.width / 2 ,0));
        auto run_to_fight2 = Spawn::create(action2->clone(), moveby2, NULL);
        
        Sequence* keeper_seq = Sequence::create(get_ready2,
                                                fight_wait2,
                                                run_to_fight2,
                                                NULL);
        p_test2->runAction(keeper_seq);
}

void GameScene::playRobbotBattleAnimation(int res, CallFunc* callback){
        //TODO::replace this animation by a occupation animation
        auto show = ScaleBy::create(1.0f, 1.0f);
        _animationLayer->runAction(Sequence::create(show, callback, NULL));
}

void GameScene::playManualBattleAnimation(int res, CallFunc* callback){
        if (ATTACK_RES_WIN == res){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_WIN);
        }else if (ATTACK_RES_DEFEATED == res){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_DEFEAT);
        }
        
        bool is_anim_on = UserDefault::getInstance()->getBoolForKey(ANIMATION_SWITCH_KEY, true);
        if (is_anim_on){
                _isPalyingAnim = true;
                auto show = ScaleBy::create(1.0f, 10.0f);
                _animationLayer->setVisible(true);
                callback->retain();
                auto cc = CallFunc::create(std::bind(&GameScene::afterShowFightBg, this, callback));
                _animationLayer->runAction(Sequence::create(show, cc, NULL));
                
        }else{
                //TODO::replace this animation by a occupation animation
                auto show = ScaleBy::create(1.0f, 1.0f);
                _animationLayer->runAction(Sequence::create(show, callback, NULL));
        }
}

void GameScene::playSupplyAnimation(CallFunc* callback){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_SUPPLY);
        _theGameLogic->starSupplyDice(callback);
}

#pragma mark - menu callback actions
void GameScene::menuEndTurn(Ref* pSender){
        if (_isPalyingAnim){
                return;
        }
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
        
        ((MenuItemImage*)pSender)->setVisible(false);
        _theGameLogic->clearManulAction();
        _gameStatus = GAME_STATUS_AIRUNNING;
        CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootSupply, this));
        this->playSupplyAnimation(callback);
}

void GameScene::menuStartGame(Ref* pSender){
        _theGameLogic->initHistoryRecord();
        ((MenuItemImage*)pSender)->setVisible(false);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
        _gameStatus = GAME_STATUS_AIRUNNING;
        this->gameAction();
}

void GameScene::menuExit(Ref* pSender){
        
        Director::getInstance()->pause();
        BaseDialogConfig config("确认要退出吗？", "娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香");
        PopUpOkCancelDialog *dialog = PopUpOkCancelDialog::create(config,
                                                                  CC_CALLBACK_1(GameScene::gameExit, this, 1),
                                                                  CC_CALLBACK_1(GameScene::gameExit, this, 0));
        dialog->setButtonTittle("退出","取消");
        this->addChild(dialog, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
}

void GameScene::gameExit(Ref* btn, int result){
        if (result == 1){
                Director::getInstance()->popScene();
        }else{
                this->removeChildByTag(key_dialog_layer_tag);
        }
        
        Director::getInstance()->resume();        
}

void GameScene::gameOver(Ref* btn, int result){
        if (result == 0){
                _theGameLogic->finishHistoryRecord();
                Director::getInstance()->popScene();
        }else{
                this->removeChildByTag(key_dialog_layer_tag);
                this->tryAgain();
        }
        
        Director::getInstance()->resume();
}
