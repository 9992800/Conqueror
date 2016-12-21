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
        ZORDER_DICE_LAYER,
        ZORDER_DIALOG_LAYER
};
enum{
        key_map_back_layer = 1,
        key_ctrl_layer_tag,
        key_anim_layer_tag,
        key_dice_layer_tag,
        key_supply_layer_tag,
        key_dialog_layer_tag,
        key_roll_show_tag,
        key_operate_board_tag_l,
        key_operate_board_tag_m,
        key_operate_board_tag_r
};


#pragma mark - constructor
#define MAP_SCALE_V    1.4f
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
        //TODO::_allFightingCharacters->release
}


bool GameScene::init()
{
        if (!Layer::init()){
                return false;
        }
        _gameStatus = GAME_STATUS_INIT;
        _theGameLogic = DiceGame::create();
        _theGameLogic->retain();
        
        _curGameData = _theGameLogic->initGameData(_playerNumber, _charactorIdx, _colorIdx); 
       
        this->initControlLayer();
        this->initMapSize(_curGameData);
        this->initAnimationLayer();
        
        return true;
}

#pragma mark - initilization

void GameScene::initMapSize(GameData* data){
        
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
        _maxFrameShow = Rect(visibleSize.width * (1 - gap * 0.5), visibleSize.height * (1 - gap * 0.5), visibleSize.width * gap, visibleSize.height * gap);
}


void GameScene::initAreaTcShow(){
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto player_0 = cocos2d::ui::ImageView::create("maps/supply_back_0.png");
        auto p_size = player_0->getContentSize();
        auto roll = cocos2d::ui::Scale9Sprite::create("maps/supply_back_roll.png");
        _supplyLabelMap = std::map<int, Label*>();
        
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
                roll->addChild(p, 1);
                if ( i == 0){
                        _curInTurnBack = Sprite::create("maps/current_turn.png");
                        _curInTurnBack->setPosition(p->getPosition());
                        _curInTurnBack->setTag(-1);
                        roll->addChild(_curInTurnBack, 2);
                }
                
                int chara_idx = player->getPosCharactorIdx();
                std::string charactr_name =   CHARACTER_NAME[chara_idx];
                auto character = Sprite::create(charactr_name);
                character->setScale(0.7);
                auto ch_size = character->getContentSize();
                
                std::string tc_str = StringUtils::format("X%d", player->getAreaTc());
                auto numbser = Label::createWithSystemFont(tc_str, "Arial", 26);
                character->addChild(numbser);
                numbser->setPosition(Vec2(ch_size.width, ch_size.height / 2));
                
                character->setPosition(p_size.width * 0.4,  p_size.height * 0.7);
                p->addChild(character);
                
                _supplyLabelMap.insert(std::pair<int, Label*>(player_uid, numbser));
                if (player_uid == _curGameData->_userId){
                        _curPlayerSupFlag = p;
                }
        }
        _controlLayer->addChild(roll, ZORDER_MAP_GROUND, key_roll_show_tag);
}

void GameScene::initOperateBoard(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        
        auto operat_board = Sprite::create("maps/openrate_back.png");
        operat_board->setPosition(Vec2(visibleSize.width / 2, operat_board->getContentSize().height / 2));
        _controlLayer->addChild(operat_board, ZORDER_MAP_GROUND, key_operate_board_tag_m);
        
        _showAreaSize.height -= operat_board->getContentSize().height;
        
        auto operat_board_m = Sprite::create("maps/openrate_board_m.png");
        operat_board_m->setPosition(Vec2(operat_board->getContentSize().width / 2,
                                         operat_board_m->getContentSize().height * 0.6));
        operat_board->addChild(operat_board_m, 1);
        
        auto board_v = Sprite::create("maps/openrate_board_v.png");
        board_v->setPosition(operat_board->getContentSize() / 2);
        operat_board->addChild(board_v, 2);
        
        auto operat_board_l = Sprite::create("maps/openrate_board_btn.png");
        operat_board_l->setPosition(operat_board_l->getContentSize() * 0.6);
        operat_board->addChild(operat_board_l, 1);

        _animationIsOn = UserDefault::getInstance()->getBoolForKey(ANIMATION_SWITCH_KEY, true);
        _animCtlBtn = cocos2d::ui::Button::create();
        if (_animationIsOn)
                _animCtlBtn->loadTextureNormal("maps/open_anim.png");
        else
                _animCtlBtn->loadTextureNormal("maps/close_anim.png");
        
        _animCtlBtn->addClickEventListener(CC_CALLBACK_1(GameScene::menuAnimSwitch, this));
        _animCtlBtn->setPosition(operat_board_l->getContentSize() / 2);
        operat_board_l->addChild(_animCtlBtn);

        auto operat_board_r = Sprite::create("maps/openrate_board_btn.png");
        operat_board_r->setPosition(Vec2(operat_board->getContentSize().width - operat_board_r->getContentSize().width * 0.6,
                                         operat_board_r->getContentSize().height * 0.6));
        operat_board->addChild(operat_board_r, 1);
        
        _addtionalSupplyTimes = ADDTIONAL_SUPPLY_TIME_PER_GAME;
        _addArmyBtn = cocos2d::ui::Button::create("maps/addtion_supply_arm.png",
                                                  "maps/addtion_supply_arm.png",
                                                  "maps/supply_finished.png");
        _addArmyBtn->setPosition(operat_board_r->getContentSize() / 2);
        _addArmyBtn->addClickEventListener(CC_CALLBACK_1(GameScene::menuAddArmy, this));
        operat_board_r->addChild(_addArmyBtn);
       
        
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
        OK_btn->setTitleFontSize(18);
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
        
        
        auto attack_tips = Label::createWithSystemFont("1.Click your area. 2.Click neighbor to attack", "Arial", 30);
        attack_tips->setPosition(Vec2(attack_tips->getContentSize().width / 2 + 20,
                               _endTurnTipsLayer->getContentSize().height / 2));
        _endTurnTipsLayer->addChild(attack_tips);
        
        operat_board_m->addChild(_endTurnTipsLayer);
        
        auto end_turn_btn = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        end_turn_btn->cocos2d::Node::setScale(1.2f);
        end_turn_btn->setTitleText("END TURN");
        end_turn_btn->setTitleFontSize(16);
        end_turn_btn->addClickEventListener(CC_CALLBACK_1(GameScene::menuEndTurn, this));
        end_turn_btn->setPosition(Vec2(operat_board_m->getContentSize().width - 48 - end_turn_btn->getContentSize().width / 2,
                                 operat_board_m->getContentSize().height / 2));
        _endTurnTipsLayer->addChild(end_turn_btn);


        _diceResultLayer = Layer::create();
        _diceResultLayer->setIgnoreAnchorPointForPosition(false);
        _diceResultLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _diceResultLayer->setContentSize(operat_board_m->getContentSize());
        _diceResultLayer->setPosition(operat_board_m->getContentSize() / 2);
        operat_board_m->addChild(_diceResultLayer, ZORDER_DICE_LAYER, key_dice_layer_tag);
        _diceResultLayer->setVisible(false);
        
        
        _supplyShowLayer = Layer::create();
        _supplyShowLayer->setIgnoreAnchorPointForPosition(false);
        _supplyShowLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _supplyShowLayer->setContentSize(operat_board_m->getContentSize());
        _supplyShowLayer->setPosition(operat_board_m->getContentSize() / 2);
        operat_board_m->addChild(_supplyShowLayer, ZORDER_DICE_LAYER, key_supply_layer_tag);
        _supplyShowLayer->setVisible(false);
}

void GameScene::initControlLayer(){
        
        int game_speed = UserDefault::getInstance()->getIntegerForKey(GAME_SPEED_KEY, 1);
        
        Director::getInstance()->getScheduler()->setTimeScale(game_speed);
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        _showAreaSize = visible_size;
        _controlLayer = Layer::create();
        
        this->initOperateBoard();
        
        auto player_0 = cocos2d::ui::ImageView::create("maps/supply_back_0.png");
        auto p_size = player_0->getContentSize();
        _showAreaSize.height -= p_size.height;
        _showAreaSize.width *= 0.9;


        auto exit_btn = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");
        exit_btn->setPosition(Vec2(exit_btn->getContentSize().width / 2,
                              visible_size.height - exit_btn->getContentSize().height / 2));
        exit_btn->addClickEventListener(CC_CALLBACK_1(GameScene::menuExit, this));
        _controlLayer->addChild(exit_btn);
        
        this->addChild(_controlLayer, ZORDER_CRTL_LAYERS, key_ctrl_layer_tag);


        Director::getInstance()->setDepthTest(true);
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::loadCharact(int idx, std::string ch_name){
        auto frameCache = SpriteFrameCache::getInstance();
        
        auto frame = frameCache->getSpriteFrameByName(ch_name);
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto charactor = Sprite::create();
                charactor->setSpriteFrame(frame);
                _allFightingCharacters[idx][i] = charactor;
                charactor->retain();
        }
}


void GameScene::initAnimationLayer(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        _animationLayer = Sprite::create("zhandou_beijing.png");
         auto anim_back_size = _animationLayer->getContentSize();
        _animationLayer->setScale(0.1f, 0.1f);
        _animationLayer->setPosition(Vec2(visibleSize / 2));
        _animationLayer->setVisible(false);

        this->addChild(_animationLayer, ZORDER_ANIM_LAYER, key_anim_layer_tag);

        float scale_factor = Director::getInstance()->getContentScaleFactor();
        for (int i = 0; i < MAX_PLAYER; i++){
                _invaderPos[i]  = invader_pos[i] * (1.f / scale_factor);
                _keeperPos[i]   = keeper_pos[i] * (1.f / scale_factor);
                _keeperPos[i].x = anim_back_size.width + _keeperPos[i].x;
        }

        READY_DISTANCE_POS *= (1.f / scale_factor);

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


void GameScene::initDialog(){
        auto visible_size = Director::getInstance()->getVisibleSize();
        _winDialogLayer = Layer::create();
        _winDialogLayer->setIgnoreAnchorPointForPosition(false);
        _winDialogLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _winDialogLayer->setPosition(visible_size / 2);
        
        auto game_win_back = Sprite::create("game_win_back.png");
        game_win_back->setPosition(game_win_back->getContentSize() / 2);
        _winDialogLayer->setContentSize(game_win_back->getContentSize());
        _winDialogLayer->addChild(game_win_back);
}

#pragma mark - touch and menu event

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
        bool need_to_scale = false;
        float scale_v = _mapLayer->getScale();
        if (touches.size() >= 2
            && GAME_STATUS_INUSERTURN == _gameStatus)
        {
                Vec2 touch1 = touches[0]->getPreviousLocation();
                Vec2 touch2 = touches[1]->getPreviousLocation();
                float distance1 = touch1.distance(touch2);
                
                Vec2 touch3 = touches[0]->getLocation();
                Vec2 touch4 = touches[1]->getLocation();
                float distance2 = touch3.distance(touch4);
                
                float interval = ((float)distance1  - distance2)/ (float)distance2;
                CCLOG("---dis(1=%.f, 2=%.f, s=%.4f)---", distance1, distance2, interval);
                if (interval <-0.02f || interval > 0.02f){
                        
                        need_to_scale = true;
                        scale_v *= (interval > 0) ? 0.95f: 1.05f;
                        
                        if (scale_v > MAP_SCALE_V){
                                scale_v = MAP_SCALE_V;
                        }
                        if (scale_v < 1.0f){
                                scale_v = 1.0f;
                        }
                }
        }
        
        if (need_to_scale){
                _mapLayer->setScale(scale_v);
                return;
        }

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
        
        _attackResult = _theGameLogic->startPlayerAttack(cell_id);
        if (nullptr != _attackResult){
                _afterBattleCallback = CallFunc::create(std::bind(&GameScene::afterPlayerBattle, this));
                _afterBattleCallback->retain();
                _attackResult->retain();
                this->playBattleAnimation(true);
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
        _theGameLogic->initHistoryRecord();
        this->removeChildByTag(key_map_back_layer);
        
        _curGameData = _theGameLogic->resetInitData();
        this->initMapSize(_curGameData);
        
        _controlLayer->removeChildByTag(key_roll_show_tag);
        this->initAreaTcShow();
        
        _gameStatus = GAME_STATUS_AIRUNNING;
        _addtionalSupplyTimes = ADDTIONAL_SUPPLY_TIME_PER_GAME;
        _addArmyBtn->setEnabled(true);
        _addArmyBtn->setBright(true);
        this->gameAction();
}

#pragma mark - animation 

void GameScene::afterPlayerBattle(){
        std::map<int, int> survival = _theGameLogic->cleanUpBattleField(_attackResult);
        _attackResult->release();
        _attackResult = NULL;
        _isPalyingAnim = false;
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
        _endTurnTipsLayer->setVisible(true);
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

void GameScene::afterRobootBattle(){
        std::map<int, int> survival = _theGameLogic->cleanUpBattleField(_attackResult);
        _attackResult->release();
        _attackResult = NULL; 
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
                        if (-1 ==  player_uid){//it's color back ground.
                                continue;
                        }
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
                
                int cur_uid = _curGameData->_jun[_curGameData->_ban];
                auto num_lab = _supplyLabelMap.find(cur_uid);
                Node* grad_node = num_lab->second->getParent()->getParent();
                _curInTurnBack->setPosition(grad_node->getPosition());
        }
}

void GameScene::afterSupply(){
        _supplyShowLayer->removeAllChildren();
        _supplyShowLayer->setVisible(false);
        _curGameData->_player[_curGameData->_userId]->useTheAddSupply();
        this->refreshSupplyDiceNum();
        _theGameLogic->next_player();
        int cur_uid = _curGameData->_jun[_curGameData->_ban];
        auto num_lab = _supplyLabelMap.find(cur_uid);
        if (num_lab != _supplyLabelMap.end()){
                Node* grad_node = num_lab->second->getParent()->getParent();
                _curInTurnBack->setPosition(grad_node->getPosition());
        }        
        this->gameAction();
}

void GameScene::gameAction(){
        _attackResult = _theGameLogic->startRobootAttack();
        if (nullptr == _attackResult || _attackResult->_result == ATTACK_RES_NONE){
                auto visible_size = Director::getInstance()->getVisibleSize();
                _mapLayer->setPosition(visible_size);
                auto scale = ScaleTo::create(0.3, MAP_SCALE_V);
                _mapLayer->runAction(Sequence::create(scale, CallFunc::create( [&](){
                        _gameStatus = GAME_STATUS_INUSERTURN;
                        _animationLayer->setVisible(false);
                        _endTurnTipsLayer->setVisible(true);
                }), NULL));
                
                return;
        }
        if (ATTACK_RES_WIN == _attackResult->_result
            || ATTACK_RES_DEFEATED == _attackResult->_result){
                
                _afterBattleCallback = CallFunc::create(std::bind(&GameScene::afterRobootBattle, this));
                
                _afterBattleCallback->retain();
                _attackResult->retain();
                this->playBattleAnimation(false);
                
        }else if(ATTACK_RES_GOTSUPPLY == _attackResult->_result){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterSupply, this));
                this->playSupplyAnimation(callback);
        }
}

void GameScene::afterFightFinished(){
        
        int area_id = ATTACK_RES_DEFEATED == _attackResult->_result ? _attackResult->_fromArea : _attackResult->_toArea;
        
        _afterBattleCallback->retain();
        if (_animationIsOn && GAME_STATUS_INUSERTURN == _gameStatus){
                
                _animationLayer->removeAllChildrenWithCleanup(true);//TODO::
                auto occupay_cc = CallFunc::create(std::bind(&DiceGame::occupayAnimation, _theGameLogic, area_id, _afterBattleCallback));
                
                auto hide = ScaleTo::create(0.4f, .1f);
                _animationLayer->runAction(Sequence::create(hide,
                                                            CallFunc::create( [&](){
                        _animationLayer->setVisible(false);
                }), occupay_cc, NULL));
                
        }else{
                _theGameLogic->occupayAnimation(area_id, _afterBattleCallback);
        }
}


void GameScene::WinnerBack(){
        
        this->ShowResultData();
        
        if (ATTACK_RES_WIN == _attackResult->_result){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_WIN);
        }else if (ATTACK_RES_DEFEATED == _attackResult->_result){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_DEFEAT);
        }
        
        
        auto obj = _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0];
        
        _animationLayer->removeChild(obj, true);//TODO::
        
        Size back_size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        auto when_back_home = CallFunc::create(std::bind(&GameScene::afterFightFinished, this));
        
        int player_uid = ATTACK_RES_WIN == _attackResult->_result ? _attackResult->_fromPlayer : _attackResult->_toPlayer;
        auto run_back_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid])->clone();
        run_back_anim->setRestoreOriginalFrame(true);
        
        auto back_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid]);
        back_wait->setRestoreOriginalFrame(true);
        
        
        if (ATTACK_RES_WIN == _attackResult->_result){
                
                int keeper_uid = _attackResult->_toPlayer;
                for (int i = 0; i < _attackResult->_to.size(); i++){
                        auto keeper = _allFightingCharacters[keeper_uid][i];
                        _animationLayer->removeChild(keeper, true);//TODO::
                }
                
                auto moveby = MoveBy::create(0.5, Vec2(READY_DISTANCE_POS - back_size.width / 2,0));
                auto run_back = Spawn::create(Animate::create(run_back_anim), FlipX::create(true), moveby, NULL);
                
                auto fight_back = Animate::create(back_wait);
                
                auto move = MoveBy::create(1.f, Vec2(-READY_DISTANCE_POS,0));
                auto run_back_anim2 = run_back_anim->clone();
                run_back_anim2->setLoops(2);
                Spawn* back_home = Spawn::create(Animate::create(run_back_anim2), move, NULL);
                
                Sequence* invade_back = Sequence::create(run_back, fight_back,  back_home, NULL);
                
                for (int i = 0; i < _attackResult->_from.size(); i++){
                        auto invader = _allFightingCharacters[player_uid][i];
                        
                        if (i == 0){
                                auto s_with_cc = Sequence::create(run_back, fight_back,  back_home, when_back_home, NULL);
                                invader->runAction(s_with_cc);
                        }else{
                                invader->runAction(invade_back->clone());
                        }
                }
        }else{
                
                int invader_uid = _attackResult->_fromPlayer;
                for (int i = 0; i < _attackResult->_from.size(); i++){
                        auto invader = _allFightingCharacters[invader_uid][i];
                        _animationLayer->removeChild(invader, true);//TODO::
                }
                
                auto moveby = MoveBy::create(0.5f, Vec2(back_size.width / 2 - READY_DISTANCE_POS, 0));
                auto run_back = Spawn::create(Animate::create(run_back_anim), FlipX::create(false), moveby, NULL);
                
                auto fight_back = Animate::create(back_wait);
                
                auto move = MoveBy::create(1.f, Vec2(READY_DISTANCE_POS, 0));
                auto run_back_anim2 = run_back_anim->clone();
                run_back_anim2->setLoops(2);
                Spawn* back_home = Spawn::create(Animate::create(run_back_anim2), move, NULL);
                
                Sequence* keeper_back = Sequence::create(run_back, fight_back, back_home, NULL);

                for (int i = 0; i < _attackResult->_to.size(); i++){
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

void GameScene::ShowResultData(){
        _diceResultLayer->setVisible(true);
        auto back_size = _diceResultLayer->getContentSize();
        auto frameCache = SpriteFrameCache::getInstance();

        for (int i = 0; i < _attackResult->_from.size(); i++){
                int value = _attackResult->_from[i];
                auto str = DICE_PIC_NAME_STR[_attackResult->_fromPlayer][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                Vec2 pos(back_size.width / 2 - dice_size.width * (i + 1), dice_size.height);
                dice->setPosition(pos);
                _diceResultLayer->addChild(dice);

                if (i == _attackResult->_from.size() - 1){
                        auto from_value = Label::createWithSystemFont(StringUtils::format("%d", _attackResult->_fromSum), "Arial", 40);
                        from_value->setPosition(Vec2(pos.x - dice_size.width - 10, pos.y));
                        _diceResultLayer->addChild(from_value);
                }
        }

        for (int i = 0; i < _attackResult->_to.size(); i++){
                int value = _attackResult->_to[i];
                auto str = DICE_PIC_NAME_STR[_attackResult->_toPlayer][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                Vec2 pos(back_size.width / 2 + dice_size.width * (i + 1),  dice_size.height);
                dice->setPosition(pos);
                _diceResultLayer->addChild(dice);

                if (i == _attackResult->_to.size() - 1){
                        auto to_value = Label::createWithSystemFont(StringUtils::format("%d", _attackResult->_toSum), "Arial", 40);
                        to_value->setPosition(Vec2(pos.x + dice_size.width + 10, pos.y));
                        _diceResultLayer->addChild(to_value);
                }
        }
}

void GameScene::Fighting(){
        
        auto winner_back = CallFunc::create(std::bind(&GameScene::WinnerBack, this));
        
        auto fight_cloud = _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0];
        _animationLayer->addChild(fight_cloud);//TODO::
        
        auto size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        
        fight_cloud->setPosition(Vec2(size.width / 2, size.height * 0.4));
        fight_cloud->setScale(2.0f);
        auto anim_cloud = cache->getAnimation("finght_cloud")->clone();
        anim_cloud->setRestoreOriginalFrame(true);
        auto anim_action = Animate::create(anim_cloud);
        
        fight_cloud->runAction(Sequence::create(anim_action, winner_back, NULL));
}

void GameScene::afterShowFightBg(){
        Size back_size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        
        //FIGHTER------------------------
{
        int player_uid = _attackResult->_fromPlayer;

        auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid])->clone();
        run_anim->setRestoreOriginalFrame(true);
        auto run_action = Animate::create(run_anim);
        auto move = MoveBy::create(0.5, Vec2(READY_DISTANCE_POS,0));
        Spawn* get_ready = Spawn::create(run_action, FlipX::create(false), move, NULL);
        
        
        
        auto stand_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid])->clone();
        stand_wait->setRestoreOriginalFrame(true);
        auto fight_wait = Animate::create(stand_wait);
        
        
        auto moveby = MoveBy::create(1.f, Vec2(back_size.width / 2 - READY_DISTANCE_POS,0));
        auto run_anim2 = run_anim->clone();
        run_anim2->setLoops(2);
        auto run_to_fight = Spawn::create(Animate::create(run_anim2), moveby, NULL);
        

        Sequence* invade_seq = Sequence::create(get_ready, fight_wait,  run_to_fight, NULL);
        
        for (int i = 0; i < _attackResult->_from.size(); i++){
                
                auto invader = _allFightingCharacters[_attackResult->_fromPlayer][i];
                invader->setPosition(_invaderPos[i] - Vec2(READY_DISTANCE_POS, 0));
                _animationLayer->addChild(invader);
                
                if (i == 0){
                        auto fighting = CallFunc::create(std::bind(&GameScene::Fighting, this));
                        auto s_with_cc = Sequence::create(get_ready, fight_wait,  run_to_fight, fighting, NULL);
                        invader->runAction(s_with_cc);
                }else{
                        invader->runAction(invade_seq->clone());
                }
        }
}
        
        
        //KEEPER------------------------
{
        int player_uid = _attackResult->_toPlayer;
        
        auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid])->clone();
        run_anim->setRestoreOriginalFrame(true);
        
        auto move = MoveBy::create(0.5, Vec2(-READY_DISTANCE_POS,0));
        Spawn* get_ready = Spawn::create(Animate::create(run_anim), FlipX::create(true), move, NULL);
        
        
        auto stand_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid]);
        stand_wait->setRestoreOriginalFrame(true);
        auto fight_wait = Animate::create(stand_wait);
        
        
        auto moveby = MoveBy::create(1.f, Vec2(READY_DISTANCE_POS - back_size.width / 2 ,0));
        auto run_anim2 = run_anim->clone();
        run_anim2->setLoops(2);
        auto run_to_fight = Spawn::create(Animate::create(run_anim2), moveby, NULL);
        
        Sequence* keeper_seq = Sequence::create(get_ready, fight_wait, run_to_fight, NULL);
        
        for (int i = 0; i < _attackResult->_to.size(); i++){
                auto keeper = _allFightingCharacters[player_uid][i];
                _animationLayer->addChild(keeper);
                keeper->setPosition(_keeperPos[i] + Vec2(READY_DISTANCE_POS, 0));
                keeper->runAction(keeper_seq->clone());
        }
}
}

void GameScene::playBattleAnimation(bool isMaunual){
        
        _endTurnTipsLayer->setVisible(false);
        if (_animationIsOn && isMaunual){
                _isPalyingAnim = true;
                _animationLayer->setVisible(true);
                auto show = ScaleTo::create(.4f, 1.0f);
                auto cc = CallFunc::create(std::bind(&GameScene::afterShowFightBg, this));
                _animationLayer->runAction(Sequence::create(show, cc, NULL));
                
        }else{
                this->ShowResultData();
                this->afterFightFinished();
        }
}

void GameScene::playSupplyAnimation2(CallFunc* callback, GamePlayer* player){
        
        std::map<AreaData*, int> supply_data = _theGameLogic->starSupplyDice(player);
        if (supply_data.size() == 0){
                callback->execute();
                return;
        }
        
        Vector<Node*> supp_nodes = _supplyShowLayer->getChildren();
        int idx = 0;
        for (std::map<AreaData*, int>::iterator it = supply_data.begin();
             it != supply_data.end(); ++it){
                
                
                int sup_num = it->second;
                AreaData*  cur_area = it->first;
                Vec2 pos = cur_area->getSpriteWorldPos();
                Vec2 rel_pos = _supplyShowLayer->convertToNodeSpace(pos);
                auto move = MoveTo::create(1.2f, rel_pos);
                std::map<AreaData*, int>::iterator it_end = supply_data.end();
                it_end--;
                
                bool is_t_last = it == it_end;
//                bool is_t_last = false;
                for (int i = idx; i < idx + sup_num; i++){
                        Node* sup_one = supp_nodes.at(i);
                        
                        bool is_last = i == idx + sup_num - 1;
                        CallFunc*  cb = CallFunc::create([this, sup_one, callback, is_t_last, is_last, cur_area](){

                                if (is_last){
                                        cur_area->updatePawn(_curGameData->_referedLayer);
                                        if (is_t_last){
                                                cur_area->drawSupply(_curGameData->_referedLayer, callback);
                                        }else{
                                                cur_area->drawSupply(_curGameData->_referedLayer, NULL);
                                        }
                                }
                        });
                        cb->retain();
                        sup_one->runAction(Sequence::create(move->clone(), cb,NULL));
                }
                idx += sup_num;
        }
}

void GameScene::playSupplyAnimation(CallFunc* callback){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_SUPPLY);
        _supplyShowLayer->setVisible(true);
        int player_id = _curGameData->_jun[_curGameData->_ban];
        GamePlayer* player = _curGameData->_player[player_id];
        int total_stock = player->setStock();
        
        auto back_size = _supplyShowLayer->getContentSize();
        for (int i = 0; i < total_stock; i++){
                int ch_idx = player->getPosCharactorIdx();
                std::string charact_name = CHARACTER_NAME[ch_idx];
                
                auto character = Sprite::create(charact_name);
                character->setScale(0.5f);
                auto ch_size = character->getContentSize() * 0.5f;
                Vec2 pos;
                pos.x = (2 * i + 1) * 0.5f * ch_size.width;
                pos.y = (((int)(i / AREA_MAX)) * 2 + 1) * back_size.height * 0.25f;
                character->setPosition(pos);
                _supplyShowLayer->addChild(character);
        }
        
        auto scale = ScaleTo::create(0.6f, 1.0f);
        callback->retain();
        CallFunc* cb = CallFunc::create(std::bind(&GameScene::playSupplyAnimation2, this, callback, player));
        cb->retain();
        _supplyShowLayer->runAction(Sequence::create(scale, cb, NULL));
}

#pragma mark - menu callback actions
void GameScene::menuEndTurn(Ref* pSender){
        if (_isPalyingAnim){
                return;
        }
        
        _theGameLogic->clearManulAction();
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        _mapLayer->setPosition(visible_size);
        
        auto scale = ScaleTo::create(0.3, 1.f);
        _mapLayer->runAction(Sequence::create(scale, CallFunc::create( [&](){
                _endTurnTipsLayer->setVisible(false);
                _gameStatus = GAME_STATUS_AIRUNNING;
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterSupply, this));
                this->playSupplyAnimation(callback);
                
        }), NULL));
}

void GameScene::createNewMap(Ref* pSender){
        _curGameData->release();
        _curGameData = _theGameLogic->initGameData(_playerNumber, _charactorIdx, _colorIdx);
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
        
        if (_animationIsOn
            &&_attackResult != nullptr
            && _isPalyingAnim){
                        this->afterFightFinished();
        }
        
        _animationIsOn = !_animationIsOn;
        if(_animationIsOn)
                _animCtlBtn->loadTextureNormal("maps/open_anim.png");
        else
                _animCtlBtn->loadTextureNormal("maps/close_anim.png");
        
        UserDefault::getInstance()->setBoolForKey(ANIMATION_SWITCH_KEY, _animationIsOn);
        UserDefault::getInstance()->flush();
}

void GameScene::menuAddArmy(Ref* btn){
        if (GAME_STATUS_INUSERTURN != _gameStatus
            ||_addtionalSupplyTimes-- <= 0){
                return;
        }
        
        auto btn_anim = _addArmyBtn->clone();
        _curGameData->_player[_curGameData->_userId]->addMoreSupply();
        auto parent = _addArmyBtn->getParent();
        btn_anim->setPosition(_addArmyBtn->getPosition());
        parent->addChild(btn_anim);
        Vec2 pos = parent->convertToNodeSpace(_curPlayerSupFlag->getParent()->convertToWorldSpace(_curPlayerSupFlag->getPosition()));
        auto move = MoveTo::create(0.8,  pos);
        auto scale = ScaleTo::create(0.8, 0.6f);
        btn_anim->runAction(Sequence::create( Spawn::create(move, scale, NULL),
                                    CallFunc::create( [this, btn_anim](){
                btn_anim->removeFromParentAndCleanup(true);
                if (_addtionalSupplyTimes <= 0){
                        _addArmyBtn->setEnabled(false);
                        _addArmyBtn->setBright(false);
                }

                this->refreshSupplyDiceNum();
                auto scale_s = ScaleTo::create(0.3, 1.4f);
                auto scale_s_r = ScaleTo::create(0.3, 1.0f);
                _curPlayerSupFlag->runAction(Sequence::create(scale_s, scale_s_r, NULL));
        }), NULL));
}
