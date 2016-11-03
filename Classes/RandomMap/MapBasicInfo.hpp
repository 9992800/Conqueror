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
        int width;
        int height;
        float opacity;
        std::vector<int> datas;
}LayerBean;

typedef struct ImageBean_s{
        std::string source;
        int width;
        int height;
}ImageBean;

typedef struct TileSetBean_s{
        std::string name;
        int tilewidth;
        int tileheight;
        int tilecount;
        int columns;
        ImageBean imageBean;
}TileSetBean;

typedef struct MapBasicBean_s{
        int widthInTiles;
        int heightInTiles;
        int tileWidth;
        int tileHeight;
        int hexsidelength;
        std::string staggeraxis;
        std::string staggerindex;
        std::string orientation;
        std::string renderorder;
}MapBasicBean;

typedef struct SimpleMapInfoBean_s{
        MapBasicBean mapBasicBean;
        TileSetBean  tileSetBean;
        LayerBean    layerBean;
}SimpleMapInfoBean;


typedef struct MapResolustionConfig_s{
        int mapTileWidth;
        int mapTileHeight;
        int hexsidelength;
        int tilesetWidth;
        int tilesetHeight;
        std::string imgSource;
        int imageWidth;
        int imageHeight;
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
