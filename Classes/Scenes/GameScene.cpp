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
        ZORDER_DICE_LAYER,
        ZORDER_DIALOG_LAYER
};
enum{
        key_map_tag             = 1,
        key_ctrl_layer_tag,
        key_anim_layer_tag,
        key_dice_layer_tag,
        key_dialog_layer_tag
};

static std::string ANIM_NAME_FIGHT_RUN[]        = {"zhanshi_run", "xunshoushi_run", "zhanshi_run", "xunshoushi_run",
        "zhanshi_run", "xunshoushi_run", "zhanshi_run", "xunshoushi_run"};

static std::string ANIM_NAME_FIGHT_STAND[]      = {"zhanshi_sd", "xunshoushi_sd", "zhanshi_sd", "xunshoushi_sd",
        "zhanshi_sd", "xunshoushi_sd", "zhanshi_sd", "xunshoushi_sd"};


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
        
        this->initMapLayer();
        
        this->initControlLayer();
        
        this->initAnimationLayer();
        
        int game_speed = UserDefault::getInstance()->getIntegerForKey(GAME_SPEED_KEY, 3);
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
        Vector<SpriteFrame*> animFrames_back(12);
        char str[100] = {0};
        for(int i = 1; i <= 12; i++){
                sprintf(str, "renwurun%04d.png", i);
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
                zhanshi->setVisible(false);
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
                _animationLayer->addChild(xunshoushi);
                xunshoushi->setVisible(false);
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
        _animationLayer->setPosition(Vec2(visibleSize / 2));
        _animationLayer->setVisible(false);
        
        this->addChild(_animationLayer, ZORDER_ANIM_LAYER, key_anim_layer_tag);
        
        this->loadZhanshi();
        
        this->loadXunShouShi();
        
        
        
        _diceResultLayer = Layer::create();
        this->addChild(_diceResultLayer, ZORDER_DICE_LAYER, key_dice_layer_tag);
        _diceResultLayer->setVisible(false);
        
        for (int i = 0; i < 9; i++){
                keeper_fight_pos[i].x = anim_back_size.width + keeper_fight_pos[i].x;
        }
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
        
        FightResultData* result = _theGameLogic->startPlayerAttack(cell_id);
        
        if (nullptr != result){
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

void GameScene::afterPlayerBattle(FightResultData* result){
        _isPalyingAnim = false;
        
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
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

void GameScene::afterRobootBattle(FightResultData* result){
        _isPalyingAnim = false;
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
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
        FightResultData* res_data = _theGameLogic->startRobootAttack();
        if (nullptr == res_data || res_data->_result == ATTACK_RES_NONE){
                
                _gameStatus = GAME_STATUS_INUSERTURN;
                _endTurnMenuItem->setVisible(true);
                return;
        }
        
        if (ATTACK_RES_WIN == res_data->_result || ATTACK_RES_DEFEATED == res_data->_result){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootBattle, this, res_data));
//                this->playRobbotBattleAnimation(res, callback);
                this->playManualBattleAnimation(res_data, callback);
                
        }else if(ATTACK_RES_GOTSUPPLY == res_data->_result){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootSupply, this));
                this->playSupplyAnimation(callback);
        }
}

void GameScene::afterFightFinished(FightResultData* resut_data, CallFunc* cb){
        
        //TODO:: accupy area.
        
        auto hide = ScaleBy::create(1.0f, .1f);
        _animationLayer->runAction(Sequence::create(hide,
                CallFunc::create( [&](){
                _animationLayer->setVisible(false);
        }), cb, NULL));
}


void GameScene::WinnerBack(FightResultData* res_data, CallFunc* cb){
        
        Size visible_size = Director::getInstance()->getVisibleSize();
        auto cache = AnimationCache::getInstance();
        auto when_back_home = CallFunc::create(std::bind(&GameScene::afterFightFinished, this,res_data, cb));
        
        int player_uid = ATTACK_RES_WIN == res_data->_result ? res_data->_fromPlayer : res_data->_toPlayer;
        auto run_back_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid]);
        run_back_anim->setRestoreOriginalFrame(true);
        auto run_back_act = Animate::create(run_back_anim);
        
        auto back_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid]);
        back_wait->setRestoreOriginalFrame(true);
        
        
        if (ATTACK_RES_WIN == res_data->_result){
                
                int keeper_uid = res_data->_toPlayer;
                for (int i = 0; i < res_data->_to.size(); i++){
                        auto keeper = _allFightingCharacters[keeper_uid][i];
                        keeper->setVisible(false);
                }
                
                auto moveby = MoveBy::create(2, Vec2(READY_DISTANCE_POS - visible_size.width / 2,0));
                auto run_back = Spawn::create(run_back_act, FlipX::create(true), moveby, NULL);
                
                auto fight_back = Animate::create(back_wait);
                
                auto move = MoveBy::create(2, Vec2(-READY_DISTANCE_POS,0));
                Spawn* back_home = Spawn::create(run_back_act->clone(), move, NULL);
                
                Sequence* invade_back = Sequence::create(run_back, fight_back,  back_home, NULL);
                
                for (int i = 0; i < res_data->_from.size(); i++){
                        auto invader = _allFightingCharacters[player_uid][i];
                        
                        if (i == 0){
                                auto s_with_cc = Sequence::create(run_back, fight_back,  back_home, when_back_home, NULL);
                                invader->runAction(s_with_cc);
                        }else{
                                invader->runAction(invade_back->clone());
                        }
                }
        }else{
                
                int invader_uid = res_data->_fromPlayer;
                for (int i = 0; i < res_data->_from.size(); i++){
                        auto invader = _allFightingCharacters[invader_uid][i];
                        invader->setVisible(false);
                }
                
                auto moveby = MoveBy::create(2, Vec2(visible_size.width / 2 - READY_DISTANCE_POS, 0));
                auto run_back = Spawn::create(run_back_act, FlipX::create(false), moveby, NULL);
                
                auto fight_back = Animate::create(back_wait);
                
                auto move = MoveBy::create(2, Vec2(READY_DISTANCE_POS, 0));
                Spawn* back_home = Spawn::create(run_back_act->clone(), move, NULL);
                
                Sequence* keeper_back = Sequence::create(run_back, fight_back, back_home, NULL);

                for (int i = 0; i < res_data->_to.size(); i++){
                        auto keeper = _allFightingCharacters[player_uid][i];
                        
                        if (0 == i){
                                auto s_with_cc = Sequence::create(run_back, fight_back,  back_home, when_back_home, NULL);
                                keeper->runAction(s_with_cc);
                        }else{
                                keeper->runAction(keeper_back->clone());
                        }
                }
        }
}

void GameScene::Fighting(FightResultData* resut_data, CallFunc* cb){
        _diceResultLayer->setVisible(true);
        
        
        auto back_size = _diceResultLayer->getContentSize();
        for (int i = 0; i < resut_data->_from.size(); i++){
                int value = resut_data->_from[i];
                auto dice = Sprite::create(StringUtils::format("dice_%d.png", value));
                dice->setPosition(Vec2(50 * i + 40, back_size.height / 2 - 120));
                _diceResultLayer->addChild(dice);
        }
        
        for (int i = 0; i < resut_data->_to.size(); i++){
                int value = resut_data->_to[i];
                auto dice = Sprite::create(StringUtils::format("dice_%d.png", value));
                dice->setPosition(Vec2(back_size.width - 50 * i - 40, back_size.height / 2 - 120));
                _diceResultLayer->addChild(dice);
        }
        
        auto from_value = Label::createWithSystemFont(StringUtils::format("%d", resut_data->_fromSum), "", 78);
        from_value->setColor(Color3B::RED);
        from_value->setPosition(Vec2(back_size.width / 2 - 100, back_size.height / 2 + 120));
        _diceResultLayer->addChild(from_value);
        
        auto to_value = Label::createWithSystemFont(StringUtils::format("%d", resut_data->_toSum), "", 78);
        to_value->setColor(Color3B::RED);
        to_value->setPosition(Vec2(back_size.width / 2 + 100, back_size.height / 2 + 120));
        _diceResultLayer->addChild(to_value);
        
        auto winner_back = CallFunc::create(std::bind(&GameScene::WinnerBack, this, resut_data, cb));
        
        //TODO:: use this as  cloud animation. need to be replaced.
        _diceResultLayer->runAction(Sequence::create(RotateBy::create(2, 360 * 2), winner_back, NULL));
}

void GameScene::afterShowFightBg(FightResultData* res_data, CallFunc* cb){
        Size visible_size = Director::getInstance()->getVisibleSize();
        auto cache = AnimationCache::getInstance();
        
        //FIGHTER
        {
                int player_uid = res_data->_fromPlayer;
        
                auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid]);
                run_anim->setRestoreOriginalFrame(true);
                auto run_action = Animate::create(run_anim);
                auto move = MoveBy::create(2, Vec2(READY_DISTANCE_POS,0));
                Spawn* get_ready = Spawn::create(run_action, FlipX::create(false), move, NULL);
                
                
                
                auto stand_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid]);
                stand_wait->setRestoreOriginalFrame(true);
                auto fight_wait = Animate::create(stand_wait);
                
                
                auto moveby = MoveBy::create(2, Vec2(visible_size.width / 2 -READY_DISTANCE_POS,0));
                auto run_to_fight = Spawn::create(run_action->clone(), moveby, NULL);
                
        
                Sequence* invade_seq = Sequence::create(get_ready, fight_wait,  run_to_fight, NULL);
                
                for (int i = 0; i < res_data->_from.size(); i++){
                        
                        auto invader = _allFightingCharacters[res_data->_fromPlayer][i];
                        invader->setVisible(true);
                        invader->setPosition(invader_fight_pos[i] - Vec2(READY_DISTANCE_POS, 0));
                        
                        if (i == 0){
                                auto fighting = CallFunc::create(std::bind(&GameScene::Fighting, this,res_data, cb));
                                auto s_with_cc = Sequence::create(get_ready, fight_wait,  run_to_fight, fighting, NULL);
                                invader->runAction(s_with_cc);
                        }else{
                                invader->runAction(invade_seq->clone());
                        }
                }
        }
        
        
        //KEEPER
        {
                int player_uid = res_data->_toPlayer;
                
                auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid]);
                run_anim->setRestoreOriginalFrame(true);
                auto run_act = Animate::create(run_anim);
                
                auto move = MoveBy::create(2, Vec2(-READY_DISTANCE_POS,0));
                Spawn* get_ready = Spawn::create(run_act, FlipX::create(true), move, NULL);
                
                
                auto stand_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid]);
                stand_wait->setRestoreOriginalFrame(true);
                auto fight_wait = Animate::create(stand_wait);
                
                
                auto moveby = MoveBy::create(2, Vec2(READY_DISTANCE_POS - visible_size.width / 2 ,0));
                auto run_to_fight = Spawn::create(run_act, moveby, NULL);
                
                Sequence* keeper_seq = Sequence::create(get_ready, fight_wait, run_to_fight, NULL);
                
                for (int i = 0; i < res_data->_to.size(); i++){
                        auto keeper = _allFightingCharacters[player_uid][i];
                        keeper->setVisible(true);
                        keeper->setPosition(keeper_fight_pos[i] + Vec2(READY_DISTANCE_POS, 0));
                        keeper->runAction(keeper_seq->clone());
                }
        }
}

void GameScene::playRobbotBattleAnimation(FightResultData* res_data, CallFunc* callback){
        //TODO::replace this animation by a occupation animation
        auto show = ScaleBy::create(1.0f, 1.0f);
        _animationLayer->runAction(Sequence::create(show, callback, NULL));
}

void GameScene::playManualBattleAnimation(FightResultData* resut_data, CallFunc* callback){
        if (ATTACK_RES_WIN == resut_data->_result){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_WIN);
        }else if (ATTACK_RES_DEFEATED == resut_data->_result){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_DEFEAT);
        }
        
        bool is_anim_on = UserDefault::getInstance()->getBoolForKey(ANIMATION_SWITCH_KEY, true);
        if (is_anim_on){
                _isPalyingAnim = true;
                auto show = ScaleBy::create(.5f, 10.0f);
                _animationLayer->setVisible(true);
                
                callback->retain();
                resut_data->retain();
                auto cc = CallFunc::create(std::bind(&GameScene::afterShowFightBg, this,resut_data, callback));
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
