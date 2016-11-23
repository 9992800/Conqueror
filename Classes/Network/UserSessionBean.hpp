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
        void setUserId(std::string);
        inline void setAccessToken(std::string token){
                this->_accessToken = token;
        }
        inline std::string getUserId(){
                return _fbUserId;
        }
        
        inline bool needLoadPicture(){
                return _picturePath.length() == 0;
        }
        
        inline void setUserAvatar(std::string ip){
                this->_picturePath = ip;
        }
        
        inline bool needReloadFB(){
                return this->_fbUserId.length() == 0;
        }
private:
        std::string     _fbUserId; 
        std::string     _accessToken;
        std::string     _picturePath;
};

#endif /* UserSessionBean_hpp */
