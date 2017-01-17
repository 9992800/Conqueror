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

USING_NS_CC;

enum{
        GUIDE_GAME_STATUS_INIT = 1,
        GUIDE_GAME_STATUS_START,
        GUIDE_GAME_STATUS_SELECT_SELF,
        GUIDE_GAME_STATUS_SELECT_ENEMY,
        GUIDE_GAME_STATUS_SELECT_WIN,
        GUIDE_GAME_STATUS_SELECT_SUPPLY
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
        
        this->initTopTittle();
        
        this->addChild(_controlLayer);
        return true;
}

NoviceGuide::~NoviceGuide(){
}

#pragma mark - init logic
void NoviceGuide::initMap(){
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
        _choseMapLayer->addChild(OK_btn);
        
        
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
}

void NoviceGuide::initGuideData(){
}

void NoviceGuide::initTopTittle(){
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
}

void NoviceGuide::menuEndTurn(Ref* pSender){
        
}
