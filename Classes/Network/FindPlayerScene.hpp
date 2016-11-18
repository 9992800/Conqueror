//
//  FindPlayerScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#ifndef FindPlayerScene_hpp
#define FindPlayerScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AppMacros.hpp"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace network;


class FindPlayer : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(FindPlayer);
        virtual ~FindPlayer();
        
        void onHttpRequestCompleted(HttpClient*, HttpResponse*);
        
protected:
        void afterAnimation();
        void onMessage(std::string);
        void menuSearching(Ref*);
        void menuCreateGame(Ref*);
private:
        int             _curMapSel;
        cocos2d::Label* _labelStatusCode;
};

#endif /* FindPlayerScene_hpp */
