//
//  BuySupplyScene.hpp
//  Conqueror
//
//  Created by wsli on 16/12/28.
//
//

#ifndef BuySupplyScene_hpp
#define BuySupplyScene_hpp

#include "AppMacros.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
typedef std::function<void(void)> fbBuySupplyCallback;

class BuySupply : public cocos2d::LayerColor
{
public:
        virtual bool init() override;
        CREATE_FUNC(BuySupply);
        BuySupply():_soundTotalOn(true)
        ,_curLisntener(nullptr){
        }
        
        inline void addParchseListner(const fbBuySupplyCallback& cb){
                this->_curLisntener = cb;
        }

        
private:
        void menuSpendCoins(Ref*, int);private:
        bool                                    _soundTotalOn;
        CocosDenshion::SimpleAudioEngine*       _soundEngine;
        fbBuySupplyCallback                     _curLisntener;
};

#endif /* BuySupplyScene_hpp */
