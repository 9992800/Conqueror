//
//  IAPListener.hpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/5.
//
//

#ifndef IAPListener_hpp
#define IAPListener_hpp

#include "cocos2d.h"
#include "PluginIAP/PluginIAP.h"

class GamePayUtil: public cocos2d::Ref, public sdkbox::IAPListener{
public:
        static GamePayUtil* getInstance();
        GamePayUtil():_initStatus(false),_requestStatus(false){}
        ~GamePayUtil(){
                sdkbox::IAP::removeListener();
        }
        virtual bool init();
        
        void reInitAppStore();
public:
        void retrieveLastProduct();
        void onRestoreIAP();
        void purchaseItem(std::string name);
        inline bool isOk(){return _initStatus;}
private:
        virtual void onInitialized(bool ok) override;
        virtual void onSuccess(sdkbox::Product const& p) override;
        virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
        virtual void onCanceled(sdkbox::Product const& p) override;
        virtual void onRestored(sdkbox::Product const& p) override;
        virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
        virtual void onProductRequestFailure(const std::string &msg) override;
        void onRestoreComplete(bool ok, const std::string &msg) override;
        
        
private:
        bool _initStatus;
        bool _requestStatus;
};

#endif /* IAPListener_hpp */
