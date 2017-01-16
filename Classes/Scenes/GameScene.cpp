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
#include "PopUpOkCancelDialog.hpp"
#include "PopUpOkDialog.hpp"
#include "CommonTipsDialog.hpp"
#include "AchievementEngine.hpp"

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
        key_operate_board_tag_r,
        key_supply_turn_counter_tag,
        key_winer_back,
        key_winer_content_back,
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
        for (int i = 0; i < FIGH_ANIM_MAX; i++){
                for (int j = 0; j < MAX_DICE_PER_AREA; j++){
                        _allFightingCharacters[i][j]->release();
                }
        }
        _winDialogLayer->release();
        _lostDialogLayer->release(); 
} 

bool GameScene::init()
{
        if (!Layer::init()){
                return false;
        }
        auto cache = UserDefault::getInstance();
        _gameStatus = GAME_STATUS_INIT;
        _soundTotalOn = cache->getBoolForKey(SOUND_MUSIC_TOTAL_KEY, true);
        _musicSwitch = cache->getBoolForKey(BACK_MUSIC_SWITCH_KEY);
        _soundEngine = CocosDenshion::SimpleAudioEngine::getInstance();
        
        
        _theGameLogic = DiceGame::create();
        _theGameLogic->retain();
        
        _curGameData = _theGameLogic->initGameData(_playerNumber, _charactorIdx, _colorIdx); 
       
        this->initControlLayer();
        this->initMapSize(_curGameData);
        this->initAnimationLayer();
        this->initDialog();
//        this->showWinDialog(); 
//        this->addChild(_lostDialogLayer, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
        
        sdkbox::PluginFacebook::setListener(this);
        sdkbox::PluginFacebook::init();         
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
                auto numbser = Label::createWithSystemFont(tc_str, "fonts/arial.ttf", 26);
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
        
        _animCtlBtn->loadTexturePressed("maps/open_anim_sel.png");
        
        _animCtlBtn->addClickEventListener(CC_CALLBACK_1(GameScene::menuAnimSwitch, this));
        _animCtlBtn->setPosition(operat_board_l->getContentSize() / 2);
        operat_board_l->addChild(_animCtlBtn);

        auto operat_board_r = Sprite::create("maps/openrate_board_btn.png");
        operat_board_r->setPosition(Vec2(operat_board->getContentSize().width - operat_board_r->getContentSize().width * 0.6,
                                         operat_board_r->getContentSize().height * 0.6));
        operat_board->addChild(operat_board_r, 1);
        
        _addtionalSupplyCounter = 0;
        _addArmyBtn = cocos2d::ui::Button::create("maps/addtion_supply_arm.png",
                                                  "maps/addtion_supply_arm.png",
                                                  "maps/supply_finished.png");
        _addArmyBtn->setPosition(operat_board_r->getContentSize() / 2);
        _addArmyBtn->addClickEventListener(CC_CALLBACK_1(GameScene::menuAddArmy, this));
        auto counter =  ui::TextBMFont::create("0", "fonts/zb_chongzhi_shuzi.fnt");
        counter->setPosition(_addArmyBtn->getContentSize() / 2);
        _addArmyBtn->addChild(counter, 1, key_supply_turn_counter_tag);
        counter->setVisible(false);
        operat_board_r->addChild(_addArmyBtn);
       
        
        auto first_tip_layer = Layer::create();
        first_tip_layer->setContentSize(operat_board_m->getContentSize());
        first_tip_layer->setPosition(operat_board_m->getContentSize() / 2);
        first_tip_layer->setIgnoreAnchorPointForPosition(false);
        first_tip_layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        auto tips = Label::createWithSystemFont("Do you want to use this map?", "fonts/arial.ttf", 40);
        tips->setPosition(Vec2(tips->getContentSize().width / 2 + 20,
                               first_tip_layer->getContentSize().height / 2));
        first_tip_layer->addChild(tips);
        
        operat_board_m->addChild(first_tip_layer);

        auto OK_btn = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        OK_btn->setTitleText("YES");
        OK_btn->setTitleFontSize(30);
        OK_btn->addClickEventListener(CC_CALLBACK_1(GameScene::menuStartGame, this, first_tip_layer));
        OK_btn->setPosition(Vec2(tips->getContentSize().width + 0.7f * OK_btn->getContentSize().width,
                                 operat_board_m->getContentSize().height / 2));
        first_tip_layer->addChild(OK_btn);
        
        
        auto NO_btn = cocos2d::ui::Button::create("DIALOG_CANCEL.png", "DIALOG_CANCEL_SEL.png");
        NO_btn->setTitleText("NO");
        NO_btn->setTitleFontSize(30);
        NO_btn->setPosition(Vec2(OK_btn->getPosition().x + NO_btn->getContentSize().width * 1.1f,
                            operat_board_m->getContentSize().height / 2));
        NO_btn->addClickEventListener(CC_CALLBACK_1(GameScene::createNewMap, this));
        first_tip_layer->addChild(NO_btn);
        
        _endTurnTipsLayer = Layer::create();
        _endTurnTipsLayer->setContentSize(operat_board_m->getContentSize());
        _endTurnTipsLayer->setPosition(operat_board_m->getContentSize() / 2);
        _endTurnTipsLayer->setIgnoreAnchorPointForPosition(false);
        _endTurnTipsLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _endTurnTipsLayer->setVisible(false);
        
        
        auto attack_tips = Label::createWithSystemFont("1.Click your area. 2.Click neighbor to attack", "fonts/arial.ttf", 30);
        attack_tips->setPosition(Vec2(attack_tips->getContentSize().width / 2 + 20,
                               _endTurnTipsLayer->getContentSize().height / 2));
        _endTurnTipsLayer->addChild(attack_tips);
        
        operat_board_m->addChild(_endTurnTipsLayer);
        
        auto end_turn_btn = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        end_turn_btn->setTitleText("END TURN");
        end_turn_btn->setTitleFontSize(22);
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
        
        this->loadCharact(PLAYER_ROLE_TYPE_GONGJIANSHOU, "gjsrun010001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_HUOQIANSHOU, "hqsrun010001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_MAONV, "mvrun010001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_CHANGQIANGSHOU, "casrun010001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_SHOUREN, "xssrun0001.png");
        
        this->loadCharact(PLAYER_ROLE_TYPE_DAOZEI, "xssrun0001.png");
        
        this->loadCharact(FIGHT_ANIM_TYPE_XINYUN, "yw0001.png");
}


void GameScene::initDialog(){
        
        /*
         *
         *win dialog
         */
        auto visible_size = Director::getInstance()->getVisibleSize();
        _winDialogLayer = Layer::create();
        _winDialogLayer->retain();
        _winDialogLayer->setIgnoreAnchorPointForPosition(false);
        _winDialogLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _winDialogLayer->setPosition(visible_size / 2);
        
        auto game_win_back = Sprite::create("game_win_back.png");
        auto win_back_size = game_win_back->getContentSize();
        game_win_back->setPosition(win_back_size / 2);
        _winDialogLayer->setContentSize(win_back_size);
        _winDialogLayer->addChild(game_win_back, 1, key_winer_back);
        
        auto game_win_title = Sprite::create("game_win_title.png");
        game_win_title->setPosition(Vec2(win_back_size.width / 2, win_back_size.height + game_win_title->getContentSize().height * 0.2));
        game_win_back->addChild(game_win_title, 2);
        
        auto game_win_shine = Sprite::create("game_win_shine.png");
        game_win_shine->setPosition(game_win_title->getPosition());
        _winDialogLayer->addChild(game_win_shine);
        auto anim_rotate = RotateBy::create(2.f, 360);
        game_win_shine->runAction(RepeatForever::create(anim_rotate));
        
        auto game_win_t_back = Sprite::create("game_win_t_back.png");
        auto game_win_t_back_pos = Vec2(0.475 *game_win_t_back->getContentSize().width ,
                                        win_back_size.height - game_win_t_back->getContentSize().height / 2 - 10);
        game_win_t_back->setPosition(game_win_t_back_pos);
        game_win_back->addChild(game_win_t_back);
        
        auto win_title_txt = Label::createWithSystemFont("You win. Congratulations!!!", "fonts/arial.ttf", 32);
        win_title_txt->setPosition(Vec2(win_title_txt->getContentSize().width * 0.6,
                                        game_win_t_back->getContentSize().height * 0.7));
        game_win_t_back->addChild(win_title_txt);
        
        
        auto game_win_c_back = Sprite::create("game_win_c_back.png");
        
        game_win_c_back->setPosition(Vec2(win_back_size.width / 2,
                                          game_win_t_back_pos.y - game_win_c_back->getContentSize().height * 0.63f));
        game_win_back->addChild(game_win_c_back, 1, key_winer_content_back);
        
        
        auto replay_btn = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        
        auto game_win_c_back_pos = game_win_c_back->getPosition();
        replay_btn->setPosition(Vec2(win_back_size.width / 2,
                                     replay_btn->getContentSize().height *1.2f));
        auto btn_pos = replay_btn->getPosition();
        auto btn_size = replay_btn->getContentSize();
        game_win_back->addChild(replay_btn);
        replay_btn->setTitleText("Replay");
        replay_btn->setTitleFontSize(26);
        replay_btn->setTitleFontName("fonts/arial.ttf");
        replay_btn->setTitleColor(Color3B::BLACK);
        replay_btn->addClickEventListener(CC_CALLBACK_1(GameScene::gameOver, this, 1));
        
        auto share_btn = (ui::Button*)replay_btn->clone();
        share_btn->setPosition(Vec2(win_back_size.width * 0.22f, btn_pos.y));
        game_win_back->addChild(share_btn);
        share_btn->setTitleText("Share");
        share_btn->addClickEventListener(CC_CALLBACK_1(GameScene::shareThisGame, this));
        
        auto return_btn = (ui::Button*)replay_btn->clone();
        return_btn->setPosition(Vec2(win_back_size.width * 0.77f, btn_pos.y));
        game_win_back->addChild(return_btn);
        return_btn->setTitleText("Back");
        return_btn->addClickEventListener(CC_CALLBACK_1(GameScene::gameOver, this, 0));
        
        /*
         *
         *lost dialog
         */
        _lostDialogLayer = Layer::create();
        _lostDialogLayer->retain();
        _lostDialogLayer->setIgnoreAnchorPointForPosition(false);
        _lostDialogLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _lostDialogLayer->setPosition(visible_size / 2);
        
        auto game_lost_back = Sprite::create("game_lost_back.png");
        auto lost_back_size = game_lost_back->getContentSize();
        game_lost_back->setPosition(lost_back_size / 2);
        _lostDialogLayer->setContentSize(lost_back_size);
        _lostDialogLayer->addChild(game_lost_back, 1);
        
        auto game_lost_title = Sprite::create("game_lost_title.png");
        game_lost_title->setPosition(Vec2(lost_back_size.width / 2, lost_back_size.height + game_lost_title->getContentSize().height * 0.2));
        game_lost_back->addChild(game_lost_title, 2);
        
        auto game_lost_t_bck = Sprite::create("game_lost_t_bck.png");
        auto game_lost_t_bck_pos = Vec2(0.452 * game_lost_t_bck->getContentSize().width , lost_back_size.height - game_lost_t_bck->getContentSize().height / 2 - 10);
        game_lost_t_bck->setPosition(game_lost_t_bck_pos);
        game_lost_back->addChild(game_lost_t_bck);
        
        auto lost_title_txt = Label::createWithSystemFont("You lost...", "fonts/arial.ttf", 32);
        lost_title_txt->setPosition(Vec2(lost_title_txt->getContentSize().width * 0.55,
                                        game_lost_t_bck->getContentSize().height * 0.7));
        game_lost_t_bck->addChild(lost_title_txt);
        
        
        
        auto game_lost_c_bck = Sprite::create("game_lost_c_bck.png");
        
        game_lost_c_bck->setPosition(Vec2(lost_back_size.width / 2,
                                          game_lost_t_bck_pos.y - game_lost_c_bck->getContentSize().height * 0.63f));
        game_lost_back->addChild(game_lost_c_bck);
        auto game_lost_c_bck_size = game_lost_back->getContentSize();
        auto lost_tips = Label::createWithSystemFont("You can engage the mercenaries to make your country powerfu. if you use you mecenaries up, you can get more coins from shop to engage more mercenaries.", "fonts/arial.ttf", 26);
        Size lost_tips_size = lost_tips->getContentSize();
        lost_tips->setAnchorPoint(Vec2(0.f, 1.0f));
        lost_tips->setDimensions(game_lost_c_bck_size.width * 0.9,
                                game_lost_c_bck_size.height * 0.9);
        
        lost_tips->setHorizontalAlignment(TextHAlignment::LEFT);
        lost_tips->setColor(Color3B::BLACK);
        lost_tips->setPosition(Vec2(game_lost_c_bck_size.width * 0.05,
                                   lost_tips->getContentSize().height  * .5f));
        game_lost_c_bck->addChild(lost_tips);
        
        
        auto return_btn_2 = (ui::Button*)return_btn->clone();
        return_btn_2->setPosition(Vec2(lost_back_size.width / 2 + return_btn_2->getContentSize().width, return_btn_2->getContentSize().height * 1.1f));
        game_lost_back->addChild(return_btn_2);
        
        auto replay_btn_2 = (ui::Button*)replay_btn->clone();
        replay_btn_2->setPosition(Vec2(lost_back_size.width / 2 - replay_btn_2->getContentSize().width, replay_btn_2->getContentSize().height * 1.1f));
        game_lost_back->addChild(replay_btn_2); 
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
                _attackResult->retain();
                _afterBattleCallback->retain();
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
        
        AchievementEngine::getInstance()->openReward(ACHIEVE_DATA_KEY_FIRST_RETRY);
        
        _theGameLogic->initHistoryRecord();
        this->removeChildByTag(key_map_back_layer);
        
        _curGameData = _theGameLogic->resetInitData();
        this->initMapSize(_curGameData);
        
        _controlLayer->removeChildByTag(key_roll_show_tag);
        this->initAreaTcShow();
        
        _gameStatus = GAME_STATUS_AIRUNNING;
        _addtionalSupplyCounter = 0;
        _addArmyBtn->setEnabled(true);
        _addArmyBtn->setBright(true);
        _addArmyBtn->getChildByTag(key_supply_turn_counter_tag)->setVisible(false);
        _diceResultLayer->setVisible(false);
        this->gameAction();
}


void GameScene::showWinDialog(){
        
        this->playSoundEffect(EFFECT_FILE_FINISH_WIN);
        this->addChild(_winDialogLayer, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
        auto back_ground = _winDialogLayer->getChildByTag(key_winer_back);
        auto content_back = back_ground->getChildByTag(key_winer_content_back);
        content_back->removeAllChildren();
        
        
        AchievementData data = AchievementEngine::getInstance()->winnerRewards(_playerNumber);
        AchievementEngine::getInstance()->winCounter();
        
        auto back_size = content_back->getContentSize();
        auto win_tips = Label::createWithSystemFont("Rewards:", "fonts/arial.ttf", 28);
        win_tips->setColor(Color3B::BLACK);
        win_tips->setPosition(Vec2(win_tips->getContentSize().width * 0.55,
                                   back_size.height - win_tips->getContentSize().height * 0.6));
        content_back->addChild(win_tips);
        
        if (data.isNUll()){
                auto coins = Sprite::create("level/coins_show.png");
                coins->setPosition(back_size * .5f);
                content_back->addChild(coins);
                
                std::string coins_num = StringUtils::format("X%d", data.bonus_coinsNum);
                auto num_label = Label::createWithSystemFont(coins_num, "fonts/arial.ttf", 42);
                num_label->setPosition(Vec2(coins->getContentSize().width * 1.6,
                                            coins->getContentSize().height * 0.5f));
                coins->addChild(num_label);
        }else{
                auto achieve_show = Sprite::create("achievement.png");
                auto achieve_size = achieve_show->getContentSize();
                achieve_show->setPosition(Vec2(back_size.width * .5f + 1.5 * achieve_size.width,
                                            back_size.height * .5f));
                content_back->addChild(achieve_show);
                
                auto achieve_tittle = Label::createWithSystemFont(data.title, "fonts/arial.ttf", 28);
                achieve_tittle->setPosition(Vec2(achieve_size.width * 2.f,
                                            achieve_size.height * 0.3f));
                achieve_show->addChild(achieve_tittle);
                
                
                std::string coins_num = StringUtils::format("X%d", data.bonus_coinsNum);
                auto coins = Sprite::create("level/coins_show.png");
                coins->setPosition(Vec2(back_size.width * .5f - 1.5 * achieve_size.width,
                                        back_size.height * .5f));
                content_back->addChild(coins);
                
                auto ci_no = Label::createWithSystemFont(coins_num, "fonts/arial.ttf", 42);
                ci_no->setPosition(Vec2(coins->getContentSize().width * 1.6f,
                                        coins->getContentSize().height * 0.5f));
                coins->addChild(ci_no);
        }
}

#pragma mark - animation

void GameScene::afterPlayerBattle(){
        std::map<int, int> survival = _theGameLogic->cleanUpBattleField(_attackResult);
        _attackResult->release();
        _afterBattleCallback->release();
        _attackResult = NULL;
        _isPalyingAnim = false;
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
        
        if (survival.size() == 1){
                this->showWinDialog();
                return;
        }
        this->refreshAreaTcShow(survival);
        _endTurnTipsLayer->setVisible(true);
}

void GameScene::afterRobootBattle(){
        std::map<int, int> survival = _theGameLogic->cleanUpBattleField(_attackResult);
        _attackResult->release();
        _afterBattleCallback->release();
        _attackResult = NULL;
        _isPalyingAnim = false;
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
        int user_tc = _theGameLogic->getUserTC();
        if (0 == user_tc){
                this->playSoundEffect(EFFECT_FILE_FINISH_LOSE);
                this->addChild(_lostDialogLayer, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
                AchievementEngine::getInstance()->resetWinCounter();
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
        _isPalyingAnim = false;
        _afterSupplyCallback->release();
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
                auto callback =  CallFunc::create( [&](){
                        _gameStatus = GAME_STATUS_INUSERTURN;
                        _animationLayer->setVisible(false);
                        _endTurnTipsLayer->setVisible(true);
                });
                
                _mapLayer->runAction(Sequence::create(scale, callback, NULL));
                
                return;
        }
        if (ATTACK_RES_WIN == _attackResult->_result
            || ATTACK_RES_DEFEATED == _attackResult->_result){
                
                _afterBattleCallback = CallFunc::create(std::bind(&GameScene::afterRobootBattle, this));
                _afterBattleCallback->retain();
                _attackResult->retain();
                this->playBattleAnimation(false);
                
        }else if(ATTACK_RES_GOTSUPPLY == _attackResult->_result){
                
                _afterSupplyCallback = CallFunc::create(std::bind(&GameScene::afterSupply, this));
                _afterSupplyCallback->retain();
                this->playSupplyAnimation();
        }
}

void GameScene::afterFightFinished(){
        int area_id;
        if (ATTACK_RES_DEFEATED == _attackResult->_result){
                area_id = _attackResult->_fromArea;
                this->playSoundEffect(EFFECT_FILE_DEFEAT);
        }else{
                area_id = _attackResult->_toArea;
                this->playSoundEffect(EFFECT_FILE_WIN);
        }
        
        if (_animationIsOn && GAME_STATUS_INUSERTURN == _gameStatus){
                
                _animationLayer->removeAllChildrenWithCleanup(true);
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
                this->playSoundEffect(EFFECT_FILE_WIN);
        }else if (ATTACK_RES_DEFEATED == _attackResult->_result){
                this->playSoundEffect(EFFECT_FILE_DEFEAT);
        }
        
        
        auto obj = _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0];
        
        _animationLayer->removeChild(obj, true);
        
        Size back_size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        auto when_back_home = CallFunc::create(std::bind(&GameScene::afterFightFinished, this));
        
        int player_uid = ATTACK_RES_WIN == _attackResult->_result ? _attackResult->_fromPlayer : _attackResult->_toPlayer;
        int charactor_idx = _curGameData->_player[player_uid]->getPosCharactorIdx();
        auto run_back_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[charactor_idx])->clone();
        run_back_anim->setLoops(2);
        run_back_anim->setRestoreOriginalFrame(true);
        
        auto back_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[charactor_idx]);
        back_wait->setRestoreOriginalFrame(true);
        
        this->playSoundEffect(EFFECT_MARCH_IN_ANIM);
        if (ATTACK_RES_WIN == _attackResult->_result){
                
                int keeper_uid = _attackResult->_toPlayer;
                int keeper_uid_idx = _curGameData->_player[keeper_uid]->getPosCharactorIdx();
                for (int i = 0; i < _attackResult->_to.size(); i++){
                        auto keeper = _allFightingCharacters[keeper_uid_idx][i];
                        _animationLayer->removeChild(keeper, true);
                }
                
                auto moveby = MoveBy::create(1.f, Vec2(READY_DISTANCE_POS - back_size.width / 2,0));
                auto run_back = Spawn::create(Animate::create(run_back_anim), FlipX::create(true), moveby, NULL);
                
                auto fight_back = Animate::create(back_wait);
                
                auto move = MoveBy::create(0.5f, Vec2(-READY_DISTANCE_POS, 0));
                auto run_back_anim2 = run_back_anim->clone();
                Spawn* back_home = Spawn::create(Animate::create(run_back_anim2), move, NULL);
                
                Sequence* invade_back = Sequence::create(run_back, fight_back,  back_home, NULL);
                
                for (int i = 0; i < _attackResult->_from.size(); i++){
                        auto invader = _allFightingCharacters[charactor_idx][i];
                        
                        if (i == 0){
                                auto s_with_cc = Sequence::create(run_back, fight_back,  back_home, when_back_home, NULL);
                                invader->runAction(s_with_cc);
                        }else{
                                invader->runAction(invade_back->clone());
                        }
                }
        }else{
                
                int invader_uid = _attackResult->_fromPlayer;
                int invader_uid_idx = _curGameData->_player[invader_uid]->getPosCharactorIdx();
                for (int i = 0; i < _attackResult->_from.size(); i++){
                        auto invader = _allFightingCharacters[invader_uid_idx][i];
                        _animationLayer->removeChild(invader, true);
                }
                
                auto moveby = MoveBy::create(1.f, Vec2(back_size.width / 2 - READY_DISTANCE_POS, 0));
                auto run_back = Spawn::create(Animate::create(run_back_anim), FlipX::create(false), moveby, NULL);
                
                auto fight_back = Animate::create(back_wait);
                
                auto move = MoveBy::create(0.5f, Vec2(READY_DISTANCE_POS, 0));
                auto run_back_anim2 = run_back_anim->clone();
                Spawn* back_home = Spawn::create(Animate::create(run_back_anim2), move, NULL);
                
                Sequence* keeper_back = Sequence::create(run_back, fight_back, back_home, NULL);

                for (int i = 0; i < _attackResult->_to.size(); i++){
                        auto keeper = _allFightingCharacters[charactor_idx][i];
                        
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
                
                int c_i = _curGameData->_player[_attackResult->_fromPlayer]->getAreaColorIndex();
                
                auto str = DICE_PIC_NAME_STR[c_i][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                Vec2 pos(back_size.width / 2 - dice_size.width * (i + 1), dice_size.height);
                dice->setPosition(pos);
                _diceResultLayer->addChild(dice);

                if (i == _attackResult->_from.size() - 1){
                        auto from_value = Label::createWithSystemFont(StringUtils::format("%d", _attackResult->_fromSum), "fonts/arial.ttf", 40);
                        from_value->setPosition(Vec2(pos.x - dice_size.width - 10, pos.y));
                        _diceResultLayer->addChild(from_value);
                }
        }

        for (int i = 0; i < _attackResult->_to.size(); i++){
                int value = _attackResult->_to[i];
                int c_i = _curGameData->_player[_attackResult->_toPlayer]->getAreaColorIndex();
                auto str = DICE_PIC_NAME_STR[c_i][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                Vec2 pos(back_size.width / 2 + dice_size.width * (i + 1),  dice_size.height);
                dice->setPosition(pos);
                _diceResultLayer->addChild(dice);

                if (i == _attackResult->_to.size() - 1){
                        auto to_value = Label::createWithSystemFont(StringUtils::format("%d", _attackResult->_toSum), "fonts/arial.ttf", 40);
                        to_value->setPosition(Vec2(pos.x + dice_size.width + 10, pos.y));
                        _diceResultLayer->addChild(to_value);
                }
        }
}

void GameScene::Fighting(){
        
        auto winner_back = CallFunc::create(std::bind(&GameScene::WinnerBack, this));
        
        auto fight_cloud = _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0];
        _animationLayer->addChild(fight_cloud);
        
        auto size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        
        fight_cloud->setPosition(Vec2(size.width / 2, size.height * 0.4));
        fight_cloud->setScale(2.0f);
        auto anim_cloud = cache->getAnimation("finght_cloud")->clone();
        anim_cloud->setRestoreOriginalFrame(true);
        auto anim_action = Animate::create(anim_cloud);
        
        this->playSoundEffect(EFFECT_BATTLE_IN_ANIM);
        fight_cloud->runAction(Sequence::create(anim_action, winner_back, NULL));
}

void GameScene::afterShowFightBg(){
        Size back_size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        
        //FIGHTER------------------------
        this->playSoundEffect(EFFECT_MARCH_IN_ANIM);
{
        int player_uid = _attackResult->_fromPlayer;
        int charactor_idx = _curGameData->_player[player_uid]->getPosCharactorIdx();
        auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[charactor_idx])->clone();
        run_anim->setRestoreOriginalFrame(true);
        auto run_action = Animate::create(run_anim);
        auto move = MoveBy::create(0.5, Vec2(READY_DISTANCE_POS,0));
        Spawn* get_ready = Spawn::create(run_action, FlipX::create(false), move, NULL);
        
        
        
        auto stand_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[charactor_idx])->clone();
        stand_wait->setRestoreOriginalFrame(true);
        auto fight_wait = Animate::create(stand_wait);
        
        
        auto moveby = MoveBy::create(1.f, Vec2(back_size.width / 2 - READY_DISTANCE_POS,0));
        auto run_anim2 = run_anim->clone();
        run_anim2->setLoops(2);
        auto run_to_fight = Spawn::create(Animate::create(run_anim2), moveby, NULL);
        
        Sequence* invade_seq = Sequence::create(get_ready, fight_wait,  run_to_fight, NULL);
        
        for (int i = 0; i < _attackResult->_from.size(); i++){
                
                auto invader = _allFightingCharacters[charactor_idx][i];
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
        int charactor_idx = _curGameData->_player[player_uid]->getPosCharactorIdx();
        auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[charactor_idx])->clone();
        run_anim->setRestoreOriginalFrame(true);
        
        auto move = MoveBy::create(0.5, Vec2(-READY_DISTANCE_POS,0));
        Spawn* get_ready = Spawn::create(Animate::create(run_anim), FlipX::create(true), move, NULL);
        
        
        auto stand_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[charactor_idx]);
        stand_wait->setRestoreOriginalFrame(true);
        auto fight_wait = Animate::create(stand_wait);
        
        
        auto moveby = MoveBy::create(1.f, Vec2(READY_DISTANCE_POS - back_size.width / 2 ,0));
        auto run_anim2 = run_anim->clone();
        run_anim2->setLoops(2);
        auto run_to_fight = Spawn::create(Animate::create(run_anim2), moveby, NULL);
        
        Sequence* keeper_seq = Sequence::create(get_ready, fight_wait, run_to_fight, NULL);
        
        for (int i = 0; i < _attackResult->_to.size(); i++){
                auto keeper = _allFightingCharacters[charactor_idx][i];
                _animationLayer->addChild(keeper);
                keeper->setPosition(_keeperPos[i] + Vec2(READY_DISTANCE_POS, 0));
                keeper->runAction(keeper_seq->clone());
        }
}
}

void GameScene::playBattleAnimation(bool isMaunual){
        
        _endTurnTipsLayer->setVisible(false);
        _isPalyingAnim = true;
        if (_animationIsOn && isMaunual){
                _animationLayer->setVisible(true);
                auto show = ScaleTo::create(.4f, 1.0f);
                auto cc = CallFunc::create(std::bind(&GameScene::afterShowFightBg, this));
                _animationLayer->runAction(Sequence::create(show, cc, NULL));
                
        }else{
                this->ShowResultData();
                this->afterFightFinished();
        }
}

void GameScene::playSupplyAnimation2(GamePlayer* player){
        
        std::map<AreaData*, int> supply_data = _theGameLogic->starSupplyDice(player);
        if (supply_data.size() == 0){
                _afterSupplyCallback->execute();
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
                auto move = MoveTo::create(0.8f, rel_pos);
                std::map<AreaData*, int>::iterator it_end = supply_data.end();
                it_end--;
                
                bool is_t_last = it == it_end; 
                for (int i = idx; i < idx + sup_num; i++){
                        Node* sup_one = supp_nodes.at(i);
                        
                        bool is_last = i == idx + sup_num - 1;
                        
                        auto  cb = CallFunc::create([this, sup_one, is_t_last, is_last, cur_area](){

                                if (is_last){
                                        cur_area->updatePawn(_curGameData->_referedLayer);
                                        if (is_t_last){
                                                cur_area->drawSupply(_curGameData->_referedLayer, _afterSupplyCallback);
                                        }else{
                                                cur_area->drawSupply(_curGameData->_referedLayer, NULL);
                                        }
                                }
                        });
                        sup_one->runAction(Sequence::create(move->clone(), cb, NULL));
                }
                idx += sup_num;
        }
}

void GameScene::playSupplyAnimation(){
        this->playSoundEffect(EFFECT_FILE_SUPPLY);
        _supplyShowLayer->setVisible(true);
        int player_id = _curGameData->_jun[_curGameData->_ban];
        GamePlayer* player = _curGameData->_player[player_id];
        int total_stock = player->setStock();
        
        auto back_size = _supplyShowLayer->getContentSize();
        for (int i = 0; i < total_stock; i++){
                int ch_idx = player->getPosCharactorIdx();
                std::string charact_name = CHARACTER_NAME[ch_idx];
                float scal_factor = 0.5f;
                auto character = Sprite::create(charact_name);
                auto ch_size = character->getContentSize() * scal_factor;
                character->setScale(scal_factor);
                Vec2 pos;
                if (i / AREA_MAX > 0){
                        pos.x = (2 * (i - AREA_MAX) + 1) * 0.5f * ch_size.width * 0.7;
                        pos.y = back_size.height * 0.25f;
                }else{
                        pos.x = (2 * i + 1) * 0.5f * ch_size.width * 0.7;
                        pos.y = 3 * back_size.height * 0.25f;
                }
                character->setPosition(pos);
                _supplyShowLayer->addChild(character);
        }
        if (total_stock > 0){ 
                auto cb = CallFunc::create(std::bind(&GameScene::playSupplyAnimation2, this, player));
                _supplyShowLayer->runAction(Sequence::create(DelayTime::create(0.6f), cb, NULL));
        }else{
                this->playSupplyAnimation2(player);
        }
        
}

#pragma mark - menu callback actions
void GameScene::menuEndTurn(Ref* pSender){
        if (_isPalyingAnim){
                return;
        }
        
        _theGameLogic->clearManulAction();
        this->playSoundEffect(EFFECT_FILE_START_GAME);
        
        
        _addtionalSupplyCounter--;
        ui::TextBMFont* counter_lab = (ui::TextBMFont*)_addArmyBtn->getChildByTag(key_supply_turn_counter_tag);
        
        if (_addtionalSupplyCounter == 0){
                _addArmyBtn->setEnabled(true);
                _addArmyBtn->setBright(true);
                counter_lab->setVisible(false);
        }else{
                
                counter_lab->setString(StringUtils::format("%d", _addtionalSupplyCounter));
        }
        
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        _mapLayer->setPosition(visible_size);
        
        auto scale = ScaleTo::create(0.3, 1.f);
        auto callback_1 = CallFunc::create( [&](){
                _endTurnTipsLayer->setVisible(false);
                _gameStatus = GAME_STATUS_AIRUNNING;
                
                _afterSupplyCallback = CallFunc::create(std::bind(&GameScene::afterSupply, this));
                _afterSupplyCallback->retain();
                
                this->playSupplyAnimation();
                
        });
        
        _isPalyingAnim = true;
        _mapLayer->runAction(Sequence::create(scale, callback_1, NULL));
}

void GameScene::createNewMap(Ref* pSender){
        this->playSoundEffect(EFFECT_FILE_CHANGE_MAP);
        _curGameData->release();
        _curGameData = _theGameLogic->initGameData(_playerNumber, _charactorIdx, _colorIdx);
        this->removeChildByTag(key_map_back_layer);
        this->initMapSize(_curGameData);
        
        this->refreshSupplyDiceNum();
}

void GameScene::menuStartGame(Ref* pSender, Layer* parent){
        this->playSoundEffect(EFFECT_FILE_START_GAME);
        this->initAreaTcShow();
        _theGameLogic->initHistoryRecord();
        parent->setVisible(false);
        this->playSoundEffect(EFFECT_FILE_START_GAME);
        _gameStatus = GAME_STATUS_AIRUNNING;
        this->gameAction();
}

void GameScene::menuExit(Ref* pSender){
        this->playSoundEffect();
        CommonTipsDialog::showModalDialog((Node*)this, "Are you sure to exit ?", [this](Ref* sender){
                this->playSoundEffect();
                CommonTipsDialog::dismissDialog(this);
                Director::getInstance()->popScene();
        });
}

void GameScene::gameOver(Ref* btn, int result){
        this->playSoundEffect();
        if (result == 0){
                _theGameLogic->finishHistoryRecord();
                Director::getInstance()->popScene();
        }else{
                this->removeChildByTag(key_dialog_layer_tag);
                this->tryAgain();
        }
}

void GameScene::afterCaptureScreen(bool yes, const std::string &outputFilename)
{
        if (!outputFilename.empty() && FileUtils::getInstance()->isFileExist(outputFilename)){
                CCLOG("##FB afterCaptureScreen: %s", outputFilename.c_str());
                if (yes) {
                        
                        sdkbox::FBShareInfo info;
                        info.type  = sdkbox::FB_PHOTO;
                        info.title = "Islands Conqueror";
                        info.link = "https://itunes.apple.com/us/app/island-conqueror/id1172744843?l=zh&ls=1&mt=8";
                        info.image = outputFilename;
                        sdkbox::PluginFacebook::share(info);
                }else{
                        sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
                }
        }
        
}

void GameScene::shareThisGame(Ref* btn){
        this->playSoundEffect();
        if (sdkbox::PluginFacebook::isLoggedIn()){
//                utils::captureScreen(CC_CALLBACK_2(GameScene::afterCaptureScreen, this), "screen.png");
                
                sdkbox::FBShareInfo info;
                info.type  = sdkbox::FB_PHOTO;
                info.title = "Islands Conqueror";
                info.link = "https://itunes.apple.com/us/app/island-conqueror/id1172744843?l=zh&ls=1&mt=8";
                
                info.image = FileUtils::getInstance()->fullPathForFilename("fb_used_toshare.png");
                sdkbox::PluginFacebook::dialog(info);
                
        }else{
                std::vector<std::string> permissions;
                permissions.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
                permissions.push_back(sdkbox::FB_PERM_READ_EMAIL);
                permissions.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
                sdkbox::PluginFacebook::login(permissions);
        }
}

void GameScene::menuAnimSwitch(Ref* btn){
        this->playSoundEffect();
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
        AchievementEngine::getInstance()->openReward(ACHIEVE_DATA_KEY_FIRST_CLOSE_ANIM);
}

void GameScene::playAddMercenary(){
        AchievementEngine::getInstance()->openReward(ACHIEVE_DATA_KEY_FIRST_USE_MERCENARY);
        
        _isPalyingAnim = true;
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
                this->refreshSupplyDiceNum();
                _isPalyingAnim = false;
                auto scale_s = ScaleTo::create(0.3, 1.4f);
                auto scale_s_r = ScaleTo::create(0.3, 1.0f);
                auto seq = Sequence::create(scale_s, scale_s_r, NULL);
                _curPlayerSupFlag->runAction(seq);
                _curInTurnBack->runAction(seq->clone());
                
        }), NULL));
        
        _addArmyBtn->setEnabled(false);
        _addArmyBtn->setBright(false);
        ui::TextBMFont* counter_lab = (ui::TextBMFont*)_addArmyBtn->getChildByTag(key_supply_turn_counter_tag);
        _addtionalSupplyCounter = TURN_INTERVAL_FOR_SUPPLEMENTS;
        counter_lab->setString(StringUtils::format("%d", TURN_INTERVAL_FOR_SUPPLEMENTS));
        counter_lab->setVisible(true);
}

void GameScene::menuAddArmy(Ref* btn){
        
        if (GAME_STATUS_INUSERTURN != _gameStatus
            ||_addtionalSupplyCounter > 0){
                return;
        }
        this->playSoundEffect();
        auto cache = UserDefault::getInstance();
        int cur_mercenary = cache->getIntegerForKey(USER_CURRENT_SUPPLY_NO,
                                                    USER_DEFAULT_SUPPLYNO_ONFIRST);
        int cur_coinsNo = cache->getIntegerForKey(USER_CURRENT_COINS, 0);
        
        if (cur_mercenary <= 0){
                if (cur_coinsNo < PRICE_PER_SUPPLEMENT){
                        std::string tips = StringUtils::format("Tips：sorry, you don't have enough coins(%d) to engage one team of mercenaries(cost:%d coions). You can get more coions through these ways:win more battles, share the game, buy from shop.",  cur_coinsNo,
                                                               PRICE_PER_SUPPLEMENT
                                                               );
                        CommonTipsDialog::showModalDialog((Node*)this, tips);
                        return;
                }else{
                        std::string tips = StringUtils::format("Tips：youre mercenaries are used up, you need engage more mercenaries by coins. Do you want to spend %d coins to engage one team of mercenaries ? Your current coins number is :%d", 
                                                               PRICE_PER_SUPPLEMENT,
                                                               cur_coinsNo);
                        
                        cur_coinsNo -= PRICE_PER_SUPPLEMENT;
                        CommonTipsDialog::showModalDialog((Node*)this, tips, [this, cache, cur_coinsNo](Ref* sender){
                                CommonTipsDialog::dismissDialog(this);
                                cache->setIntegerForKey(USER_CURRENT_COINS, cur_coinsNo);
                                cache->flush();
                                this->playAddMercenary();
                        });
                }
        }else{
                cache->setIntegerForKey(USER_CURRENT_SUPPLY_NO, --cur_mercenary);
                cache->flush();
                this->playAddMercenary();
        }
}

#pragma mark - facebook share delegate
void GameScene::onLogin(bool, const std::string&){
        
}
void GameScene::onSharedSuccess(const std::string& infos){
        CCLOG("===onSharedSuccess=%s", infos.c_str());
        
        _winDialogLayer->removeFromParent();
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto from = visible_size * 0.5f;
        auto call_back = CallFunc::create([this](){
                _theGameLogic->finishHistoryRecord();
                Director::getInstance()->popScene();
        });
        
        int rewards = AchievementEngine::getInstance()->dailyShareReward(_controlLayer, from, visible_size, call_back);
        
        if (rewards == 0){
                _theGameLogic->finishHistoryRecord();
                Director::getInstance()->popScene();
                return;
        }
}
void GameScene::onSharedFailed(const std::string& infos){
        CCLOG("===onSharedFailed=%s", infos.c_str());
}
void GameScene::onSharedCancel(){
        CCLOG("===onSharedCancel=......");
}
void GameScene::onAPI(const std::string&, const std::string&){
        
}
void GameScene::onPermission(bool, const std::string&){
        
}
void GameScene::onFetchFriends(bool, const std::string&){
        
}
void GameScene::onRequestInvitableFriends(const sdkbox::FBInvitableFriendsInfo&){
        
}
void GameScene::onInviteFriendsWithInviteIdsResult(bool, const std::string&){
        
}
void GameScene::onInviteFriendsResult(bool, const std::string&){
        
}
void GameScene::onGetUserInfo(const sdkbox::FBGraphUser&){
        
}

#pragma mark - on screen show and disapear

void GameScene::onEnter(){
        Layer::onEnter();
        
        auto cache      = UserDefault::getInstance();
        int game_speed  = cache->getIntegerForKey(GAME_SPEED_KEY, 1);
        Director::getInstance()->getScheduler()->setTimeScale(game_speed);
        
        _soundTotalOn = cache->getBoolForKey(SOUND_MUSIC_TOTAL_KEY, true);
        _musicSwitch = cache->getBoolForKey(BACK_MUSIC_SWITCH_KEY);
        _soundEngine = CocosDenshion::SimpleAudioEngine::getInstance();
        _soundEngine->stopBackgroundMusic();
        _soundEngine->stopAllEffects();

        auto sound = CocosDenshion::SimpleAudioEngine::getInstance();
        if (_soundTotalOn && _musicSwitch){
                sound->playBackgroundMusic(BACK_MUSIC_IN_BATTLE, true);
        }
}

void GameScene::update(float delta){
}


void GameScene::onExit(){
        Layer::onExit(); 
        Director::getInstance()->getScheduler()->setTimeScale(1);
        auto sound = CocosDenshion::SimpleAudioEngine::getInstance();
        sound->stopBackgroundMusic();
        sound->stopAllEffects();
}
