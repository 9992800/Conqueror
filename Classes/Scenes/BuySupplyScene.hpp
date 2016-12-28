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

class BuySupply : public cocos2d::LayerColor
{
public:
        virtual bool init() override;
        CREATE_FUNC(BuySupply);
private:
        void menuExit(Ref*);
};

#endif /* BuySupplyScene_hpp */
