//
//  IAPListener.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/5.
//
//

#include "GamePayUtil.hpp"

USING_NS_CC;
using namespace sdkbox;

static GamePayUtil* s_SharePayUtil = nullptr;
GamePayUtil* GamePayUtil::getInstance()
{
        if (!s_SharePayUtil)
        {
                s_SharePayUtil = new (std::nothrow) GamePayUtil();
                CCASSERT(s_SharePayUtil, "FATAL: Not enough memory");
                s_SharePayUtil->init();
        }
        
        return s_SharePayUtil;
}

#pragma mark - init the app store

bool GamePayUtil::init(){
        IAP::setDebug(true);
        IAP::setListener(this);
        IAP::init();
        
        return true;
}

void GamePayUtil::reInitAppStore(){
        if (false == _initStatus){
                IAP::setListener(this);
                IAP::init();
        }
}

void GamePayUtil::onInitialized(bool ok){
        CCLOG("%s : %d", __func__, ok);
        _initStatus = ok;
}

#pragma mark - retrieve last Product

void GamePayUtil::retrieveLastProduct(){
        IAP::refresh();
}

void GamePayUtil::onProductRequestSuccess(std::vector<Product> const &products){
        _requestStatus = true;
        //TODO:: store them to local database.
        for (int i=0; i < products.size(); i++){
                CCLOG("IAP: ========= IAP Item =========");
                CCLOG("IAP: Name: %s", products[i].name.c_str());
                CCLOG("IAP: ID: %s", products[i].id.c_str());
                CCLOG("IAP: Title: %s", products[i].title.c_str());
                CCLOG("IAP: Desc: %s", products[i].description.c_str());
                CCLOG("IAP: Price: %s", products[i].price.c_str());
                CCLOG("IAP: Price Value: %f", products[i].priceValue); 
        }
}
void GamePayUtil::onProductRequestFailure(const std::string &msg){
        _requestStatus = false;
        CCLOG("Fail to load products");
}

#pragma mark - pusrchase item

void GamePayUtil::purchaseItem(std::string produtName){
        IAP::purchase(produtName);
}
void GamePayUtil::onSuccess(sdkbox::Product const& p){
        CCLOG("Purchase Success: %s", p.id.c_str());
        if (p.name == "remove_ads") {
                CCLOG("Purchase complete: Remove Ads");
        }
}
void GamePayUtil::onFailure(sdkbox::Product const& p, const std::string &msg){
        CCLOG("Purchase Failed: %s", msg.c_str());
}
void GamePayUtil::onCanceled(sdkbox::Product const& p){
        CCLOG("Purchase Canceled: %s", p.id.c_str());
}

#pragma mark -restore item

void GamePayUtil::onRestoreIAP(){
        IAP::restore();
}
void GamePayUtil::onRestored(sdkbox::Product const& p){
        CCLOG("Purchase Restored: %s", p.name.c_str());
}

void GamePayUtil::onRestoreComplete(bool ok, const std::string &msg){
         CCLOG("%s:%d:%s", __func__, ok, msg.data());
}

