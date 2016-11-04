//
//  GameData.hpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/3.
//
//

#ifndef GameData_hpp
#define GameData_hpp

#include "AreaData.hpp"
#include "GamePlayer.hpp"
#include "JoinData.hpp"
#include "GameAI.hpp"
#include "cocos2d.h"

USING_NS_CC;

class DiceGame;
class MapCreator;
class AreaData;
class GameAI;

class GameData: public Ref{
        friend class DiceGame;
        friend class MapCreator;
        friend class AreaData;
        friend class GameAI;
public:
        static GameData* create(int n = 3)
        {
                GameData *pRet = new(std::nothrow) GameData(n);
                if (pRet && pRet->init())
                {
                        pRet->autorelease();
                        return pRet;
                }
                else
                {
                        delete pRet;
                        pRet = nullptr;
                        return nullptr;
                }
        }
        
        
        GameData(int n);
        inline bool init(){return true;}
        ~GameData();
public:
        GameData clone();
        void reshDataByMapInfo(TMXTiledMap* map);
private:
        int                             _userId;
        int                             _curPlayerNum;
        int                             _gameStatus;
        int                             _ban;
        int                             _areaFrom;
        int                             _areaTo;
        std::vector<int>                _mapData;
        std::vector<AreaData*>          _areaData;
        std::vector<GamePlayer*>        _player;
        std::vector<JoinData*>          _join;
        std::vector<int>                _jun;
        std::vector<int>                _cel;
        std::vector<int>                _rcel;
        std::vector<int>                _num;
        std::vector<int>                _chk;
        
};

#endif /* GameData_hpp */
