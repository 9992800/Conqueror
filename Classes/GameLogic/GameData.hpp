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

class DiceGame;
class MapCreator;

class GameData{
        friend class DiceGame;
        friend class MapCreator;
        
public:
        GameData(int n = 3);
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
