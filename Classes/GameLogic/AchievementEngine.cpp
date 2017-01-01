//
//  AchievementEngine.cpp
//  Conqueror
//
//  Created by wsli on 17/1/1.
//
//

#include "AchievementEngine.hpp"
static AchievementEngine* s_SharedEngine;

AchievementEngine* AchievementEngine::getInstance(){
        if (!s_SharedEngine) {
                s_SharedEngine = new (std::nothrow) AchievementEngine();
                CCASSERT(s_SharedEngine, "FATAL: Not enough memory");
                s_SharedEngine->init();
        }
        
        return s_SharedEngine;
}

bool AchievementEngine::init(){
        return true;
}
