//
//  MapCreator.hpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/3.
//
//

#ifndef MapCreator_hpp
#define MapCreator_hpp
#include "GameData.hpp"
#include "OnlineGameData.hpp"
#include "MapBasicInfo.hpp"

class MapCreator{
public:
        static MapCreator* instance();
        bool init();
        
public:
        TMXTiledMap* createMap(std::vector<int>); 
private:
        SimpleMapInfoBean initMapBasicInfo(std::vector<int>);
};
#endif /* MapCreator_hpp */
