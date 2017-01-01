//
//  BuySupplyScene.cpp
//  Conqueror
//
//  Created by wsli on 16/12/28.
//
//

#include "BuySupplyScene.hpp"
#include "ShoppingScene.hpp"
#include "APPConstants.hpp"

enum{
        k_back_ground = 1,
        k_item_mer_tips,
        k_item_mer_no,
        k_item_coins_no,
        k_item_get_it
};


Scene* BuySupply::createScene(){
        auto scene = Scene::create();
        auto layer = BuySupply::create();
        scene->addChild(layer);
        return scene;
}
bool BuySupply::init() {
        if (!Layer::init()){
                return false;
        }
        
        _mercenaryData = GolbalConfig::getInstance()->getMercenaryPriceData();
        this->_totalCount = (int)_mercenaryData.size();
        
        
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto scene_back = Sprite::create("shopping/shopping_back.png");
        scene_back->setPosition(visible_size / 2);
        auto scene_back_size = scene_back->getContentSize();
        this->addChild(scene_back, 0, k_back_ground);
        
        this->initCurCoins(scene_back);
        
        
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


void BuySupply::initCurCoins(Node* scene_back){
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        
        auto exit_btn = ui::Button::create("CloseNormal.png", "CloseSelected.png");
        exit_btn->setPosition(Vec2(visible_size.width - exit_btn->getContentSize().width * 0.6f,
                                   visible_size.height - exit_btn->getContentSize().height * 0.6f));
        
        exit_btn->addClickEventListener([=](Ref*){
                Director::getInstance()->popScene();
        });
        
        scene_back->addChild(exit_btn);
        auto title = Sprite::create("shopping/buy_supply_title.png");
        title->setPosition(Vec2(visible_size.width / 2, visible_size.height - title->getContentSize().height * 0.5f));
        scene_back->addChild(title);
        
        auto coins_back = Sprite::create("level/coind_back.png");
        auto coins_back_size = coins_back->getContentSize();
        Vec2 coins_back_pos = Vec2(coins_back->getContentSize().width
                                   ,exit_btn->getPosition().y);
        coins_back->setPosition(coins_back_pos);
        
        _coinsShow = Sprite::create("level/coins_show.png");
        auto coins_show_size = _coinsShow->getContentSize();
        Vec2 coins_pos = Vec2(coins_back_size.width,
                              coins_back_size.height * 0.5f);
        _coinsShow->setPosition(coins_pos);
        coins_back->addChild(_coinsShow);
        
        
        auto cache = UserDefault::getInstance();
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS);
        
        _coinsNumLb = Label::createWithSystemFont(StringUtils::format("%d", cur_coins), "fonts/arial.ttf", 32);
        _coinsNumLb->setPosition(coins_back_size * 0.5f);
        coins_back->addChild(_coinsNumLb);
        
        scene_back->addChild(coins_back);
        
        
        auto mercanery_back = Sprite::create("level/coind_back.png");
        auto mercanery_back_size = mercanery_back->getContentSize();
        Vec2 mercanery_back_pos = Vec2(exit_btn->getPosition().x - mercanery_back_size.width
                                   ,exit_btn->getPosition().y);
        mercanery_back->setPosition(mercanery_back_pos);
        
        _mercenaryShow = Sprite::create("level/dice_show.png");
        auto mercenary_show_size = _mercenaryShow->getContentSize();
        Vec2 mercenary_pos = Vec2(mercanery_back_size.width,
                              mercanery_back_size.height * 0.5f);
        _mercenaryShow->setPosition(mercenary_pos);
        mercanery_back->addChild(_mercenaryShow);
         
        int mercenary_no = cache->getIntegerForKey(USER_CURRENT_SUPPLY_NO);
        
        _mercenAriesNumLb = Label::createWithSystemFont(StringUtils::format("%d", mercenary_no), "fonts/arial.ttf", 32);
        _mercenAriesNumLb->setPosition(mercanery_back_size * 0.5f);
        mercanery_back->addChild(_mercenAriesNumLb);
        
        scene_back->addChild(mercanery_back);
        
        _soundTotalOn = cache->getBoolForKey(SOUND_MUSIC_TOTAL_KEY, true);
        _soundEngine = CocosDenshion::SimpleAudioEngine::getInstance();
}

#pragma mark -page list action.
ui::Layout* BuySupply::createListItem(){
        
        auto default_back =  Sprite::create("achievement/achieve_item_back.png");
        
        ui::Layout* default_item = ui::Layout::create();
        default_item->setBackGroundImage("achievement/achieve_item_back.png");
        auto default_item_size = default_back->getContentSize();
        default_item->setContentSize(default_item_size);
        
        auto mernary_tips = ui::ImageView::create("shopping/mercenary_tips_1.png");
        auto tips_size = mernary_tips->getContentSize();
        mernary_tips->setPosition(Vec2(tips_size.width * 0.6f, default_item_size.height - tips_size.height * 0.6f));
        default_item->addChild(mernary_tips, 1, k_item_mer_tips);
        
        auto mercenary_flag = ui::ImageView::create("level/dice_show.png");
        auto mercenary_flag_size = mercenary_flag->getContentSize();
        Vec2 mercenary_flag_pos(2.f * mercenary_flag_size.width,
                                default_item_size.height * 0.5f);
        mercenary_flag->setPosition(mercenary_flag_pos);
        default_item->addChild(mercenary_flag, 2);
        
        auto mercenary_no = ui::ImageView::create("shopping/NO_100.png");
        mercenary_no->setPosition(Vec2(mercenary_flag_pos.x + 2.2f * mercenary_flag_size.width,
                                       mercenary_flag_pos.y));
        default_item->addChild(mercenary_no, 2, k_item_mer_no);
        
        auto equal_flag = ui::ImageView::create("shopping/equals_flag.png");
        equal_flag->setPosition(Vec2(default_item_size.width * 0.35f, mercenary_flag_pos.y));
        default_item->addChild(equal_flag);
        
        auto coins_show = ui::ImageView::create("level/coins_show.png");
        coins_show->setPosition(Vec2(default_item_size.width * 0.45f, mercenary_flag_pos.y));
        default_item->addChild(coins_show);
        
        auto coins_no = ui::ImageView::create("shopping/NO_1000.png");
        coins_no->setPosition(Vec2(coins_show->getPosition().x + 2.2f * coins_show->getContentSize().width, mercenary_flag_pos.y));
        default_item->addChild(coins_no, 2, k_item_coins_no);
        
        auto butt_on = ui::Button::create("DIALOG2_OK.png","DIALOG2_OK_SEL.png");
        auto butt_on_size = butt_on->getContentSize();
        butt_on->setPosition(Vec2(default_item_size.width - butt_on_size.width * 0.8f,
                                  default_item_size.height * 0.5f));
        butt_on->setTouchEnabled(true);
        butt_on->setTitleText("Get it");
        butt_on->setTitleFontName("fonts/arial.ttf");
        butt_on->setTitleFontSize(38);
        butt_on->addClickEventListener(CC_CALLBACK_1(BuySupply::actionGetItem, this));
        butt_on->setName("ssss_ssss");
        default_item->addChild(butt_on, 2, k_item_get_it);
        
        return default_item;
}

void BuySupply::initItemDetails(ui::Widget* mercenary_item, int idx){
        mercenary_item->setTag(idx);
        MercenaryItem data = _mercenaryData.at(idx);
        
        auto item_desc_tips = (ui::ImageView*)mercenary_item->getChildByTag(k_item_mer_tips);;
        item_desc_tips->loadTexture(data.itemTips);
        
        auto item_value = (ui::ImageView*)mercenary_item->getChildByTag(k_item_mer_no);
        item_value->loadTexture(data.valueNoImg);
        
        auto item_price = (ui::ImageView*)mercenary_item->getChildByTag(k_item_coins_no);
        item_price->loadTexture(data.priceNoImg);
        
        
        mercenary_item->getChildByTag(k_item_get_it)->setTag(idx);
}

void BuySupply::update(float dt){
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


float BuySupply::getItemPositionYInView(cocos2d::ui::Widget* item)const{
        auto worldPos = item->getParent()->convertToWorldSpaceAR(item->getPosition());
        auto viewPos = this->_listView->convertToNodeSpaceAR(worldPos);
        return viewPos.y;
}

void BuySupply::updateItem(int itemID, int templateID)
{
        auto mercenary_item = _listView->getItems().at(templateID);
        mercenary_item->setTag(itemID);
        
        MercenaryItem data = _mercenaryData.at(itemID);
        
        auto item_desc_tips = (ui::ImageView*)mercenary_item->getChildByTag(k_item_mer_tips);
        item_desc_tips->loadTexture(data.itemTips);
        
        auto item_value = (ui::ImageView*)mercenary_item->getChildByTag(k_item_mer_no);
        item_value->loadTexture(data.valueNoImg);
        
        auto item_price = (ui::ImageView*)mercenary_item->getChildByTag(k_item_coins_no);
        item_price->loadTexture(data.priceNoImg);
        
        auto button = (ui::Button*)mercenary_item->getChildByName("ssss_ssss");//mercenary_item->getChildByTag(k_item_get_it);
        button->setTag(itemID);
}


#pragma mark - menu action.

void BuySupply::actionGetItem(Ref*btn){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        
        auto cache = UserDefault::getInstance();
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, 0);
        
        int chose_item_id = ((Node*)btn)->getTag();
        MercenaryItem data = _mercenaryData.at(chose_item_id);
        
        if (cur_coins < data.itemPrice){
                auto scene = Shopping::createScene();
                Director::getInstance()->pushScene(scene);
        }else{
                this->playCoinsSubAnim(data);
        }
}

void BuySupply::playCoinsSubAnim(MercenaryItem data){
        
        auto call_back = CallFunc::create([this, data](){
                auto cache = UserDefault::getInstance();
                int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, 0);
                int cur_mercenaries = cache->getIntegerForKey(USER_CURRENT_SUPPLY_NO, 0);
                cur_coins -= data.itemPrice;
                cur_mercenaries += data.itemValue;
                cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
                cache->setIntegerForKey(USER_CURRENT_SUPPLY_NO, cur_mercenaries);
                cache->flush();
                
                _coinsNumLb->setString(StringUtils::format("%d", cur_coins));
                _mercenAriesNumLb->setString(StringUtils::format("%d", cur_mercenaries));
        });
        
        auto scale_by = ScaleBy::create(0.2f, 1.4f);
        auto seq = Sequence::create(scale_by, scale_by->reverse(),  NULL);
        auto seq_2 = Sequence::create(scale_by->clone(), call_back,scale_by->clone()->reverse(), NULL);
        
        _coinsShow->runAction(seq);
        _mercenaryShow->runAction(seq->clone());
        _mercenAriesNumLb->runAction(seq->clone());
        _coinsNumLb->runAction(seq_2);
}
