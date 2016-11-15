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

typedef struct history_replay_tag{
        GameData*         gameData;
        std::vector<int>  from;
        std::vector<int>  to;
        std::vector<int>  res;
        std::vector<Vec2> sup;
}HistoryReplayData;

class ReplayLast : public cocos2d::Layer{
public:
        static Scene* createScene(HistoryReplayData data);
        virtual bool init() override;
        CREATE_FUNC2(ReplayLast, HistoryReplayData, data);
        
        ReplayLast(HistoryReplayData data);
        virtual ~ReplayLast();
        
        virtual void onEnter()override;
        virtual void update(float delta)override;
        virtual void onExit()override;
        
private:
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        
        void menuExit(Ref* pSender);
        void menuStartShow(Ref* pSender);
        void playHistory(float);
        void resetHistoryData();
private:
        float                   _lowestPostion_y;
        GameData*               _gameData;
        GameData*               _clonedGameData;
        std::vector<int>        _hisFrom;
        std::vector<int>        _hisTo;
        std::vector<int>        _hisRes;
        std::vector<Vec2>       _hisSupply;
        int                     _timeCounter;
        int                     _dataIdx;
        int                     _curStatus;
        MenuItemImage*          _startBtn;
};

#endif /* ReplayLastScene_hpp */
