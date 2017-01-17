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

class NoviceGuide : public cocos2d::Layer{
        
public:
        static cocos2d::Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(NoviceGuide);
        ~NoviceGuide();
        void onEnter() override;
        void update(float delta)override;
        virtual void onExit()override;
        
        void initMap();
        void initController();
        void initGuideData();
        void initTopTittle();
        
private:
        void menuExit(Ref* pSender);
        void menuEngageArmy(Ref* pSender);
        void menuStartGame(Ref* pSender);
        void menuEndTurn(Ref* pSender);
        
private:
        Layer   *_controlLayer,
                *_diceResultLayer,
                *_endTurnTipsLayer,
                *_supplyShowLayer,
                *_choseMapLayer;
};
#endif /* NoviceGuideScene_hpp */
