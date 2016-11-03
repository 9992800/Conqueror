//
//  RandomMap.cpp
//  TesMap
//
//  Created by Wansheng Li on 16/10/10.
//
//

#include "RandomMap.hpp"

RandomMap* RandomMap::create(const SimpleMapInfoBean& infos){
        RandomMap* ret = new (std::nothrow)RandomMap();
        if (ret->init(infos)){
                ret->autorelease();
                return ret;
        }
        
        CC_SAFE_DELETE(ret);
        
        return nullptr;
}

std::string RandomMap::getXmlString(){
        XMLPrinter printer;
        this->Print(&printer);
        
        return std::string(printer.CStr());
}

RandomMap::RandomMap():XMLDocument(){
}


void RandomMap::initRootMap(const MapBasicBean& basic){
        
        _rootElement = this->NewElement("map");
        _rootElement->SetAttribute("version", "1.0");
        _rootElement->SetAttribute("orientation",       basic.orientation.c_str());
        _rootElement->SetAttribute("renderorder",       basic.renderorder.c_str());
        _rootElement->SetAttribute("width",             basic.widthInTiles);
        _rootElement->SetAttribute("height",            basic.heightInTiles);
        _rootElement->SetAttribute("tilewidth",         basic.tileWidth);
        _rootElement->SetAttribute("tileheight",        basic.tileHeight);
        _rootElement->SetAttribute("hexsidelength",     basic.hexsidelength);
        _rootElement->SetAttribute("staggeraxis",       basic.staggeraxis.c_str());
        _rootElement->SetAttribute("staggerindex",      basic.staggerindex.c_str());
        _rootElement->SetAttribute("nextobjectid",      1);
        
        this->InsertFirstChild(_rootElement);
} 

void RandomMap::initTilesSet(const TileSetBean& tilesetInfo){
        XMLElement * tilesSet = this->NewElement("tileset");
        
        {
                const ImageBean& imageInfo =  tilesetInfo.imageBean;
                
                XMLElement* image = this->NewElement("image");
                image->SetAttribute("source",   imageInfo.source.c_str());
                image->SetAttribute("width",    imageInfo.width);
                image->SetAttribute("height",   imageInfo.height);

                tilesSet->InsertEndChild(image);
        }
        
        tilesSet->SetAttribute("firstgid",      "1");
        tilesSet->SetAttribute("name",          tilesetInfo.name.c_str());
        tilesSet->SetAttribute("tilewidth",     tilesetInfo.tilewidth);
        tilesSet->SetAttribute("tileheight",    tilesetInfo.tileheight);
        tilesSet->SetAttribute("tilecount",     tilesetInfo.tilecount);
        tilesSet->SetAttribute("columns",       tilesetInfo.columns);
        
        _rootElement->InsertEndChild(tilesSet);
}


void RandomMap::initLayer(const LayerBean& layerInfo){
        XMLElement * layer = this->NewElement("layer");
        XMLElement* data = this->NewElement("data");
        {
                const std::vector<int> tile_list = layerInfo.datas;
                for (const int& gid: tile_list){
                        XMLElement* tile = this->NewElement("tile");
                        tile->SetAttribute("gid", gid);
                        
                        data->InsertEndChild(tile);
                }
        }
        layer->InsertEndChild(data);
        
        
        layer->SetAttribute("name",     layerInfo.name.c_str());
        layer->SetAttribute("width",    layerInfo.width);
        layer->SetAttribute("height",   layerInfo.height);
        layer->SetAttribute("opacity",  layerInfo.opacity);

        _rootElement->InsertEndChild(layer);
}

bool RandomMap::init(const SimpleMapInfoBean& infos){
        this->initRootMap(infos.mapBasicBean);
        this->initTilesSet(infos.tileSetBean);
        this->initLayer(infos.layerBean);
        return true;
}


