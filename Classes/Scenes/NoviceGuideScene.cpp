//
//  NoviceGuideScene.cpp
//  Conqueror
//
//  Created by wsli on 17/1/16.
//
//

#include "NoviceGuideScene.hpp"
Scene* NoviceGuide::createScene(){
        
        auto scene = Scene::create();
        auto layer = NoviceGuide::create();
        scene->addChild(layer);
        
        return scene;
}

bool NoviceGuide::init(){
        if (!Layer::init()){
                return false;
        }
        
        return true;
}
