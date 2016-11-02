//
//  GameHelpScene.hpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/2.
//
//

#ifndef GameHelpScene_hpp
#define GameHelpScene_hpp

#include "cocos2d.h"
USING_NS_CC;


class GameHelp : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(GameHelp);
        
protected:
        void menuOk(Ref* pSender);
        
private:
        MenuItemImage* _okButton;
};


#endif /* GameHelpScene_hpp */
