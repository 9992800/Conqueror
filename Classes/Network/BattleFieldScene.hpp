//
//  BattleFieldScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#ifndef BattleFieldScene_hpp
#define BattleFieldScene_hpp

#include "cocos2d.h"
#include "network/WebSocket.h"
#include "OnlineGameData.hpp"
#include "AppMacros.hpp"

USING_NS_CC;

class BattleField: public Layer, public cocos2d::network::WebSocket::Delegate {
        
public:
        static Scene* createScene(OnlineGameData*);
        virtual bool init() override;
        CREATE_FUNC2(BattleField, OnlineGameData*, data);
        BattleField(OnlineGameData* data):_onlineMapData(data){
                
        }
        virtual ~BattleField();
        
        void onEnter() override;
        void update(float delta)override;
        void onExit()override;
        
        virtual void onOpen(cocos2d::network::WebSocket* ws)override;
        virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
        virtual void onClose(cocos2d::network::WebSocket* ws)override;
        virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;
        
        
protected:
        void startBattle();
        void menuExit(Ref*);
        void sendKeepAliveData();
        void startTimerKeepAlive();
        int  sendMessage(std::string);
private:
        network::WebSocket* _wsiSendText;
        float           _lowestPostion_y;
        OnlineGameData* _onlineMapData;
};

#endif /* BattleFieldScene_hpp */
