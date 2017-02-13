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
#include "picojson.h"

using namespace cocos2d::network;
USING_NS_CC;
typedef std::function<void(std::string)> ccReloadAvatarCallback;
class UserSessionBean:public Ref{
public:
        static UserSessionBean* getInstance();
        UserSessionBean();
        ~UserSessionBean();
        bool init();
        
        static bool checkResponse(HttpResponse*, picojson::value&);
public:
        inline std::string getUserId(){
                return this->_fbUserId;
        }
        inline std::string getUserName(){
                return this->_fbUserName;
        }
        inline std::string getUserAvatarImgPath(){
                return this->_fbUserAvatarPath;
        }
        
        void initSession(); 
        void reloadFBAvatar();
        void inviteMyFriendToThisBattle();

protected: 
        
        void onHttpRequestCompleted(HttpClient *sender,
                                    HttpResponse *response);
        
private:
        std::string     _fbUserId;
        std::string     _fbUserName;
        std::string     _fbUserAvatarPath;
};

#endif /* UserSessionBean_hpp */
