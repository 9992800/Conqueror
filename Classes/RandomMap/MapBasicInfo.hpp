//
//  MapBasicInfo.hpp
//  TesMap
//
//  Created by Wansheng Li on 16/10/10.
//
//

#ifndef _MapBasicInfo_hpp_
#define _MapBasicInfo_hpp_
#include "cocos2d.h"

typedef struct LayerBean_s{
        std::string name;
        float width;
        float height;
        float opacity;
        std::vector<int> datas;
}LayerBean;

typedef struct ImageBean_s{
        std::string source;
        float width;
        float height;
}ImageBean;

typedef struct TileSetBean_s{
        std::string name;
        float tilewidth;
        float tileheight;
        float tilecount;
        float columns;
        ImageBean imageBean;
}TileSetBean;

typedef struct MapBasicBean_s{
        float widthInTiles;
        float heightInTiles;
        float tileWidth;
        float tileHeight;
        float hexsidelength;
}MapBasicBean;

typedef struct SimpleMapInfoBean_s{
        MapBasicBean mapBasicBean;
        TileSetBean  tileSetBean;
        LayerBean    layerBean;
}SimpleMapInfoBean;


typedef struct MapResolustionConfig_s{
        float   mapTileWidth;
        float   mapTileHeight;
        float   hexsidelength;
        float   tilesetWidth;
        float   tilesetHeight;
        std::string imgSource;
        float   imageWidth;
        float   imageHeight;
}MapResolustionConfig;



USING_NS_CC;
class MapResolustion: public Ref{
public:
        
        enum {
                SMALL_SCREEN_RESOLUTION = 1,
                MEDIUM_SCREEN_RESOLUTION = 2,
                BIG_SCREEN_RESOLUTION = 4
        };
        static void initConfig(int solution);
        static const MapResolustionConfig& getConfig();
};


#endif /* _MapBasicInfo_hpp_ */
