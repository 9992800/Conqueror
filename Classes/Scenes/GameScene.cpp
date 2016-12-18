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
#include "ui/CocosGUI.h" 

enum{
        ZORDER_BACK_GROUND = 0,
        ZORDER_MAP_GROUND,
        ZORDER_CRTL_LAYERS,
        ZORDER_ANIM_LAYER,
        ZORDER_DICE_LAYER,
        ZORDER_DIALOG_LAYER
};
enum{
        key_map_back_layer = 1,
        key_ctrl_layer_tag,
        key_anim_layer_tag,
        key_dice_layer_tag,
        key_dialog_layer_tag,
        key_roll_show_tag,
        key_operate_board_tag_l,
        key_operate_board_tag_m,
        key_operate_board_tag_r
};


#pragma mark - constructor
#define MAP_SCALE_V    1.6f
#define MAP_BACK_SIZE  2.0f

int GameScene::_playerNumber = 2;
int GameScene::_charactorIdx = 0;
int GameScene::_colorIdx = 0;
Scene* GameScene::createScene(int gameLevel, int charactorIdx, int colorIdx)
{
        auto scene = Scene::create();
        _playerNumber = gameLevel;
        _charactorIdx = charactorIdx;
        _colorIdx = colorIdx;
        auto layer = GameScene::create();
        scene->addChild(layer); 
        return scene;
}

GameScene::~GameScene(){
        _theGameLogic->release();
}


bool GameScene::init()
{
        if (!Layer::init()){
                return false;
        }
        _theGameLogic = DiceGame::create();
        _theGameLogic->retain();
        
        _curGameData = _theGameLogic->initGameData(_playerNumber);
        _curGameData->initPlayerChAndColor(_charactorIdx, _colorIdx);
       
        this->initControlLayer();
        this->initMapSize(_curGameData);
        this->initAnimationLayer();
        
        return true;
}

#pragma mark - initilization

void GameScene::initMapSize(GameData* data){
        _gameStatus = GAME_STATUS_INIT;
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        _mapLayer = LayerColor::create();
        _mapLayer->changeWidthAndHeight(_showAreaSize.width ,
                                        _showAreaSize.height);
        _mapLayer->setIgnoreAnchorPointForPosition(false);
        _mapLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        ScreenCoordinate::getInstance()->configScreen(_showAreaSize);
        
        data->reshDataByBackGrnd(_mapLayer);
        _mapLayer->setPosition(visibleSize);
        
        auto back_ground = LayerColor::create(TILE_COLOR_BACKGRUND, visibleSize.width * MAP_BACK_SIZE , visibleSize.height * MAP_BACK_SIZE);
        back_ground->addChild(_mapLayer);
        back_ground->setIgnoreAnchorPointForPosition(false);
        back_ground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        back_ground->setPosition(visibleSize / 2);
        this->addChild(back_ground, ZORDER_MAP_GROUND, key_map_back_layer);
        
        _minFrameShow = Rect(0, visibleSize.height - 30, 0, 60);
        float gap = (MAP_SCALE_V - 1) ;
        _maxFrameShow = Rect(visibleSize.width * (1 - gap * 0.5), visibleSize.height * (1 - gap * 0.5), visibleSize.width * gap * 2, visibleSize.height * gap * 2);
}


void GameScene::initAreaTcShow(){
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto player_0 = cocos2d::ui::ImageView::create("maps/supply_back_0.png");
        auto p_size = player_0->getContentSize();
        auto roll = cocos2d::ui::Scale9Sprite::create("maps/supply_back_roll.png");
        
        auto bakc_size = Size(42 + p_size.width * _playerNumber,
                              roll->getContentSize().height);
        roll->setContentSize(bakc_size);
        roll->setCapInsets(Rect(21, 6, 27, 14));
        roll->setPosition(Vec2(visible_size.width / 2,
                               visible_size.height - bakc_size.height/2));
        
        for (int i = 0; i < _playerNumber; i++){
                int player_uid = _curGameData->_jun[i];
                GamePlayer* player = _curGameData->_player[player_uid];
                std::string p_f_i = player->getFlagImge();
                auto p = Sprite::create(p_f_i);
                p->setPosition(Vec2(21 +  0.5 * (i * 2 + 1) * p_size.width,  roll->getContentSize().height - p_size.height / 2));
                p->setTag(player_uid);
                roll->addChild(p);
                
                int chara_idx = player->getPosCharactorIdx();
                std::string charactr_name =   CHARACTER_NAME[chara_idx];
                auto character = Sprite::create(charactr_name);
                character->setScale(0.7);
                auto ch_size = character->getContentSize();
                
                std::string tc_str = StringUtils::format("X%d", player->getAreaTc());
                auto numbser = Label::createWithSystemFont(tc_str, "arial", 26);
                character->addChild(numbser);
                numbser->setPosition(Vec2(ch_size.width, ch_size.height / 2));
                
                character->setPosition(p_size.width * 0.4,  p_size.height * 0.7);
                p->addChild(character);
                
                _supplyLabelMap.insert(std::pair<int, Label*>(player_uid, numbser));
        }
        
        _controlLayer->addChild(roll, ZORDER_MAP_GROUND, key_roll_show_tag);
}

void GameScene::initOperateBoard(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto operat_board_m = Sprite::create("maps/openrate_back_m.png");
        operat_board_m->setPosition(Vec2(visibleSize.width / 2, operat_board_m->getContentSize().height / 2));
        _controlLayer->addChild(operat_board_m, ZORDER_MAP_GROUND, key_operate_board_tag_m);
        
        _showAreaSize.height -= operat_board_m->getContentSize().height;
        
        auto operat_board_l = Sprite::create("maps/openrate_back_l.png");
        operat_board_l->setPosition(operat_board_l->getContentSize() / 2);
        _controlLayer->addChild(operat_board_l, ZORDER_MAP_GROUND, key_operate_board_tag_l);
        
        auto operat_board_r = Sprite::create("maps/openrate_back_r.png");
        operat_board_r->setPosition(Vec2(visibleSize.width - operat_board_r->getContentSize().width / 2,
                                         operat_board_r->getContentSize().height / 2));
        _controlLayer->addChild(operat_board_r, ZORDER_MAP_GROUND, key_operate_board_tag_r);
        
       
        
        auto first_tip_layer = Layer::create();
        first_tip_layer->setContentSize(operat_board_m->getContentSize());
        first_tip_layer->setPosition(operat_board_m->getContentSize() / 2);
        first_tip_layer->setIgnoreAnchorPointForPosition(false);
        first_tip_layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        auto tips = Label::createWithSystemFont("Do you want to use this map?", "", 40);
        tips->setPosition(Vec2(tips->getContentSize().width / 2 + 20,
                               first_tip_layer->getContentSize().height / 2));
        first_tip_layer->addChild(tips);
        
        operat_board_m->addChild(first_tip_layer);
        
        auto OK_btn = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        OK_btn->cocos2d::Node::setScale(1.2f);
        OK_btn->setTitleText("YES");
        OK_btn->setTitleFontSize(24);
        OK_btn->addClickEventListener(CC_CALLBACK_1(GameScene::menuStartGame, this, first_tip_layer));
        OK_btn->setPosition(Vec2(tips->getContentSize().width + 48 + OK_btn->getContentSize().width / 2,
                                 operat_board_m->getContentSize().height / 2));
        first_tip_layer->addChild(OK_btn);
        
        
        auto NO_btn = cocos2d::ui::Button::create("DIALOG_CANCEL.png", "DIALOG_CANCEL_SEL.png");
        NO_btn->cocos2d::Node::setScale(1.2f);
        NO_btn->setTitleText("NO");
        NO_btn->setTitleFontSize(24);
        NO_btn->setPosition(Vec2(OK_btn->getPosition().x + 48 + NO_btn->getContentSize().width,
                            operat_board_m->getContentSize().height / 2));
        NO_btn->addClickEventListener(CC_CALLBACK_1(GameScene::createNewMap, this));
        first_tip_layer->addChild(NO_btn);
        
        
        
        _endTurnTipsLayer = Layer::create();
        _endTurnTipsLayer->setContentSize(operat_board_m->getContentSize());
        _endTurnTipsLayer->setPosition(operat_board_m->getContentSize() / 2);
        _endTurnTipsLayer->setIgnoreAnchorPointForPosition(false);
        _endTurnTipsLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _endTurnTipsLayer->setVisible(false);
        
        
        auto attack_tips = Label::createWithSystemFont("1.Click your area. 2.Click neighbor to attack", "", 30);
        attack_tips->setPosition(Vec2(attack_tips->getContentSize().width / 2 + 20,
                               _endTurnTipsLayer->getContentSize().height / 2));
        _endTurnTipsLayer->addChild(attack_tips);
        
        operat_board_m->addChild(_endTurnTipsLayer);
        
        auto end_turn_btn = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        end_turn_btn->cocos2d::Node::setScale(1.2f);
        end_turn_btn->setTitleText("END TURN");
        end_turn_btn->addClickEventListener(CC_CALLBACK_1(GameScene::menuEndTurn, this));
        end_turn_btn->setPosition(Vec2(operat_board_m->getContentSize().width - 48 - end_turn_btn->getContentSize().width / 2,
                                 operat_board_m->getContentSize().height / 2));
        _endTurnTipsLayer->addChild(end_turn_btn);
}

void GameScene::initControlLayer(){
        
        
        _animationIsOn = UserDefault::getInstance()->getBoolForKey(ANIMATION_SWITCH_KEY, true);
        int game_speed = UserDefault::getInstance()->getIntegerForKey(GAME_SPEED_KEY, 1);
        
        Director::getInstance()->getScheduler()->setTimeScale(game_speed);
        
        _showAreaSize = Director::getInstance()->getVisibleSize();
        
        _controlLayer = Layer::create();
        
        this->initOperateBoard();
        
        auto player_0 = cocos2d::ui::ImageView::create("maps/supply_back_0.png");
        auto p_size = player_0->getContentSize();
        _showAreaSize.height -= p_size.height;
        _showAreaSize.width *= 0.9;
        
        this->addChild(_controlLayer, ZORDER_CRTL_LAYERS, key_ctrl_layer_tag);
        
        Director::getInstance()->setDepthTest(true);
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::loadDiceResultLayer(){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto anim_back_size = _animationLayer->getContentSize();
        _diceResultLayer = Layer::create();
        auto dice_size = Size(anim_back_size.width, anim_back_size.height / 3);
        _diceResultLayer->setContentSize(dice_size);
        _diceResultLayer->setPosition(Vec2((visibleSize - dice_size) / 2));
        this->addChild(_diceResultLayer, ZORDER_DICE_LAYER, key_dice_layer_tag);
        _diceResultLayer->setVisible(false);
        
        float scale_factor = Director::getInstance()->getContentScaleFactor();
        for (int i = 0; i < MAX_PLAYER; i++){
                _invaderPos[i]  = invader_pos[i] * (1.f / scale_factor);
                _keeperPos[i]   = keeper_pos[i] * (1.f / scale_factor);
                _keeperPos[i].x = anim_back_size.width + _keeperPos[i].x;
        }
        
        READY_DISTANCE_POS *= (1.f / scale_factor);
}

void GameScene::loadCharact(int idx, std::string ch_name){
        auto frameCache = SpriteFrameCache::getInstance();
        
        auto frame = frameCache->getSpriteFrameByName(ch_name);
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto charactor = Sprite::create();
                charactor->setSpriteFrame(frame);
                _allFightingCharacters[idx][i] = charactor;
                charactor->setVisible(false);
                _animationLayer->addChild(charactor);
        }
}


void GameScene::initAnimationLayer(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        _animationLayer = Sprite::create("zhandou_beijing.png");
        _animationLayer->setScale(0.1f, 0.1f);
        
        
        _animationLayer->setPosition(Vec2(visibleSize / 2));
        _animationLayer->setVisible(false);
        this->addChild(_animationLayer, ZORDER_ANIM_LAYER, key_anim_layer_tag);
        
        this->loadDiceResultLayer();
        
        this->loadCharact(PLAYER_ROLE_TYPE_ZHANSHI, "renwurun0001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_XUNSHOUSHI, "xssrun0001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_QISHI, "xssrun0001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_GONGJIANSHOU, "xssrun0001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_PAOSHOU, "xssrun0001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_SHOUREN, "xssrun0001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_MONV, "xssrun0001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_DAOZEI, "xssrun0001.png");
        
        this->loadCharact(FIGHT_ANIM_TYPE_XINYUN, "yw0001.png");
}
#pragma mark - touch and menu event

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
        
        auto touch = touches[0];
        auto diff = touch->getDelta();

        if (diff.x >= 0.001f || diff.y >= 0.001
            || diff.x <= -0.001f ||diff.y <= -0.001f){
                _isMoved = true;
        }
        
        auto currentPos = _mapLayer->getPosition();
        
        if (GAME_STATUS_INUSERTURN == _gameStatus){
                 if (_maxFrameShow.getMaxY() < (currentPos.y + diff.y)
                     || (currentPos.y + diff.y) < _maxFrameShow.getMinY()){
                         diff.y = 0;
                 }
        
                if (_maxFrameShow.getMaxX() < (currentPos.x + diff.x)
                    || (currentPos.x + diff.x) < _maxFrameShow.getMinX()){
                        diff.x = 0;
                }
        }else{
                diff.x = 0;
                printf("=(%2.f, %2.f)=", _minFrameShow.getMaxY(), _minFrameShow.getMinY());
                if (_minFrameShow.getMaxY() < (currentPos.y + diff.y)
                    || (currentPos.y + diff.y) < _minFrameShow.getMinY()){
                        diff.y = 0;
                }
        }
        
       
        
        _mapLayer->setPosition(currentPos + diff);
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *event){
        
        if (_isMoved || _isPalyingAnim
            ||_gameStatus != GAME_STATUS_INUSERTURN) {
                _isMoved = false;
                return;
        }
        
        auto touch = touches[0];
        auto position = touch->getLocation();
        Vec2 pos_in_map = _mapLayer->convertToNodeSpace(position);
        
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
                this->playBattleAnimation(result, callback, true);
        }
}

void GameScene::refreshSupplyDiceNum(){
        for (std::map<int, Label*>::iterator it = _supplyLabelMap.begin();
             it != _supplyLabelMap.end(); it++){
                int p_uid = it->first;
                std::string s_u = StringUtils::format("X%d",
                                                      _curGameData->_player[p_uid]->getAreaTc());
                it->second->setString(s_u);
        }
}

void GameScene::tryAgain(){
        this->removeChildByTag(key_map_back_layer);
        
        _curGameData = _theGameLogic->resetInitData();
        this->initMapSize(_curGameData);
        
        this->refreshSupplyDiceNum();
}

#pragma mark - animation 

void GameScene::afterPlayerBattle(FightResultData* result){
        std::map<int, int> survival = _theGameLogic->cleanUpBattleField(result);
        
        _isPalyingAnim = false;
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
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
        this->refreshAreaTcShow(survival);
}

void GameScene::afterRobootBattle(FightResultData* result){
        std::map<int, int> survival = _theGameLogic->cleanUpBattleField(result);
        
        _isPalyingAnim = false;
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
        
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
        
        this->refreshAreaTcShow(survival);
        this->gameAction();
}


void GameScene::refreshAreaTcShow(std::map<int, int> survival){
        bool need_to_repaint = false;
        for (std::map<int, Label*>::iterator it = _supplyLabelMap.begin();
             it != _supplyLabelMap.end(); it++){
                
                std::map<int, int>::iterator it_tc = survival.find(it->first);
                if (survival.end() == it_tc){
                        need_to_repaint = true;
                        continue;
                }
                std::string tc_num_str = StringUtils::format("X%d", it_tc->second);
                it->second->setString(tc_num_str);
        }
        
       
        
        if (need_to_repaint){
                
                cocos2d::ui::Scale9Sprite* roll = (cocos2d::ui::Scale9Sprite*)_controlLayer->getChildByTag(key_roll_show_tag);
                Vector<Node*> flag_nodes = roll->getChildren();
                auto p_size = flag_nodes.at(0)->getContentSize();
                
                auto bakc_size = Size(42 + p_size.width * survival.size(),
                                      roll->getContentSize().height);
                roll->setContentSize(bakc_size);
                
                for (int i = 0; i < flag_nodes.size(); i++){
                        auto flag_obj = flag_nodes.at(i);
                        int player_uid =  flag_obj->getTag();
                        GamePlayer* player = _curGameData->_player[player_uid];
                        if (player->getAreaTc() == 0){
                                roll->removeChild(flag_obj);
                                _supplyLabelMap.erase(player_uid);
                        }
                }
                
                flag_nodes = roll->getChildren();
                for (int i = 0; i < flag_nodes.size(); i++){
                        Node* one_flag = flag_nodes.at(i);
                        one_flag->setPosition(Vec2(21 +  0.5 * (i * 2 + 1) * p_size.width,  roll->getContentSize().height - p_size.height / 2));
                }
        }
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
                _mapLayer->setScale(MAP_SCALE_V);
                _endTurnTipsLayer->setVisible(true);
                _gameStatus = GAME_STATUS_INUSERTURN;
                return;
        }
        
        if (ATTACK_RES_WIN == res_data->_result || ATTACK_RES_DEFEATED == res_data->_result){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootBattle, this, res_data));
                this->playBattleAnimation(res_data, callback, false);
                
        }else if(ATTACK_RES_GOTSUPPLY == res_data->_result){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootSupply, this));
                this->playSupplyAnimation(callback);
        }
}

void GameScene::afterFightFinished(FightResultData* resut_data, CallFunc* cb){
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                _allFightingCharacters[resut_data->_fromPlayer][i]->setVisible(false);
                _allFightingCharacters[resut_data->_toPlayer][i]->setVisible(false);
        }
        _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0]->setVisible(false);
        
        auto occupay_cc = CallFunc::create(std::bind(&DiceGame::occupayAnimation, _theGameLogic, resut_data, cb));
        
        auto hide = ScaleTo::create(1.0f, .1f);
        _animationLayer->runAction(Sequence::create(hide,
                CallFunc::create( [&](){
                _animationLayer->setVisible(false);
        }), occupay_cc, NULL));
}


void GameScene::WinnerBack(FightResultData* res_data, CallFunc* cb){
        
        this->ShowResultData(res_data);
        
        if (ATTACK_RES_WIN == res_data->_result){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_WIN);
        }else if (ATTACK_RES_DEFEATED == res_data->_result){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_DEFEAT);
        }
        
        
        _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0]->setVisible(false);
        
        Size back_size = _animationLayer->getContentSize();
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
                
                auto moveby = MoveBy::create(2, Vec2(READY_DISTANCE_POS - back_size.width / 2,0));
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
                
                auto moveby = MoveBy::create(2, Vec2(back_size.width / 2 - READY_DISTANCE_POS, 0));
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

void GameScene::ShowResultData(FightResultData* resut_data){
        _diceResultLayer->setVisible(true);
        auto back_size = _diceResultLayer->getContentSize();
        auto frameCache = SpriteFrameCache::getInstance();
        
        
        for (int i = 0; i < resut_data->_from.size(); i++){
                int value = resut_data->_from[i];
                auto str = DICE_PIC_NAME_STR[resut_data->_fromPlayer][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                dice->setPosition(Vec2(dice_size.width * (i + 1), dice_size.height));
                _diceResultLayer->addChild(dice);
        }
        
        for (int i = 0; i < resut_data->_to.size(); i++){
                int value = resut_data->_to[i];
                auto str = DICE_PIC_NAME_STR[resut_data->_toPlayer][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                dice->setPosition(Vec2(back_size.width - dice_size.width * (i + 1),  dice_size.height));
                _diceResultLayer->addChild(dice);
        }
        
        auto from_value = Label::createWithSystemFont(StringUtils::format("%d", resut_data->_fromSum), "", 78);
        from_value->setColor(Color3B::BLUE);
        from_value->setPosition(Vec2(back_size.width / 2 - 100, back_size.height - 20 ));
        _diceResultLayer->addChild(from_value);
        
        auto to_value = Label::createWithSystemFont(StringUtils::format("%d", resut_data->_toSum), "", 78);
        to_value->setColor(Color3B::BLUE);
        to_value->setPosition(Vec2(back_size.width / 2 + 100, back_size.height - 20));
        _diceResultLayer->addChild(to_value);
}

void GameScene::Fighting(FightResultData* resut_data, CallFunc* cb){
        
        auto winner_back = CallFunc::create(std::bind(&GameScene::WinnerBack, this, resut_data, cb));
        
        auto fight_cloud = _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0];
        fight_cloud->setVisible(true);
        
        auto size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        
        fight_cloud->setPosition(Vec2(size.width / 2, size.height * 0.4));
        fight_cloud->setScale(2.0f);
        auto anim_cloud = cache->getAnimation("finght_cloud");
        anim_cloud->setRestoreOriginalFrame(true);
        auto anim_action = Animate::create(anim_cloud);
        
        fight_cloud->runAction(Sequence::create(anim_action, winner_back, NULL));
}

void GameScene::afterShowFightBg(FightResultData* res_data, CallFunc* cb){
        Size back_size = _animationLayer->getContentSize();
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
                
                
                auto moveby = MoveBy::create(2, Vec2(back_size.width / 2 - READY_DISTANCE_POS,0));
                auto run_to_fight = Spawn::create(run_action->clone(), moveby, NULL);
                
        
                Sequence* invade_seq = Sequence::create(get_ready, fight_wait,  run_to_fight, NULL);
                
                for (int i = 0; i < res_data->_from.size(); i++){
                        
                        auto invader = _allFightingCharacters[res_data->_fromPlayer][i];
                        invader->setVisible(true);
                        invader->setPosition(_invaderPos[i] - Vec2(READY_DISTANCE_POS, 0));
                        
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
                
                
                auto moveby = MoveBy::create(2, Vec2(READY_DISTANCE_POS - back_size.width / 2 ,0));
                auto run_to_fight = Spawn::create(run_act, moveby, NULL);
                
                Sequence* keeper_seq = Sequence::create(get_ready, fight_wait, run_to_fight, NULL);
                
                for (int i = 0; i < res_data->_to.size(); i++){
                        auto keeper = _allFightingCharacters[player_uid][i];
                        keeper->setVisible(true);
                        keeper->setPosition(_keeperPos[i] + Vec2(READY_DISTANCE_POS, 0));
                        keeper->runAction(keeper_seq->clone());
                }
        }
}

void GameScene::playBattleAnimation(FightResultData* resut_data, CallFunc* callback, bool isMaunual){
        
        _isPalyingAnim = true;
        if (_animationIsOn && isMaunual){
                _animationLayer->setVisible(true);
                auto show = ScaleTo::create(.5f, 1.0f);
                callback->retain();
                resut_data->retain();
                auto cc = CallFunc::create(std::bind(&GameScene::afterShowFightBg, this, resut_data, callback));
                _animationLayer->runAction(Sequence::create(show, cc, NULL));
                
        }else{
                this->ShowResultData(resut_data);
                callback->retain();
                resut_data->retain();
                this->afterFightFinished(resut_data, callback); 
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
        auto visible_size = Director::getInstance()->getVisibleSize();
        _mapLayer->setScale(1.f);
        _mapLayer->setPosition(visible_size);
        _endTurnTipsLayer->setVisible(false);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
        
        _theGameLogic->clearManulAction();
        _gameStatus = GAME_STATUS_AIRUNNING;
        CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootSupply, this));
        this->playSupplyAnimation(callback);
}

void GameScene::createNewMap(Ref* pSender){
        _curGameData->release();
        _curGameData = _theGameLogic->initGameData(_playerNumber);
        _curGameData->initPlayerChAndColor(_charactorIdx, _colorIdx);
        this->removeChildByTag(key_map_back_layer);
        this->initMapSize(_curGameData);
        
        this->refreshSupplyDiceNum();
}

void GameScene::menuStartGame(Ref* pSender, Layer* parent){
        this->initAreaTcShow();
        _theGameLogic->initHistoryRecord();
        parent->setVisible(false);
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

void GameScene::menuAnimSwitch(Ref* btn){
        auto label = (Label*)((Node*)btn)->getChildByTag(111);
        if (_animationIsOn){
                _animationIsOn = false;
                label->setString("打开动画");
                _animationLayer->setVisible(false);
        }else{
                label->setString("关闭动画");
                _animationIsOn = true;
        }
        
        UserDefault::getInstance()->setBoolForKey(ANIMATION_SWITCH_KEY, _animationIsOn);
        UserDefault::getInstance()->flush();
}
