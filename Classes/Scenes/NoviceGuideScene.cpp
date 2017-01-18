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
        GUIDE_GAME_STATUS_SHOW_MYRES,
        GUIDE_GAME_STATUS_SHOW_ENRES,
        GUIDE_GAME_STATUS_SHOW_NEWTC,
        GUIDE_GAME_STATUS_ENGAGE_MERCENARY,
        GUIDE_GAME_STATUS_SUPPLY,
        
        
        kChoseMapYesTag = 100,
        kGuideLayerNextBtnTag,
        
        ZORDER_BACK_LEVEL,
        ZORDER_FRONT_LEVEL,
        ZORDER_SHINNING_LEVEL,
        ZORDER_ACTION_LEVEL
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
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto back_ground = LayerColor::create(TILE_COLOR_BACKGRUND);
        this->addChild(back_ground, 1);
        auto back_layer_size = back_ground->getContentSize();
        
        auto guide_back_img = Sprite::create("guide/guid_backgrd.png");
        guide_back_img->setPosition(Vec2(back_layer_size.width * 0.5f,
                                         back_layer_size.height * 0.56f));
        back_ground->addChild(guide_back_img);
        
        _tcMapShineBeforeAction = Sprite::create("guide/shine_char_me_tc_8.png");
        _tcMapShineBeforeAction->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_tcMapShineBeforeAction, ZORDER_SHINNING_LEVEL);
        
        _tcMapShineafterAction = Sprite::create("guide/shine_char_me_tc_9.png");
        _tcMapShineafterAction->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_tcMapShineafterAction, ZORDER_SHINNING_LEVEL);
        
        _meShineBack = Sprite::create("guide/shine_char_me.png");
        _meShineBack->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_meShineBack, ZORDER_SHINNING_LEVEL);
        
        _enemyShineBack = Sprite::create("guide/shine_target_enemy.png");
        _enemyShineBack->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_enemyShineBack, ZORDER_SHINNING_LEVEL);
        
        _tcMapShineBeforeAction->setOpacity(0);
        auto fade_in = FadeOut::create(.8f);
        auto shining = RepeatForever::create(Sequence::create(fade_in, fade_in->reverse(), NULL));
        
        _tcMapShineafterAction->setVisible(false);
        _tcMapShineBeforeAction->setVisible(false);
        _enemyShineBack->setVisible(false);
        _meShineBack->setVisible(false);
        
        _enemyShineBack->runAction(shining->clone());
        _meShineBack->runAction(shining->clone());
        _tcMapShineBeforeAction->runAction(shining->clone());
        _tcMapShineafterAction->runAction(shining);
        
        _beforeActionFromMe = Sprite::create("guide/char_me_s.png");
        _beforeActionFromMe->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_beforeActionFromMe, ZORDER_BACK_LEVEL);
        
        _afterActionMe = Sprite::create("guide/char_me_e.png");
        _afterActionMe->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_afterActionMe, ZORDER_BACK_LEVEL);
        _afterActionMe->setVisible(false);
        
        _befroeActionTargetEnemy = Sprite::create("guide/target_enemy_s.png");
        _befroeActionTargetEnemy->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_befroeActionTargetEnemy, ZORDER_BACK_LEVEL);
        
        _afterActionEnmey = Sprite::create("guide/target_enemy_e.png");
        _afterActionEnmey->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_afterActionEnmey, ZORDER_FRONT_LEVEL);
        _afterActionEnmey->setVisible(false);
        
        _afterActionEnmeyBg = Sprite::create("guide/char_me_tc_bg.png");
        _afterActionEnmeyBg->setPosition(visible_size * 0.5f);
        guide_back_img->addChild(_afterActionEnmeyBg, ZORDER_BACK_LEVEL);
        _afterActionEnmeyBg->setVisible(false);
        
        _selectFrom = ui::Button::create("level/sel_num_btn_back.png");//TODO::sel_num_btn_back_em
        _selectFrom->addClickEventListener(CC_CALLBACK_1(NoviceGuide::choseFromArea, this));
        _selectFrom->setPosition(Vec2(visible_size.width *0.25f, visible_size.height * 0.7f));
        guide_back_img->addChild(_selectFrom, ZORDER_ACTION_LEVEL);
        
        _targetTo = ui::Button::create("level/sel_num_btn_back.png");//TODO::sel_num_btn_back_em
        _targetTo->addClickEventListener(CC_CALLBACK_1(NoviceGuide::choseToArea, this));
        _targetTo->setPosition(Vec2(visible_size.width *0.4f, visible_size.height * 0.75f));
        guide_back_img->addChild(_targetTo, ZORDER_ACTION_LEVEL);
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
        
        _mercenaryBtn = cocos2d::ui::Button::create("maps/addtion_supply_arm.png",
                                                  "maps/addtion_supply_arm.png");
        _mercenaryBtn->setPosition(operat_board_r->getContentSize() / 2);
        _mercenaryBtn->addClickEventListener(CC_CALLBACK_1(NoviceGuide::menuEngageArmy, this));
        operat_board_r->addChild(_mercenaryBtn);
        
        
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
        
        _endTurnBtn = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        _endTurnBtn->setTitleText("END TURN");
        _endTurnBtn->setTitleFontSize(22);
        _endTurnBtn->addClickEventListener(CC_CALLBACK_1(NoviceGuide::menuEndTurn, this));
        _endTurnBtn->setPosition(Vec2(operat_board_m->getContentSize().width - 48 - _endTurnBtn->getContentSize().width / 2,
                                       operat_board_m->getContentSize().height / 2));
        _endTurnTipsLayer->addChild(_endTurnBtn);
        
        
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

        _tcShowNumbMe = Label::createWithSystemFont("X8", "fonts/arial.ttf", 26);
        character_me->addChild(_tcShowNumbMe);
        _tcShowNumbMe->setPosition(Vec2(ch_size.width, ch_size.height / 2));
        character_me->setPosition(p_size.width * 0.4,  p_size.height * 0.7);
        _tcShowMe->addChild(character_me);
        
        

        _tcShowEnemy = Sprite::create("maps/supply_back_1.png");
        _tcShowEnemy->setPosition(Vec2(21 +  1.5  * p_size.width,  roll->getContentSize().height - p_size.height / 2));
        roll->addChild(_tcShowEnemy, 1);
        auto character_enemy = Sprite::create("xunshoushi_pos.png");
        character_enemy->setScale(0.7f);
        
        _tcShowNumbEnemy = Label::createWithSystemFont("X6", "fonts/arial.ttf", 26);
        character_enemy->addChild(_tcShowNumbEnemy);
        _tcShowNumbEnemy->setPosition(Vec2(ch_size.width, ch_size.height / 2));
        character_enemy->setPosition(p_size.width * 0.4,  p_size.height * 0.7);
        _tcShowEnemy->addChild(character_enemy);
        
        
        _controlLayer->addChild(roll);
        
        this->addChild(_controlLayer, 2);
}

void NoviceGuide::initGuideData(){
        
        _curGuideState = GUIDE_GAME_STATUS_INIT;
        auto visible_size = Director::getInstance()->getVisibleSize();
        Size layer_size(visible_size.width * 0.4f, visible_size.height * 0.3f);
        _guideLayer = cocos2d::ui::Scale9Sprite::create("DIALOG_BACKGROUND.png");
        _guideLayer->setContentSize(layer_size);
        _guideLayer->setCapInsets(Rect(12, 12, 40, 40));
        this->addChild(_guideLayer, 3);
        
        _contentText = Label::createWithSystemFont("Click 'YES' to select this map as your battle field, the map is created randomly.", "fonts/arial.ttf", 24);
        _guideLayer->addChild(_contentText);
        Size content_size = _contentText->getContentSize();
        _contentText->setAnchorPoint(Vec2(0.f, 1.0f));
        _contentText->setPosition(Vec2(50, layer_size.height - 30));
        
        _contentText->setDimensions(layer_size.width - 100,
                                layer_size.height - 30);
        
        _contentText->setHorizontalAlignment(TextHAlignment::LEFT);
        
        _guideHandUpDown = Sprite::create("tips_hands.png");
        this->addChild(_guideHandUpDown, 4);
        
        auto yes_btn = _choseMapLayer->getChildByTag(kChoseMapYesTag);
        auto pos = _choseMapLayer->convertToWorldSpace(yes_btn->getPosition());
        auto pos2 = this->convertToNodeSpace(pos);
        _guideHandUpDown->setPosition(Vec2(pos2.x, pos2.y - yes_btn->getContentSize().height));
        
        auto hands_up = MoveBy::create(0.2f, Vec2(0, 40));
        _handsUpDownAnim = RepeatForever::create(Sequence::create(hands_up, hands_up->reverse(), NULL));
        _handsUpDownAnim->retain();
        _guideHandUpDown->runAction(_handsUpDownAnim->clone());
        
        _guideHandLeftRight = Sprite::create("tips_hands2.png");
        this->addChild(_guideHandLeftRight, 4);
        _guideHandLeftRight->setVisible(false);
        
        auto hands_left = MoveBy::create(0.2f, Vec2(40, 0));
        _handsLeftRightAnim = RepeatForever::create(Sequence::create(hands_left,hands_left->reverse(), NULL));
        _handsLeftRightAnim->retain();
        _guideHandLeftRight->runAction(_handsLeftRightAnim);
        
        _guideLayer->setPosition(pos2 + Vec2(0, 0.7f * layer_size.height));
        
        _nextButton = ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        auto but_size = _nextButton->getContentSize();
        _nextButton->setPosition(Vec2(layer_size.width - 0.55f * but_size.width,
                                      but_size.height * 0.55f));
        _nextButton->setTitleText("Next");
        _nextButton->setTitleFontName("fonts/arial.ttf");
        _nextButton->setTitleFontSize(24);
        _nextButton->setVisible(false);
        _guideLayer->addChild(_nextButton, 1, kGuideLayerNextBtnTag);
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
        if (GUIDE_GAME_STATUS_ENGAGE_MERCENARY != _curGuideState)
                return;
        
        
        
        auto btn_anim = _mercenaryBtn->clone();
        auto parent = _mercenaryBtn->getParent();
        btn_anim->setPosition(_mercenaryBtn->getPosition());
        parent->addChild(btn_anim);
        Vec2 pos = parent->convertToNodeSpace(_tcShowMe->getParent()->convertToWorldSpace(_tcShowMe->getPosition()));
        auto move = MoveTo::create(0.8,  pos);
        auto scale = ScaleTo::create(0.8, 0.6f);
        btn_anim->runAction(Sequence::create(Spawn::create(move, scale, NULL),
                                             CallFunc::create( [this, btn_anim](){
                
                btn_anim->removeFromParentAndCleanup(true);
                
                _tcShowNumbMe->setString("X19");
                auto scale_s = ScaleTo::create(0.3, 1.4f);
                auto scale_s_r = ScaleTo::create(0.3, 1.0f);
                auto seq = Sequence::create(scale_s, scale_s_r, NULL);
                _tcShowMe->runAction(seq);
                
                
                _curGuideState = GUIDE_GAME_STATUS_SUPPLY;
                
                _contentText->setString("Click this button to end your turn. Don't attack too much enemy's area, this will make your areas weak. Keep your area with more soldier then the enemy will not occupy your area easily.");
                auto pos1 = _endTurnBtn->getParent()->convertToWorldSpace(_endTurnBtn->getPosition());
                auto pos2 = this->convertToNodeSpace(pos1);
                _guideHandUpDown->setPosition(pos2);
                auto layer_size = _guideLayer->getContentSize();
                _guideLayer->setPosition(pos2 + Vec2(0, 0.7 * layer_size.height));
                
        }), NULL));
        
        _mercenaryBtn->setEnabled(false);
        _mercenaryBtn->setBright(false);
        _counterTurns = ui::TextBMFont::create("4", "fonts/zb_chongzhi_shuzi.fnt");
        _counterTurns->setPosition(_mercenaryBtn->getContentSize() * 0.5f);
        _mercenaryBtn->addChild(_counterTurns);
}

void NoviceGuide::menuStartGame(Ref* pSender){
        _choseMapLayer->removeFromParent();
        _endTurnTipsLayer->setVisible(true);
        _curGuideState = GUIDE_GAME_STATUS_START;
        _guideHandLeftRight->setVisible(true);
        _guideHandUpDown->setVisible(false);
        
        _contentText->setString("Here it shows the max number of  your adjoining area, it also means how many soldiers you can get after this turn.");
        
        auto scale_by = ScaleBy::create(0.8f, 1.2f);
        auto seq = Repeat::create(Sequence::create(scale_by, scale_by->reverse(), NULL), 5);
        
        _tcShowMe->runAction(seq);
        
        auto pos = _tcShowMe->getParent()->convertToWorldSpace(_tcShowMe->getPosition());
        
        auto pos2 = this->convertToNodeSpace(pos);
        _guideHandLeftRight->setPosition(Vec2(pos2.x - _tcShowMe->getContentSize().width, pos2.y));
        _guideLayer->setPosition(Vec2(pos2.x + 0.8f * _contentText->getContentSize().width, pos2.y - 0.8f *_contentText->getContentSize().height));
        _nextButton->addClickEventListener(CC_CALLBACK_1(NoviceGuide::showSelectGuide, this));
        _nextButton->setVisible(true);
        _tcMapShineBeforeAction->setVisible(true);
}

void NoviceGuide::menuEndTurn(Ref* pSender){
        if (GUIDE_GAME_STATUS_SUPPLY != _curGuideState)
                return;
        
        _counterTurns->setString("3");
//        _curGuideState = ;
}


void NoviceGuide::showSelectGuide(Ref* btn){
        _nextButton->setVisible(false);
        
        _contentText->setString("Click the area as invader to attack your enenmies neighbored on you.");
        
        _guideHandLeftRight->setVisible(false);
        _guideHandUpDown->setVisible(true);
        
        _curGuideState = GUIDE_GAME_STATUS_SELECT_SELF;
        
        _tcMapShineBeforeAction->setVisible(false);
        _meShineBack->setVisible(true);
        
        auto pos1 = _selectFrom->getParent()->convertToWorldSpace(_selectFrom->getPosition());
        auto pos2 = this->convertToNodeSpace(pos1);
        _guideHandUpDown->setPosition(pos2 + Vec2(0, -60));
}

void NoviceGuide::showCombatResult(Node* fires){
        fires->removeFromParent();
        _endTurnTipsLayer->setVisible(false);
        _diceResultLayer->setVisible(true);
        _curGuideState = GUIDE_GAME_STATUS_SHOW_MYRES;
        
        auto back_size = _diceResultLayer->getContentSize();
        Size dice_size;
        auto frameCache = SpriteFrameCache::getInstance();
        int from[] = {1, 4, 5, 6, 2, 2, 3};
        int to[] = {5, 6};
        for (int i = 0; i < 7; i++){
                int value = from[i];
                std::string dice_n = DICE_PIC_NAME_STR[0][value - 1];
                auto frame = frameCache->getSpriteFrameByName(dice_n);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                dice_size = dice->getContentSize();
                Vec2 pos(back_size.width / 2 - dice_size.width * (i + 1), dice_size.height);
                dice->setPosition(pos);
                _diceResultLayer->addChild(dice);
        }
        
        auto from_value = Label::createWithSystemFont("23", "fonts/arial.ttf", 40);
        from_value->setPosition(Vec2(back_size.width / 2 - 5 * dice_size.width - 10,
                                     dice_size.height));
        _diceResultLayer->addChild(from_value);
        
        for (int i = 0; i < 2; i++){
                int value = to[i];
                auto str = DICE_PIC_NAME_STR[1][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                Vec2 pos(back_size.width / 2 + dice_size.width * (i + 1),  dice_size.height);
                dice->setPosition(pos);
                _diceResultLayer->addChild(dice);
        }
        auto to_value = Label::createWithSystemFont("11", "fonts/arial.ttf", 40);
        to_value->setPosition(Vec2(back_size.width / 2 + 4 * dice_size.width + 10, dice_size.height));
        _diceResultLayer->addChild(to_value);
        
        _contentText->setString("This is your combat result:1, 4, 5, 6, 2, 2, 3, total:23. You have 7 soldiers in the area which means you can have 7 dice chances.");
        _guideHandLeftRight->setVisible(false);
        _guideHandUpDown->setVisible(true);
        auto pos = _diceResultLayer->convertToWorldSpace(Vec2(back_size.width / 2 - 2 * dice_size.width + 10, dice_size.height));
        auto pos2 = this->convertToNodeSpace(pos);
        auto pos3 = pos2 + Vec2(6 * dice_size.width, 0);
        _guideHandUpDown->setPosition(pos2);
        
        _guideLayer->setPosition(Vec2(pos2.x, pos2.y + _contentText->getContentSize().height));
        
        _nextButton->addClickEventListener([this, pos3](Ref*){
                
                _curGuideState = GUIDE_GAME_STATUS_SHOW_ENRES;
                _contentText->setString("This is your enemy's result:5, 6, total:11. There're only 2 soldiers in this area. So you can win this combat and occupy its area easily. It also makes your adjoining area number bigger.");
                _guideHandUpDown->setPosition(pos3);
                
                _nextButton->addClickEventListener(CC_CALLBACK_1(NoviceGuide::showNewTcValue, this));
                auto scale_by = ScaleBy::create(0.8f, 1.5f);
                auto seq = Sequence::create(scale_by, scale_by->reverse(), NULL);
                _tcShowMe->runAction(seq->clone());
                _tcShowEnemy->runAction(seq);
                _tcShowNumbMe->setString("X9");
                _tcShowNumbEnemy->setString("X5");
        });
        
        
        _nextButton->setVisible(true);
}
void NoviceGuide::showEngageMercenary(Ref*){
        _guideHandUpDown->setVisible(true);
        _guideHandLeftRight->setVisible(false);
        _endTurnTipsLayer->setVisible(true);
        _diceResultLayer->setVisible(false);
        _nextButton->setVisible(false);
        _curGuideState = GUIDE_GAME_STATUS_ENGAGE_MERCENARY;
        _contentText->setString("Click this button to engage mercenaries, you will get extra 10 sodilers to make you powerfull, but this can be used only once in every 4 turns.");
        auto pos1 = _mercenaryBtn->getParent()->convertToWorldSpace(_mercenaryBtn->getPosition());
        auto pos2 = this->convertToNodeSpace(pos1);
        _guideHandUpDown->setPosition(pos2);
        auto layer_size = _guideLayer->getContentSize();
        _guideLayer->setPosition(pos2 + Vec2(-0.4f * layer_size.width,
                                             0.7f * layer_size.height));
}


void NoviceGuide::choseFromArea(Ref*){
        _enemyShineBack->setVisible(true);
        _contentText->setString("Click this area to attack your enenmy. You can defeat him easily because you have much more soldiers.");
        
        _curGuideState = GUIDE_GAME_STATUS_SELECT_ENEMY;
        
        auto pos1 = _targetTo->getParent()->convertToWorldSpace(_targetTo->getPosition());
        auto pos2 = this->convertToNodeSpace(pos1);
        _guideHandUpDown->setPosition(pos2 + Vec2(0, -60));
}

void NoviceGuide::choseToArea(Ref*){
        if (GUIDE_GAME_STATUS_SELECT_ENEMY != _curGuideState){
                return;
        }
        _curGuideState = GUIDE_GAME_STATUS_SHOW_NEWTC;
        _enemyShineBack->setVisible(false);
        _meShineBack->setVisible(false);
        _beforeActionFromMe->setVisible(false);
        _afterActionMe->setVisible(true);
        _befroeActionTargetEnemy->setVisible(false);
        _afterActionEnmey->setVisible(true);
        _afterActionEnmeyBg->setVisible(true);
        
        auto cache = AnimationCache::getInstance();
        auto fire = cache->getAnimation("finght_occupay");
        fire->setRestoreOriginalFrame(true);         
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("XX0001.png");
        auto fire_spr = Sprite::create();
        fire_spr->setSpriteFrame(frame);
        auto visible_size = Director::getInstance()->getVisibleSize();
        fire_spr->setPosition(Vec2(visible_size.width *0.4f, visible_size.height * 0.75f));
        this->addChild(fire_spr, 5);
        
        auto call_back = CallFunc::create(std::bind(&NoviceGuide::showCombatResult, this, fire_spr));
        
        fire_spr->runAction(Sequence::create(Animate::create(fire), call_back, NULL));
}

void NoviceGuide::showNewTcValue(Ref*){
        _tcShowNumbMe->setString("9");
        _tcShowNumbEnemy->setString("5");
        _tcMapShineafterAction->setVisible(true);
        _guideHandLeftRight->setVisible(true);
        _guideHandUpDown->setVisible(false);
        _curGuideState = GUIDE_GAME_STATUS_ENGAGE_MERCENARY;
        _contentText->setString("Now the max number of your adjoining area is 9 and your total areas number is 16.");
        _nextButton->setVisible(true);
        _nextButton->addClickEventListener(CC_CALLBACK_1(NoviceGuide::showEngageMercenary, this));
        auto pos = _tcShowMe->getParent()->convertToWorldSpace(_tcShowMe->getPosition());
        auto pos2 = this->convertToNodeSpace(pos);
        _guideHandLeftRight->setPosition(pos2 + Vec2(-60, 0));
}
