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
        
        this->initMap();
        
        this->initController();
        
        this->initGuideData();
        
        return true;
}

NoviceGuide::~NoviceGuide(){
}

#pragma mark - init logic

void NoviceGuide::initMap(){
}

void NoviceGuide::initController(){
}

void NoviceGuide::initGuideData(){
}

#pragma mark -  update logic

void NoviceGuide::onEnter(){
        Layer::onEnter();
}
void NoviceGuide::update(float delta){
}

void NoviceGuide::onExit(){
        Layer::onExit();
}
