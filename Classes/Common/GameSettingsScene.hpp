//
//  GameSettings.hpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#ifndef GameSettings_hpp
#define GameSettings_hpp

#include "cocos2d.h"
USING_NS_CC;

class GameSettings : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(GameSettings);
        
protected:
        void menuOk(Ref* pSender);
        
private:
        MenuItemImage* _okButton;
};


#endif /* GameSettings_hpp */
