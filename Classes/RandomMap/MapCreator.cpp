//
//  MapCreator.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/3.
//
//
#include "MapCreator.hpp"
#include "RandomMap.hpp"

static MapCreator* s_SharedCreator;
MapCreator* MapCreator::instance(){
        if (!s_SharedCreator){
                s_SharedCreator = new (std::nothrow)MapCreator();
                CCASSERT(s_SharedCreator, "FATAL: Not enough memory");
                s_SharedCreator->init();
        }
        
        return s_SharedCreator;
}

bool MapCreator::init(){
        return true;
}


TMXTiledMap* MapCreator::createMap(GameData* data){
        
        SimpleMapInfoBean simpleBean = this->initMapBasicInfo(data->_mapData);
        
        auto mapxml = RandomMap::create(simpleBean);
        std::string map_xml_str = mapxml->getXmlString();
        
//        printf("\r\n%s", map_xml_str.c_str());
        
        TMXTiledMap* map = TMXTiledMap::createWithXML(map_xml_str, "maps");
        
        data->_refereMap = map;
        return map;
}

SimpleMapInfoBean MapCreator::initMapBasicInfo(std::vector<int> map_data){
        SimpleMapInfoBean simpleBean;
        float row = XMAX, columns = YMAX;
        
        MapResolustionConfig config =  MapResolustion::getConfig();
        
        MapBasicBean mapBasic = {row, columns, config.mapTileWidth, config.mapTileHeight, config.hexsidelength};
        simpleBean.mapBasicBean = mapBasic;
        
        TileSetBean tileSet = {"game", config.tilesetWidth, config.tilesetHeight,
                9, 3, {config.imgSource, config.imageWidth, config.imageHeight}};
        
        simpleBean.tileSetBean = tileSet;
        
        LayerBean layer = {LAYER_NAME_IN_TILE_MAP, row, columns, 1.0};
        layer.datas = map_data;
        simpleBean.layerBean = layer;
        
        return simpleBean;
}
