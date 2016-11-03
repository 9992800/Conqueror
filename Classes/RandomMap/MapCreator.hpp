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
class MapCreator{
public:
        static MapCreator* instance();
public:
        TMXTiledMap* createMap(const GameData& gd);
};
#endif /* MapCreator_hpp */
