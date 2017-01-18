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
private:
        void menuExit(Ref* pSender);
        void menuEngageArmy(Ref* pSender);
        void menuStartGame(Ref* pSender);
        void menuEndTurn(Ref* pSender);
        void showSelectGuide(Ref*);
        void showCombatResult(Node*);
        void showEngageMercenary(Ref*);
        void showNewTcValue(Ref*);
        void choseFromArea(Ref*);
        void choseToArea(Ref*);
        
private:
        int     _curGuideState;
        Layer   *_controlLayer,
                *_diceResultLayer,
                *_endTurnTipsLayer,
                *_supplyShowLayer,
                *_choseMapLayer;
        
        cocos2d::Sprite *_tcShowMe,
                        *_tcShowEnemy,
                        *_guideHandUpDown,
                        *_guideHandLeftRight,
                        *_tcMapShineBeforeAction,
                        *_tcMapShineafterAction,
                        *_beforeActionFromMe,
                        *_befroeActionTargetEnemy,
                        *_afterActionMe,
                        *_afterActionEnmey,
                        *_afterActionEnmeyBg,
                        *_meShineBack,
                        *_enemyShineBack;
        
        cocos2d::ui::Scale9Sprite  *_guideLayer;
        cocos2d::Label  *_contentText, *_tcShowNumbMe, *_tcShowNumbEnemy;
        cocos2d::RepeatForever  *_handsUpDownAnim,
                                *_handsLeftRightAnim;
        
        ui::Button      *_nextButton,
                        *_mercenaryBtn,
                        *_endTurnBtn,
                        *_selectFrom,
                        *_targetTo;
        ui::TextBMFont* _counterTurns;
};
#endif /* NoviceGuideScene_hpp */
