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
                        _curConfig = {60, 70, 34, 60, 70, "tilemap6.png", 180, 140};
                        break;
                case MapResolustion::MEDIUM_SCREEN_RESOLUTION:
                        _curConfig = {30, 35, 17, 30, 35, "tilemap5.png", 90, 70};
                        break;
                        
                default:
                case MapResolustion::SMALL_SCREEN_RESOLUTION:
                        _curConfig = {30, 34, 16, 30, 34, "tilemap7.png", 90, 70};
               
                        break;
        }
}

const MapResolustionConfig& MapResolustion::getConfig(){
        return  _curConfig;
}
