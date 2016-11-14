//
//  ReplayLastScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/14.
//
//

#include "ReplayLastScene.hpp"
Scene* ReplayLast::createScene(Data data){
        auto scene = Scene::create();
        auto layer = ReplayLast::create(data);
        
        scene->addChild(layer);
        return scene;
}

bool ReplayLast::init(){
        return true;
}
