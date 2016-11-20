//
//  UserSessionBean.hpp
//  Conqueror
//
//  Created by wsli on 16/11/18.
//
//

#ifndef UserSessionBean_hpp
#define UserSessionBean_hpp
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/WebSocket.h"
#include "network/HttpClient.h"

using namespace network;
USING_NS_CC;

class UserSessionBean:public Ref{
public:
        static UserSessionBean* getInstance();
        UserSessionBean();
        ~UserSessionBean();
        bool init();
        
        static bool checkResponse(HttpResponse*, rapidjson::Value&);
        
public:
        inline std::string getUserId(){
                return _fbUserId;
        }
        
private:
        std::string     _fbUserId;
        std::string     _sessionId;
        std::string     _userToken;
};

#endif /* UserSessionBean_hpp */
