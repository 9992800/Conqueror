//
//  ShoppingScene.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/12/25.
//
//

#include "ShoppingScene.hpp"
#include "ModalDialog.hpp"
#include "AchievementEngine.hpp"

enum{
        key_back_ground = 1
};
Scene* Shopping::createScene(){
        auto scene = Scene::create();
        auto layer = Shopping::create();
        scene->addChild(layer);
        return scene;
}

bool Shopping::init(){
        
        if (!Layer::init()){
                return false;
        }
        
#if COCOS2D_DEBUG >= 1
        IAP::setDebug(true);
#else
        IAP::setDebug(false);
#endif
        IAP::setListener(this);
        IAP::init();
        
//        IAP::refresh();
//        IAP::restore();
        
        _productPriceMap = std::map<std::string, ui::Text*>();
        _productsMap     = std::map<std::string, Product>();
        _soundEngine     = CocosDenshion::SimpleAudioEngine::getInstance();
        _soundTotalOn    = UserDefault::getInstance()->getBoolForKey(SOUND_MUSIC_TOTAL_KEY, true);
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        
        auto back_ground = Sprite::create("shopping/shopping_back.png");
        auto back_ground_size = back_ground->getContentSize();
        back_ground->setPosition(visible_size / 2);
        this->addChild(back_ground, 0, key_back_ground);
        
        auto return_btn = cocos2d::ui::Button::create("DIALOG_CANCEL.png","DIALOG_CANCEL_SEL.png");
        return_btn->setPosition(Vec2(back_ground_size.width - return_btn->getContentSize().width,
                                     return_btn->getContentSize().height));
        return_btn->addClickEventListener([this](Ref*){
                if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
                Director::getInstance()->popScene();
        });
        Value v = LOCALIZED_STRING_MAP.find("back")->second;
        return_btn->setTitleText(v.asString());
        return_btn->setTitleFontName(SYSTEM_FONTS);
        return_btn->setTitleFontSize(28);
        back_ground->addChild(return_btn);
        
        
        //6yuan coins
        auto coins_item_10 = cocos2d::ui::Button::create("shopping/shop_item_10.png");
        auto item_size = coins_item_10->getContentSize();
        coins_item_10->setPosition(Vec2(item_size.width * 0.6, back_ground_size.height * 0.5 + item_size.height * 0.55));
        coins_item_10->addClickEventListener(CC_CALLBACK_1(Shopping::buyItems, this, SHOP_ITEM_ID_10COINS_KEY));
        back_ground->addChild(coins_item_10);
        
        auto item_title_10 = ui::ImageView::create(RESOURCE_NAME_MAP.find("shop_times_10")->second);
        item_title_10->setPosition(Vec2(item_size.width * 0.45f, item_size.height * 0.4));
        coins_item_10->addChild(item_title_10);
        
        auto item_price_10 = ui::Text::create("$0.99", SYSTEM_FONTS, 46);
        item_price_10->setPosition(Vec2(item_size.width * 0.5f, item_size.height * 0.15));
        item_price_10->setIgnoreAnchorPointForPosition(false);
        item_price_10->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item_price_10->setTextColor(Color4B::BLACK);
        coins_item_10->addChild(item_price_10);
        _productPriceMap.insert(std::pair<std::string, ui::Text*>(SHOP_ITEM_ID_10COINS_KEY,
                                                               item_price_10));
        
        
        
        //90yuan coins
        auto coins_item_200 = cocos2d::ui::Button::create("shopping/shop_item_200.png");
        coins_item_200->setPosition(Vec2(coins_item_10->getPosition().x,
                                        back_ground_size.height * 0.5 - item_size.height * 0.55));
        
        coins_item_200->addClickEventListener(CC_CALLBACK_1(Shopping::buyItems, this, SHOP_ITEM_ID_200COINS_KEY));
        back_ground->addChild(coins_item_200);
        
        auto item_title_200 = ui::ImageView::create(RESOURCE_NAME_MAP.find("shop_times_200")->second);
        item_title_200->setPosition(Vec2(item_size.width * 0.45f, item_size.height * 0.4));
        coins_item_200->addChild(item_title_200);
        
        auto item_price_200 = ui::Text::create("$4.99", SYSTEM_FONTS, 46);
        item_price_200->setPosition(Vec2(item_size.width * 0.5f, item_size.height * 0.15));
        item_price_200->setIgnoreAnchorPointForPosition(false);
        item_price_200->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item_price_200->setTextColor(Color4B::BLACK);
        coins_item_200->addChild(item_price_200);
        _productPriceMap.insert(std::pair<std::string, ui::Text*>(SHOP_ITEM_ID_200COINS_KEY,
                                                               item_price_200));
        
        
        //30yuan coins
        auto coins_item_60 = cocos2d::ui::Button::create("shopping/shop_item_60.png");
        coins_item_60->setPosition(Vec2(item_size.width * 1.7, coins_item_10->getPosition().y));
        
        coins_item_60->addClickEventListener(CC_CALLBACK_1(Shopping::buyItems, this, SHOP_ITEM_ID_60COINS_KEY));
        back_ground->addChild(coins_item_60);
        
        auto item_title_60 = ui::ImageView::create(RESOURCE_NAME_MAP.find("shop_times_60")->second);
        item_title_60->setPosition(Vec2(item_size.width * 0.45f, item_size.height * 0.4));
        coins_item_60->addChild(item_title_60);
        
        auto item_price_60 = ui::Text::create("$13.99", SYSTEM_FONTS, 46);
        item_price_60->setPosition(Vec2(item_size.width * 0.5f, item_size.height * 0.15));
        item_price_60->setIgnoreAnchorPointForPosition(false);
        item_price_60->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item_price_60->setTextColor(Color4B::BLACK);
        coins_item_60->addChild(item_price_60);
        _productPriceMap.insert(std::pair<std::string, ui::Text*>(SHOP_ITEM_ID_60COINS_KEY,
                                                               item_price_60));
        
        
        //300yuan coins
        auto coins_item_680 = cocos2d::ui::Button::create("shopping/shop_item_680.png");
        coins_item_680->setPosition(Vec2(coins_item_60->getPosition().x,
                                         coins_item_200->getPosition().y));
        
        coins_item_680->addClickEventListener(CC_CALLBACK_1(Shopping::buyItems, this, SHOP_ITEM_ID_680COINS_KEY));
        back_ground->addChild(coins_item_680);
        
        auto item_title_680 = ui::ImageView::create(RESOURCE_NAME_MAP.find("shop_times_680")->second);
        item_title_680->setPosition(Vec2(item_size.width * 0.5f, item_size.height * 0.4));
        coins_item_680->addChild(item_title_680);
        
        auto item_price_680 = ui::Text::create("$46.99", SYSTEM_FONTS, 46);
        item_price_680->setPosition(Vec2(item_size.width * 0.45f, item_size.height * 0.15));
        item_price_680->setIgnoreAnchorPointForPosition(false);
        item_price_680->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item_price_680->setTextColor(Color4B::BLACK);
        coins_item_680->addChild(item_price_680);
        _productPriceMap.insert(std::pair<std::string, ui::Text*>(SHOP_ITEM_ID_680COINS_KEY,
                                                               item_price_680));
        
        
        
        //600yuan coins
        auto coins_item_1480 = cocos2d::ui::Button::create("shopping/shop_item_1480.png");
        coins_item_1480->setPosition(Vec2(item_size.width * 2.2 + coins_item_1480->getContentSize().width * 0.5f, back_ground_size.height * 0.5f));
        
        coins_item_1480->addClickEventListener(CC_CALLBACK_1(Shopping::buyItems, this, SHOP_ITEM_ID_1480COINS_KEY));
        back_ground->addChild(coins_item_1480);
        auto item_size_big = coins_item_1480->getContentSize();
        
        
        auto item_title_1480 = ui::ImageView::create(RESOURCE_NAME_MAP.find("shop_times_1480")->second);
        item_title_1480->setPosition(Vec2(item_size_big.width * 0.4f, item_size_big.height * 0.4));
        coins_item_1480->addChild(item_title_1480);
        
        auto item_price_1480 = ui::Text::create("$94.99", SYSTEM_FONTS, 56);
        item_price_1480->setPosition(Vec2(item_size_big.width * 0.5f, item_size_big.height * 0.15));
        item_price_1480->setIgnoreAnchorPointForPosition(false);
        item_price_1480->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item_price_1480->setTextColor(Color4B::BLACK);
        coins_item_1480->addChild(item_price_1480);
        _productPriceMap.insert(std::pair<std::string, ui::Text*>(SHOP_ITEM_ID_1480COINS_KEY,
                                                               item_price_1480));
        
        
        auto coins_back = Sprite::create("level/coind_back.png");
        auto coins_back_size = coins_back->getContentSize();
        Vec2 coins_back_pos = Vec2(visible_size  - coins_back_size);
        coins_back->setPosition(coins_back_pos);
        
        _coinsShow = Sprite::create("level/coins_show.png");
        auto coins_show_size = _coinsShow->getContentSize();
        Vec2 coins_pos = Vec2(coins_back_size.width,
                              coins_back_size.height * 0.5f);
        _coinsShow->setPosition(coins_pos);
        coins_back->addChild(_coinsShow);
        
        auto cache = UserDefault::getInstance();
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, 0);
        
        _coinsNumLb = Label::createWithSystemFont(StringUtils::format("%d", cur_coins), SYSTEM_FONTS, 32);
        _coinsNumLb->setPosition(coins_back_size * 0.5f);
        coins_back->addChild(_coinsNumLb);
        
        back_ground->addChild(coins_back);
        
        ModalLayer::showModalDialog(this);
        return true;
}

#pragma mark - menu function
void Shopping::buyItems(Ref* btn, std::string product_id){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        std::map<std::string, Product>::iterator it =  _productsMap.find(product_id);
        if (it == _productsMap.end()){
                return;
        }
        
        ModalLayer::showModalDialog(this);
        _curSelBtn = (ui::Button*)btn;
        Product p = it->second;
        IAP::purchase(p.name);
}

void Shopping::showBuySuccessAnim(int bought_no){
        auto visible_size = Director::getInstance()->getVisibleSize();
        
        auto showBtn = _curSelBtn->clone();
        showBtn->setScale(1.2f);
        auto back_ground = this->getChildByTag(key_back_ground);
        back_ground->addChild(showBtn);
        
        auto scale = ScaleTo::create(0.5f, 1.2f);
        auto move  = MoveTo::create(0.5f, visible_size / 2);
        auto to_center = Spawn::create(scale, move, NULL);
        
        auto call_back = CallFunc::create([this, showBtn, bought_no](){
                
                auto parent_size = showBtn->getContentSize();
                auto coin_pos = Vec2(parent_size.width * 0.4f, parent_size.height * 0.6f);
                int cur_coins = UserDefault::getInstance()->getIntegerForKey(USER_CURRENT_COINS);
                
                int coins_num_to_show = bought_no / 10;
                
                auto dest_pos = _coinsShow->getParent()->convertToWorldSpace(_coinsShow->getPosition());
                auto local_pos = showBtn->convertToNodeSpace(dest_pos);
                auto move_to = MoveTo::create(1.f, local_pos);
                auto coins_change = AnimationCache::getInstance()->getAnimation("coins_changes");
                auto coins_rotate = coins_change->clone();
                coins_rotate->setRestoreOriginalFrame(true);
                coins_rotate->setDelayPerUnit(1.f / 48.f);
                coins_rotate->setLoops(4);
                auto to_dest = Spawn::create(move_to, Animate::create(coins_rotate), NULL);
                
                for (int i = 0; i < coins_num_to_show; i++){
                        cur_coins += 10;
                        bool last_one = i == coins_num_to_show - 1;
                        
                        
                        auto call_back2 = CallFunc::create([this, showBtn, bought_no](){
                                
                                auto scaleBy = ScaleBy::create(0.2f, 1.4f);
                                auto seq = Sequence::create(scaleBy, scaleBy->reverse(), NULL);
                                _coinsShow->runAction(seq->clone());
                                _coinsNumLb->runAction(seq->clone());
                                
                                showBtn->removeFromParentAndCleanup(true);
                                auto cache = UserDefault::getInstance();
                                int rel_coins = cache->getIntegerForKey(USER_CURRENT_COINS);
                                rel_coins += bought_no;
                                cache->setIntegerForKey(USER_CURRENT_COINS, rel_coins);
                                _coinsNumLb->setString(StringUtils::format("%d", rel_coins));
                                cache->flush();
                                
                        });
                        
                        auto show_seq = Sequence::create(to_dest->clone(), call_back2, NULL);
                        
                        auto item_s = Sprite::create("level/coins_show.png");;
                        Size r_p(random(-100, 100), random(-100, 100));
                        item_s->setPosition(coin_pos +r_p);
                        showBtn->addChild(item_s);
                        if (last_one)
                                item_s->runAction(show_seq);
                        else{
                                item_s->runAction(to_dest->clone());
                        }
                }
        });
        
        auto seq = Sequence::create(to_center, call_back, nullptr);
        
        showBtn->runAction(seq);
        
        AchievementEngine::getInstance()->openReward(ACHIEVE_DATA_KEY_FIRST_BUY_COINS);
}

#pragma mark - payment callback
void Shopping::onSuccess(const Product& product){
        ModalLayer::dismissDialog(this);
        
        
        int bought_no = 1;
        if (product.id == SHOP_ITEM_ID_10COINS_KEY){
                bought_no = SHOP_ITME_VALUE_10COINS;
                
        }else if(product.id == SHOP_ITEM_ID_60COINS_KEY){
                bought_no = SHOP_ITME_VALUE_60COINS;
                
        }else if(product.id == SHOP_ITEM_ID_200COINS_KEY){
                bought_no = SHOP_ITME_VALUE_200COINS;
                
        }else if(product.id == SHOP_ITEM_ID_680COINS_KEY){
                bought_no = SHOP_ITME_VALUE_680COINS;
                
        }else if(product.id == SHOP_ITEM_ID_1480COINS_KEY){
                bought_no = SHOP_ITME_VALUE_1480COINS;
        }else{
                return;
        }
        this->showBuySuccessAnim(bought_no);
}

void Shopping::onFailure(const Product& p, const std::string& msg){
        CCLOG("Purchase Failed: %s", msg.c_str());
        ModalLayer::dismissDialog(this);
}
void Shopping::onCanceled(const Product& p){
        CCLOG("Purchase Canceled: %s", p.id.c_str());
        ModalLayer::dismissDialog(this);
}

void Shopping::onProductRequestSuccess(const std::vector<Product>& products){
        ModalLayer::dismissDialog(this);
        _productsMap.clear();
        for (int i=0; i < products.size(); i++){
                CCLOG("IAP: ========= IAP Item =========");
                Product p = products[i];
                std::string id =  p.id;
                CCLOG("IAP: Name: %s", p.name.c_str());
                CCLOG("IAP: ID: %s",  id.c_str());
                CCLOG("IAP: Title: %s", p.title.c_str());
                CCLOG("IAP: Desc: %s", p.description.c_str());
                CCLOG("IAP: Price: %s", p.price.c_str());
                CCLOG("IAP: Price Value: %f", p.priceValue);
                _productsMap.insert(std::pair<std::string, Product>(id, p));
                
                std::map<std::string, ui::Text*>::iterator it = _productPriceMap.find(id);
                if (it != _productPriceMap.end()){
                        auto price = it->second;
                        price->setString(p.price);
                }
                
        }
}

void Shopping::onProductRequestFailure(const std::string& msg){
        ModalLayer::dismissDialog(this);
        CCLOG("onProductRequestFailure Restored: %s", msg.c_str());
}
void Shopping::onRestoreComplete(bool ok, const std::string &msg){
        CCLOG("%s:%d:%s", __func__, ok, msg.data());
}

void Shopping::onInitialized(bool ok)
{
        CCLOG("%s : %d", __func__, ok);
}

void Shopping::onRestored(const Product& p)
{
        CCLOG("Purchase Restored: %s", p.name.c_str());
}

