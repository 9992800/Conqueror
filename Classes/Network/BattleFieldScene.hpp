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
#include "network/HttpClient.h"

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
        
        virtual void onOpen(network::WebSocket* ws)override;
        virtual void onMessage(network::WebSocket* ws, const network::WebSocket::Data& data)override;
        virtual void onClose(network::WebSocket* ws)override;
        virtual void onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error)override;
        
        void onHttpRequestCompleted(network::HttpClient*,
                                    network::HttpResponse*);
protected:
        void startBattle();
        void menuExit(Ref*);
        void sendKeepAliveData();
        int  sendMessage(std::string);
        void exitGame();
private:
        network::WebSocket* _wsiSendText;
        float           _lowestPostion_y;
        OnlineGameData* _onlineMapData;
};

#endif /* BattleFieldScene_hpp */
