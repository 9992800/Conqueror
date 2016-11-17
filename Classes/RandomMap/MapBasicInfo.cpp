//
//  MapBasicInfo.cpp
//  DiceWars
//
//  Created by wsli on 16/10/18.
//
//
#include "MapBasicInfo.hpp"

static MapResolustionConfig _curConfig;

void MapResolustion::initConfig(int solution){
        switch (solution) {
                case MapResolustion::BIG_SCREEN_RESOLUTION:
                        _curConfig = {51.75, 52.5, 16.5, 51.75, 52.5, "tilemap7.png", 155.25, 103.5};
                        
                        break;
                case MapResolustion::MEDIUM_SCREEN_RESOLUTION:
                        _curConfig = {34.5, 35, 11, 34.5, 35, "tilemap6.png", 103.5, 69};
                        
                        break;
                        
                default:
                case MapResolustion::SMALL_SCREEN_RESOLUTION:
                        _curConfig = {34.5, 35, 11, 34.5, 35, "tilemap5.png", 103.5, 103.5};               
                        break;
        }
}

const MapResolustionConfig& MapResolustion::getConfig(){
        return  _curConfig;
}
