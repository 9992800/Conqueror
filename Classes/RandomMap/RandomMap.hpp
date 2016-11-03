//
//  RandomMap.hpp
//  DiceWars
//
//  Created by wsli on 16/10/10.
//
//

#ifndef __DICE_WARS_RandomMap_H
#define __DICE_WARS_RandomMap_H

#include "MapBasicInfo.hpp"
#include "cocos2d.h"
#include "external/tinyxml2/tinyxml2.h"

using namespace tinyxml2;
USING_NS_CC;

class RandomMap :public Ref, public XMLDocument{
public:
        static RandomMap* create(const SimpleMapInfoBean& infos);
        std::string getXmlString();
        
protected:
        RandomMap();
        bool init(const SimpleMapInfoBean& infos);
        void initRootMap(const MapBasicBean& basic);
        void initTilesSet(const TileSetBean& tileset);
        void initLayer(const LayerBean& layer);
private:
        XMLElement* _rootElement;
};

#endif /* RandomMap_hpp */
