//
//  NoviceGuideScene.cpp
//  Conqueror
//
//  Created by wsli on 17/1/16.
//
//

#include "ui/CocosGUI.h"
#include "NoviceGuideScene.hpp"
#include "AppMacros.hpp"
#include "LevelSelectScene.hpp"

enum{
        GUIDE_GAME_STATUS_INIT = 1,
        GUIDE_GAME_STATUS_START,
        GUIDE_GAME_STATUS_SELECT_SELF,
        GUIDE_GAME_STATUS_SELECT_ENEMY,
        GUIDE_GAME_STATUS_SELECT_WIN,
        GUIDE_GAME_STATUS_SELECT_SUPPLY,
        
        
        kChoseMapYesTag = 100,
};

Scene* NoviceGuide::createScene(){
        
        auto scene = Scene::create();
        auto layer = NoviceGuide::create();
        scene->addChild(layer);
        
        return scene;
}

bool NoviceGuide::init(){
        if (!Layer::init()){
                return false;
        }
        
        this->initMap();
        
        this->initController();
        
        this->initGuideData();
        
        return true;
}

NoviceGuide::~NoviceGuide(){
        _handsUpDownAnim->release();
}

#pragma mark - init logic
void NoviceGuide::initMap(){
        auto back_ground = LayerColor::create(TILE_COLOR_BACKGRUND);
        this->addChild(back_ground, 1);
}

void NoviceGuide::initController(){
        auto visible_size = Director::getInstance()->getVisibleSize();
        _controlLayer = Layer::create();
        auto exit_btn = ui::Button::create("CloseNormal.png", "CloseSelected.png");
        exit_btn->setPosition(Vec2(exit_btn->getContentSize().width / 2,
                                   visible_size.height - exit_btn->getContentSize().height / 2));
        exit_btn->addClickEventListener(CC_CALLBACK_1(NoviceGuide::menuExit, this));
        _controlLayer->addChild(exit_btn);
        
        auto operat_board = Sprite::create("maps/openrate_back.png");
        operat_board->setPosition(Vec2(visible_size.width / 2, operat_board->getContentSize().height / 2));
        _controlLayer->addChild(operat_board);
        
        
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
        
        
        auto anim_btn = cocos2d::ui::Button::create("maps/open_anim.png", "maps/open_anim_sel.png");
        
        
        anim_btn->setPosition(operat_board_l->getContentSize() / 2);
        operat_board_l->addChild(anim_btn);
        
        auto operat_board_r = Sprite::create("maps/openrate_board_btn.png");
        operat_board_r->setPosition(Vec2(operat_board->getContentSize().width - operat_board_r->getContentSize().width * 0.6,
                                         operat_board_r->getContentSize().height * 0.6));
        operat_board->addChild(operat_board_r, 1);
        
        auto mercenary_btn = cocos2d::ui::Button::create("maps/addtion_supply_arm.png",
                                                  "maps/addtion_supply_arm.png");
        mercenary_btn->setPosition(operat_board_r->getContentSize() / 2);
        mercenary_btn->addClickEventListener(CC_CALLBACK_1(NoviceGuide::menuEngageArmy, this));
        operat_board_r->addChild(mercenary_btn);
        
        
        _choseMapLayer = Layer::create();
        _choseMapLayer->setContentSize(operat_board_m->getContentSize());
        _choseMapLayer->setPosition(operat_board_m->getContentSize() / 2);
        _choseMapLayer->setIgnoreAnchorPointForPosition(false);
        _choseMapLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        auto tips = Label::createWithSystemFont("Do you want to use this map?", "fonts/arial.ttf", 40);
        tips->setPosition(Vec2(tips->getContentSize().width / 2 + 20,
                               _choseMapLayer->getContentSize().height / 2));
        _choseMapLayer->addChild(tips);
        
        operat_board_m->addChild(_choseMapLayer);
        
        auto OK_btn = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        OK_btn->setTitleText("YES");
        OK_btn->setTitleFontSize(30);
        OK_btn->addClickEventListener(CC_CALLBACK_1(NoviceGuide::menuStartGame, this));
        OK_btn->setPosition(Vec2(tips->getContentSize().width + 0.7f * OK_btn->getContentSize().width,
                                 operat_board_m->getContentSize().height / 2));
        _choseMapLayer->addChild(OK_btn, 1, kChoseMapYesTag);
        
        
        auto NO_btn = cocos2d::ui::Button::create("DIALOG_CANCEL.png", "DIALOG_CANCEL_SEL.png");
        NO_btn->setTitleText("NO");
        NO_btn->setTitleFontSize(30);
        NO_btn->setPosition(Vec2(OK_btn->getPosition().x + NO_btn->getContentSize().width * 1.1f,
                                 operat_board_m->getContentSize().height / 2));
        _choseMapLayer->addChild(NO_btn);
        
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
        end_turn_btn->addClickEventListener(CC_CALLBACK_1(NoviceGuide::menuEndTurn, this));
        end_turn_btn->setPosition(Vec2(operat_board_m->getContentSize().width - 48 - end_turn_btn->getContentSize().width / 2,
                                       operat_board_m->getContentSize().height / 2));
        _endTurnTipsLayer->addChild(end_turn_btn);
        
        
        _diceResultLayer = Layer::create();
        _diceResultLayer->setIgnoreAnchorPointForPosition(false);
        _diceResultLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _diceResultLayer->setContentSize(operat_board_m->getContentSize());
        _diceResultLayer->setPosition(operat_board_m->getContentSize() / 2);
        operat_board_m->addChild(_diceResultLayer);
        _diceResultLayer->setVisible(false);
        
        
        _supplyShowLayer = Layer::create();
        _supplyShowLayer->setIgnoreAnchorPointForPosition(false);
        _supplyShowLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _supplyShowLayer->setContentSize(operat_board_m->getContentSize());
        _supplyShowLayer->setPosition(operat_board_m->getContentSize() / 2);
        operat_board_m->addChild(_supplyShowLayer);
        _supplyShowLayer->setVisible(false);
        
        
        _tcShowMe = Sprite::create("maps/supply_back_0.png");
        auto p_size = _tcShowMe->getContentSize();
        auto roll = cocos2d::ui::Scale9Sprite::create("maps/supply_back_roll.png");
        
        auto bakc_size = Size(42 + p_size.width * 2,
                              roll->getContentSize().height);
        roll->setContentSize(bakc_size);
        roll->setCapInsets(Rect(21, 6, 27, 14));
        roll->setPosition(Vec2(visible_size.width / 2,
                               visible_size.height - bakc_size.height/2));
        
       
        
        _tcShowMe->setPosition(Vec2(21 +  0.5  * p_size.width,  roll->getContentSize().height - p_size.height / 2));
        roll->addChild(_tcShowMe, 1);
        auto character_me = Sprite::create("zhanshi_pos.png");
        character_me->setScale(0.7);
        auto ch_size = character_me->getContentSize();

        auto numbser = Label::createWithSystemFont("X4", "fonts/arial.ttf", 26);
        character_me->addChild(numbser);
        numbser->setPosition(Vec2(ch_size.width, ch_size.height / 2));
        character_me->setPosition(p_size.width * 0.4,  p_size.height * 0.7);
        _tcShowMe->addChild(character_me);
        
        
        
        _tcShowEnemy = Sprite::create("maps/supply_back_1.png");
        _tcShowEnemy->setPosition(Vec2(21 +  1.5  * p_size.width,  roll->getContentSize().height - p_size.height / 2));
        roll->addChild(_tcShowEnemy, 1);
        auto character_enemy = Sprite::create("xunshoushi_pos.png");
        character_enemy->setScale(0.7f);
        
        auto numbser_e = Label::createWithSystemFont("X3", "fonts/arial.ttf", 26);
        character_enemy->addChild(numbser_e);
        numbser_e->setPosition(Vec2(ch_size.width, ch_size.height / 2));
        character_enemy->setPosition(p_size.width * 0.4,  p_size.height * 0.7);
        _tcShowEnemy->addChild(character_enemy);
        
        
        _controlLayer->addChild(roll);
        
        this->addChild(_controlLayer, 2);
}

void NoviceGuide::initGuideData(){
        
        _curGuideState = GUIDE_GAME_STATUS_INIT;
        
        auto layer_size = Director::getInstance()->getVisibleSize() * 0.4f;
        _guideLayer = cocos2d::ui::Scale9Sprite::create("DIALOG_BACKGROUND.png");
        _guideLayer->setContentSize(layer_size);
        _guideLayer->setCapInsets(Rect(12, 12, 40, 40));
        _guideLayer->setPosition(layer_size/2);
        this->addChild(_guideLayer, 3);
        
        _contentText = Label::createWithSystemFont("    Click 'YES' to select this map as your battle field, the map is created randomly. If you don't like this map, you can click 'NO' to recreate a new map. In this guide, click 'YES' to start game please.", "fonts/arial.ttf", 24);
        _guideLayer->addChild(_contentText);
        Size content_size = _contentText->getContentSize();
        _contentText->setAnchorPoint(Vec2(0.f, 1.0f));
        _contentText->setPosition(Vec2(50, layer_size.height - 30));
        
        _contentText->setDimensions(layer_size.width - 100,
                                layer_size.height - 30);
        
        _contentText->setHorizontalAlignment(TextHAlignment::LEFT);
        
        _guideHand = Sprite::create("tips_hands.png");
        this->addChild(_guideHand, 4);
        
        auto yes_btn = _choseMapLayer->getChildByTag(kChoseMapYesTag);
        auto pos = _choseMapLayer->convertToWorldSpace(yes_btn->getPosition());
        auto pos2 = this->convertToNodeSpace(pos);
        _guideHand->setPosition(Vec2(pos2.x, pos2.y - yes_btn->getContentSize().height));
        
        auto hands_up = MoveBy::create(0.2f, Vec2(0, 40));
        _handsUpDownAnim = RepeatForever::create(Sequence::create(hands_up, hands_up->reverse(), NULL));
        _handsUpDownAnim->retain();
        _guideHand->runAction(_handsUpDownAnim->clone());
        
        _guideLayer->setPosition(pos2 + Vec2(0, layer_size.height));
}

#pragma mark -  update logic
void NoviceGuide::onEnter(){
        Layer::onEnter();
}

void NoviceGuide::update(float delta){
}

void NoviceGuide::onExit(){
        Layer::onExit();
}


#pragma mark - menu actions.
void NoviceGuide::menuExit(Ref* pSender){ 
        auto main = LevelSelect::createScene();
        Director::getInstance()->replaceScene(main);
}

void NoviceGuide::menuEngageArmy(Ref* pSender){
}

void NoviceGuide::menuStartGame(Ref* pSender){
        _choseMapLayer->removeFromParent();
        _endTurnTipsLayer->setVisible(true);
        _curGuideState = GUIDE_GAME_STATUS_START;
}

void NoviceGuide::menuEndTurn(Ref* pSender){
        if (GUIDE_GAME_STATUS_SELECT_SUPPLY != _curGuideState)
                return;         
}
