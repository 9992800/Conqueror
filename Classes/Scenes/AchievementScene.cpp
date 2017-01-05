//
//  AchievementScene.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/12/27.
//
//
#include "AchievementScene.hpp"
#include "AchievementEngine.hpp"
enum{
        k_item_title_backgrd = 1,
        k_item_title_text,
        k_item_desc_text,
        k_item_get_btn,
        k_item_status,
        k_item_status_back
};

Scene* Achievement::createScene(){
        auto scene = Scene::create();
        auto layer = Achievement::create();
        scene->addChild(layer);
        return scene;
}

bool Achievement::init() {
        if (!Layer::init()){
                return false;
        }
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto scene_back = Sprite::create("shopping/shopping_back.png");
        scene_back->setPosition(visible_size / 2);
        auto scene_back_size = scene_back->getContentSize();
        this->addChild(scene_back);
        
        auto exit_btn = ui::Button::create("CloseNormal.png", "CloseSelected.png");
        exit_btn->setPosition(Vec2(visible_size.width - exit_btn->getContentSize().width * 0.6f,
                                   visible_size.height - exit_btn->getContentSize().height * 0.6f));
        
        exit_btn->addClickEventListener([=](Ref*){
                Director::getInstance()->popScene();
        });
        scene_back->addChild(exit_btn);
        
        auto title = Sprite::create("achievement/achieve_title.png");
        title->setPosition(Vec2(visible_size.width / 2, visible_size.height - title->getContentSize().height * 0.5f));
        scene_back->addChild(title);
        
        
        auto default_item = this->createListItem();
        _listView = ui::ListView::create();
        // set list view ex direction
        _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        _listView->setBounceEnabled(true);  
        _listView->setContentSize(Size(default_item->getContentSize().width,
                                       visible_size.height * 0.8f));
        _listView->setPosition(Vec2(visible_size.width * 0.02f, visible_size.height * 0.05f));
        _listView->setScrollBarPositionFromCorner(Vec2(7, 7)); 
        scene_back->addChild(_listView);
        
        _listView->setItemModel(default_item);
        _listView->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
        this->_achievementData = GolbalConfig::getInstance()->getAchievementData();
        this->_totalCount = (int)_achievementData.size();
        this->_spawnCount = this->_totalCount;
        
        this->_bufferZone = default_item->getContentSize().height * 1.1f;
        
        for (int i = 0; i < this->_totalCount; ++i) {
                if (i < this->_spawnCount) {
                        ui::Widget* item = default_item->clone();
                        this->initItemDetails(item, i);
                        _listView->pushBackCustomItem(item);
                }
        }
        
        
        float spacing = 8;
        _listView->setItemsMargin(spacing);        
        _itemTemplateHeight = default_item->getContentSize().height;
        this->_reuseItemOffset = (_itemTemplateHeight + spacing) * _spawnCount;
        
        this->scheduleUpdate();
        _listView->forceDoLayout();
        
        _listView->forceDoLayout();
        float totalHeight = _itemTemplateHeight * _totalCount + (_totalCount - 1) * spacing;
        _listView->setInnerContainerSize(Size(_listView->getInnerContainerSize().width, totalHeight));
        _listView->jumpToTop();
        
        
        return true;
}


#pragma mark - list view functions.
ui::Layout* Achievement::createListItem(){
        
        auto default_back =  Sprite::create("achievement/achieve_item_back.png");
        
        ui::Layout* default_item = ui::Layout::create();
        default_item->setBackGroundImage("achievement/achieve_item_back.png");
        auto default_item_size = default_back->getContentSize();
        default_item_size.width *= 1.05f;
        default_item->setContentSize(default_item_size);
        
        auto achieve_title = ui::ImageView::create("achievement/achieve_title_back.png");
        auto title_size = achieve_title->getContentSize();
        achieve_title->setPosition(Vec2(title_size.width * 0.51f,
                                        default_item_size.height - title_size.height * 0.5f));
        default_item->addChild(achieve_title, 1, k_item_title_backgrd);
        
        auto item_tile_txt = ui::Text::create("Achievement Item", "fonts/arial.ttf", 30);
        item_tile_txt->setPosition(Vec2(title_size.width *0.3f, title_size.height * 0.7f));
        achieve_title->addChild(item_tile_txt, 2, k_item_title_text);
        
        auto achieve_status = ui::ImageView::create("achievement/achieve_status_new.png");
        auto achieve_status_size = achieve_status->getContentSize();
        achieve_status->setPosition(Vec2(title_size.width *0.7f, title_size.height * 0.7f));
        achieve_title->addChild(achieve_status, 2, k_item_status);
        achieve_status->setVisible(false);
        
        auto new_shine = ui::ImageView::create("game_win_shine.png");
        new_shine->setPosition(Vec2(title_size.width *0.7f, title_size.height * 0.7f));
        new_shine->setScale(0.6f);
        achieve_title->addChild(new_shine, 1, k_item_status_back);
        new_shine->runAction(RepeatForever::create(RotateBy::create(2.f, 360)));
        new_shine->setVisible(false);
        
        auto achieve_cup = ui::ImageView::create("achievement/achive_flag.png");
        auto achieve_cup_size = achieve_cup->getContentSize();
        achieve_cup->setPosition(achieve_cup_size * 0.6f);
        default_item->addChild(achieve_cup, 2);
        
        auto item_desc_txt = ui::Text::create("This is the description of the achievement.", "fonts/arial.ttf", 32);
        item_desc_txt->setPosition(Vec2(achieve_cup_size.width * 1.2f,
                                        default_item_size.height * 0.7f));
        item_desc_txt->setColor(Color3B::ORANGE);
        item_desc_txt->setAnchorPoint(Vec2(0.f, 1.0f));
        item_desc_txt->ignoreContentAdaptWithSize(false);
        item_desc_txt->setContentSize(default_item_size * 0.4);
        item_desc_txt->setTextHorizontalAlignment(TextHAlignment::LEFT);
        default_item->addChild(item_desc_txt, 3, k_item_desc_text);
        
        
        auto butt_on = ui::Button::create("DIALOG2_OK.png","DIALOG2_OK_SEL.png");
        auto butt_on_size = butt_on->getContentSize();
        butt_on->setPosition(Vec2(default_item_size.width - butt_on_size.width,
                                  default_item_size.height * 0.5f));
        butt_on->setTouchEnabled(true);
        butt_on->setTitleText("GET IT");
        butt_on->setTitleFontName("fonts/arial.ttf");
        butt_on->setTitleFontSize(32);
        butt_on->setName("ssss_ssss");
        butt_on->addClickEventListener(CC_CALLBACK_1(Achievement::actionButton, this));
        default_item->addChild(butt_on, 4, k_item_get_btn);
        
        return default_item;
}
void Achievement::initItemDetails(ui::Widget* achieve_item, int idx){
        achieve_item->setTag(idx);
        
        auto item_desc = (ui::Text*)achieve_item->getChildByTag(k_item_desc_text);;
        AchievementData data = _achievementData.at(idx);
        item_desc->setString(data.desc);
        
        auto tittle_back = achieve_item->getChildByTag(k_item_title_backgrd);
        auto item_title = (ui::Text*)tittle_back->getChildByTag(k_item_title_text);
        item_title->setString(data.title);
        
        auto button = (ui::Button*)achieve_item->getChildByName("ssss_ssss");
        button->setTag(idx);
        
        auto new_shine  = (ui::ImageView*)tittle_back->getChildByTag(k_item_status_back);
        auto new_tag    = (ui::ImageView*)tittle_back->getChildByTag(k_item_status);
        
        if (REWARDS_STATUS_CLOSED == data.bonus_status){
                button->setTitleText("GET THIS");
        }else if (REWARDS_STATUS_OPEN == data.bonus_status){
                button->setTitleText("COLLECT");
                new_tag->setVisible(true);
                new_shine->setVisible(true);
        }else{
                button->setTitleText("FINISHED");
        }
}

void Achievement::update(float dt){
        this->_updateTimer += dt;
        if (this->_updateTimer < this->_updateInterval) {
                return;
        }
        
        float totalHeight = _itemTemplateHeight * _totalCount + (_totalCount - 1) * 4;
        
        auto listViewHeight = _listView->getContentSize().height;
        
        this->_updateTimer = 0;
        auto isDown = this->_listView->getInnerContainerPosition().y < this->_lastContentPosY;
        auto items = _listView->getItems();
        
        for (int i = 0; i < _spawnCount && i < _totalCount; ++i) {
                auto item = items.at(i);
                auto itemPos = this->getItemPositionYInView(item);
                if (isDown) {
                        if (itemPos < -_bufferZone && item->getPosition().y + _reuseItemOffset < totalHeight) {
                                int itemID = item->getTag() - (int)items.size();
                                item->setPositionY(item->getPositionY() + _reuseItemOffset);
                                CCLOG("itemPos = %f, itemID = %d, tempateID = %d", itemPos, itemID, i);
                                this->updateItem(itemID, i);
                        }
                }
                else {
                        if (itemPos > _bufferZone + listViewHeight &&
                            item->getPosition().y - _reuseItemOffset >= 0) {
                                
                                item->setPositionY(item->getPositionY() - _reuseItemOffset);
                                int itemID = item->getTag() + (int)items.size();
                                CCLOG("itemPos = %f, itemID = %d, templateID = %d", itemPos, itemID, i);
                                this->updateItem(itemID, i);
                        }
                }
        }
        //update ListView Items
        this->_lastContentPosY = this->_listView->getInnerContainer()->getPosition().y;
}


float Achievement::getItemPositionYInView(cocos2d::ui::Widget* item)const{
        auto worldPos = item->getParent()->convertToWorldSpaceAR(item->getPosition());
        auto viewPos = this->_listView->convertToNodeSpaceAR(worldPos);
        return viewPos.y;
}

void Achievement::updateItem(int itemID, int templateID)
{
        auto itemTemplate = _listView->getItems().at(templateID);
        auto item_desc = (ui::Text*)itemTemplate->getChildByTag(k_item_desc_text);
        itemTemplate->setTag(itemID);
        AchievementData data = _achievementData.at(itemID);
        item_desc->setString(data.desc);
        
        auto tittle_back = itemTemplate->getChildByTag(k_item_title_backgrd);
        auto item_title = (ui::Text*)tittle_back->getChildByTag(k_item_title_text);
        item_title->setString(data.title);
        
        auto new_shine  = (ui::ImageView*)tittle_back->getChildByTag(k_item_status_back);
        auto new_tag    = (ui::ImageView*)tittle_back->getChildByTag(k_item_status);
        
        auto button = (ui::Button*)itemTemplate->getChildByName("ssss_ssss");
        button->setTag(itemID);
        
        if (REWARDS_STATUS_CLOSED == data.bonus_status){
                button->setTitleText("GET THIS");
        }else if (REWARDS_STATUS_OPEN == data.bonus_status){
                button->setTitleText("COLLECT");
                new_tag->setVisible(true);
                new_shine->setVisible(true);
        }else{
                button->setTitleText("FINISHED");
        }
}


#pragma mark - menu action
void Achievement::jumpToGuidScene(AchievementData data){
        
}

void Achievement::collectAchievement(AchievementData data){
        auto v_size = Director::getInstance()->getVisibleSize();
        
        Vector<FiniteTimeAction*> actions;
        
        if (data.bonus_coinsNum > 0){
                
                auto call_back = CallFunc::create([this, data](){
                        auto cache = UserDefault::getInstance();
                        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS);
                        cur_coins += data.bonus_coinsNum;
                        cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
                        cache->flush();
                });
                auto call_back_todo = CallFunc::create([this, v_size, call_back](){
                AchievementEngine::getInstance()->coinsAnimShow(this,
                                                                v_size * 0.5f,
                                                                v_size, call_back);
                });
                
                actions.pushBack(call_back_todo);
        }
        if (data.bonus_mercenaryNum > 0){
                auto call_back_todo = CallFunc::create([this, v_size, data](){
                         Vec2 position(v_size * 0.5f);
                        auto call_back = CallFunc::create([this, data](){
                                auto cache = UserDefault::getInstance();
                                int sup_no = cache->getIntegerForKey(USER_CURRENT_SUPPLY_NO);
                                sup_no += data.bonus_mercenaryNum;
                                cache->setIntegerForKey(USER_CURRENT_SUPPLY_NO, sup_no);
                                cache->flush();
                        });
                        
                        auto clean_call_self = CallFunc::create([this, data](){
                                for (int i = 0; i < data.bonus_mercenaryNum; i++){
                                        this->removeChildByTag(MODAL_DIALOG_NODETAG + i);
                                }
                        });

                        auto to_dest = MoveTo::create(1.0f, v_size);
                        auto seq_last = Sequence::create(to_dest->clone(), clean_call_self, call_back, NULL);
                        auto seq = Sequence::create(to_dest->clone(), clean_call_self, call_back, NULL);
                        
                        for (int i = 0; i < data.bonus_mercenaryNum; i++){
                                Vec2 offset(random(-0.05f * v_size.width, 0.05f * v_size.width),
                                            random(-0.05f * v_size.height, 0.05f * v_size.height));
                                auto mercenary =  Sprite::create("level/dice_show.png");
                                mercenary->setPosition(position + offset);
                                this->addChild(mercenary, SUPER_LAYER_PRIVILIEGE, MODAL_DIALOG_NODETAG  + i);
                                if (i == data.bonus_mercenaryNum - 1){
                                        mercenary->runAction(seq->clone());
                                }else{
                                        mercenary->runAction(seq_last->clone());
                                }
                        }
                });
                actions.pushBack(call_back_todo);
        }
//        if (data.bonus_charactor_key.length() > 0){
//                auto call_back_todo = CallFunc::create([this, v_size, data](){
//                
//                        std::string img_path = AchievementEngine::getInstance()->getCharactorImg(data.bonus_charactor_key);
//                        auto character = Sprite::create(img_path);
//                        auto ch_size = character->getContentSize();
//                        character->setPosition(ch_size * 0.5f);
//                        
//                        auto cha_layer = Layer::create();
//                        cha_layer->setContentSize(ch_size);
//                        cha_layer->addChild(character, 2);
//                        
//                        auto shining_back = Sprite::create("game_win_shine.png");
//                        shining_back->setPosition(ch_size * 0.5f);
//                        shining_back->runAction(RepeatForever::create(RotateBy::create(2.0f, 360)));
//                        cha_layer->addChild(shining_back, 1);
//                        
//                        auto cale_by = ScaleBy::create(1.f, 3.f);
//                        auto call_back = CallFunc::create([this, cha_layer, data](){
//                                cha_layer->removeFromParentAndCleanup(true);
//                                
//                                auto cache = UserDefault::getInstance();
//                                cache->setBoolForKey(data.bonus_charactor_key.c_str(), true);
//                                cache->flush();
//                        });
//                        cha_layer->runAction(Sequence::create(cale_by, DelayTime::create(0.3f),
//                                                              call_back, NULL));
//                        
//                        this->addChild(cha_layer, SUPER_LAYER_PRIVILIEGE);
//                        
//                });
////                actions.pushBack(call_back_todo);
//        }
//        if (data.bonus_map_key.length() > 0){
//                auto call_back_todo = CallFunc::create([this, v_size, data](){
//
//                        std::string tips = AchievementEngine::getInstance()->getMapName(data.bonus_map_key);
//                        
//                        auto label = Label::createWithSystemFont(tips, "fonts/arial.ttf", 32);
//                        
//                        auto cale_by = ScaleBy::create(1.f, 3.f);
//                        auto call_back = CallFunc::create([this, label, data](){
//                                label->removeFromParentAndCleanup(true);
//                                
//                                auto cache = UserDefault::getInstance();
//                                cache->setBoolForKey(data.bonus_charactor_key.c_str(), true);
//                                cache->flush();
//                        });
//                        label->runAction(Sequence::create(cale_by, DelayTime::create(0.3f),
//                                                          call_back, NULL));
//                        
//                        this->addChild(label, SUPER_LAYER_PRIVILIEGE);
//                });
////                actions.pushBack(call_back_todo);
//        }
        
        auto call_back_todo = CallFunc::create([this, v_size, data](){
                AchievementEngine::getInstance()->finishReward(data.cache_key);
         });
//        actions.pushBack(call_back_todo);
        
        this->runAction(Sequence::create(actions));
}

void Achievement::actionButton(Ref* btn){
        
        ui::Button* action_orig = (ui::Button*)btn;
        int itemID = action_orig->getTag();
        
        AchievementData data = _achievementData.at(itemID);
        if (REWARDS_STATUS_CLOSED == data.bonus_status){
                this->jumpToGuidScene(data);
        }else if (REWARDS_STATUS_OPEN == data.bonus_status){
                this->collectAchievement(data);
        }else{
                return;
        }
}
