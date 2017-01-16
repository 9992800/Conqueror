//
//  NoviceGuideScene.hpp
//  Conqueror
//
//  Created by wsli on 17/1/16.
//
//

#ifndef NoviceGuideScene_hpp
#define NoviceGuideScene_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class NoviceGuide : public cocos2d::Layer{
        
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(NoviceGuide);
        ~NoviceGuide();
        void onEnter() override;
        void update(float delta)override;
        virtual void onExit()override;
        
        void initMap();
        void initController();
        void initGuideData();
};
#endif /* NoviceGuideScene_hpp */
