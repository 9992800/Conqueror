//
//  ShoppingScene.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/12/25.
//
//

#include "ShoppingScene.hpp"

Scene* Shopping::createScene(){
        auto scene = Scene::create();
        auto layer = Shopping::create();
        scene->addChild(layer);
        return scene;
}

bool Shopping::init(){
        IAP::setDebug(true);
        IAP::setListener(this);
        IAP::init();
        IAP::refresh();
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        
        auto back_ground = Sprite::create("shopping/shopping_back.png");
        auto back_ground_size = back_ground->getContentSize();
        back_ground->setPosition(visible_size / 2);
        this->addChild(back_ground);
        
        auto return_btn = cocos2d::ui::Button::create("DIALOG_CANCEL.png","DIALOG_CANCEL_SEL.png");
        return_btn->::Shopping::setPosition(back_ground_size.width * 0.92, back_ground_size.height * 0.08);
        return_btn->addClickEventListener([](Ref*){
                Director::getInstance()->popScene();
        });
        return_btn->setTitleText("返回");
        return_btn->setTitleFontName("fonts/arial.ttf");
        return_btn->setTitleFontSize(28);
        return_btn->setScale(1.4f);
        back_ground->addChild(return_btn);
        
        auto coins_item_10 = cocos2d::ui::Button::create("shopping/shop_item_10.png");
        auto item_size = coins_item_10->getContentSize();
        coins_item_10->setPosition(Vec2(item_size.width * 0.6, back_ground_size.height * 0.5 + item_size.height * 0.6));
        coins_item_10->addClickEventListener(CC_CALLBACK_1(Shopping::buyItems, this, SHOP_ITEM_10_COINS_KEY));
        back_ground->addChild(coins_item_10);
        
        auto item_desc = Label::createWithSystemFont("送首充礼包!", "fonts/arial.ttf", 24);
        item_desc->setPosition(item_size.width *.03, item_size.height * 0.5);
        coins_item_10->addChild(item_desc);
        
        auto item_price = Label::createWithSystemFont("6元", "fonts/arial.ttf", 24);
        item_price->setPosition(item_size.width *.05, item_size.height * 0.1);
        coins_item_10->addChild(item_price);
        return true;
}

#pragma mark - shop function
void Shopping::buyItems(Ref*, std::string product_name){
        IAP::purchase(product_name);
}

#pragma mark - payment callback
void Shopping::onSuccess(const Product& p){
        
}
void Shopping::onFailure(const Product& p, const std::string& msg){
        CCLOG("Purchase Failed: %s", msg.c_str());
}
void Shopping::onCanceled(const Product& p){
        CCLOG("Purchase Canceled: %s", p.id.c_str());
}

void Shopping::onProductRequestSuccess(const std::vector<Product>& products){
        _productsMap.clear();
        for (int i=0; i < products.size(); i++){
                CCLOG("IAP: ========= IAP Item =========");
                CCLOG("IAP: Name: %s", products[i].name.c_str());
                CCLOG("IAP: ID: %s", products[i].id.c_str());
                CCLOG("IAP: Title: %s", products[i].title.c_str());
                CCLOG("IAP: Desc: %s", products[i].description.c_str());
                CCLOG("IAP: Price: %s", products[i].price.c_str());
                CCLOG("IAP: Price Value: %f", products[i].priceValue);
                _productsMap.insert(std::pair<std::string, Product>(products[i].name, products[i]));
        }
}

void Shopping::onProductRequestFailure(const std::string& msg){
        CCLOG("onProductRequestFailure Restored: %s", msg.c_str());
}
void Shopping::onRestoreComplete(bool ok, const std::string &msg){
        CCLOG("%s:%d:%s", __func__, ok, msg.data());
}

