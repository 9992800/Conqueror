//
//  ReplayLastScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/14.
//
//

#ifndef ReplayLastScene_hpp
#define ReplayLastScene_hpp
#include "AppMacros.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class ReplayLast : public cocos2d::Layer{
public:
        static Scene* createScene(GameData*);
        virtual bool init() override;
        CREATE_FUNC2(ReplayLast, GameData*, data);
        ReplayLast(GameData* data):_gameData(data),
        _timeCounter(0),_dataIdx(0){
        }
        
        virtual ~ReplayLast();
        
        virtual void onEnter()override;
        virtual void update(float delta)override;
        virtual void onExit()override;
        
private:
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        
        void menuExit(Ref* pSender);
        void menuStartShow(Ref* pSender);
        void playHistory(float);
private:
        float                   _lowestPostion_y;
        GameData*               _gameData;
        std::vector<int>        _hisFrom;
        std::vector<int>        _hisTo;
        std::vector<int>        _hisRes;
        int                     _timeCounter;
        int                     _dataIdx;
};

#endif /* ReplayLastScene_hpp */
