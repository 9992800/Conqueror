//
//  ReplayLastScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/14.
//
//

#ifndef ReplayLastScene_hpp
#define ReplayLastScene_hpp
#include "AppMacros.hpp"
#include "cocos2d.h"
USING_NS_CC;

class ReplayLast : public cocos2d::Layer{
public:
        static Scene* createScene(Data data);
        virtual bool init() override;
        CREATE_FUNC2(ReplayLast, Data, data);
        ReplayLast(Data data):_theData(data){
        }
private:
        Data _theData;
};

#endif /* ReplayLastScene_hpp */
