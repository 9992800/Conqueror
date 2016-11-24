//
//  OnlineGameData.hpp
//  Conqueror
//
//  Created by wsli on 16/11/21.
//
//

#ifndef OnlineGameData_hpp
#define OnlineGameData_hpp

#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"

USING_NS_CC;
class OnlineGameData : public Ref{
        
        friend class MapCreator;
public:
        CREATE_FUNC(OnlineGameData);
        bool init();
        OnlineGameData();
        ~OnlineGameData();
        
        std::string getMapData();
        inline std::vector<int> getIntMapData(){
                return _mapIntData;
        }
        
        void reshDataByMapInfo(TMXTiledMap*);
        inline void setServerId(std::string id){
                this->_serverId = id;
        }
        inline std::string getServerId(){
                return this->_serverId;
        }
        
        inline void setRole(int r){
                this->_mapRoler = r;
        }
        inline int getRole(){
                return this->_mapRoler;
        }
        
private:
        int                     _playerNum;
        std::vector<int>        _mapIntData;
        rapidjson::Document     _battleData;
        
        TMXTiledMap*            _refereMap;
        std::string             _serverId;
        int                     _mapRoler;
};
#endif /* OnlineGameData_hpp */
