//
//  ShoppingScene.hpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/12/25.
//
//

#ifndef ShoppingScene_hpp
#define ShoppingScene_hpp

#include "AppMacros.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PluginIAP/PluginIAP.h"

USING_NS_CC;
using namespace sdkbox;

class Shopping : public cocos2d::Layer, public sdkbox::IAPListener
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(Shopping);
        ~Shopping(){
                IAP::removeListener();
        }
        
private:
        void onInitialized(bool success)override;
        void onSuccess(const Product& p)override;
        void onFailure(const Product& p, const std::string& msg)override;
        void onCanceled(const Product& p)override;
        void onRestored(const Product& p)override;
        void onProductRequestSuccess(const std::vector<Product>& products)override;
        void onProductRequestFailure(const std::string& msg)override;
        void onRestoreComplete(bool ok, const std::string &msg)override;
        
private:
        void buyItems(Ref*, std::string);
private:
        
        std::map<std::string, Product>          _productsMap;
        std::map<std::string, Label*>           _productPriceMap;
};

#endif /* ShoppingScene_hpp */
