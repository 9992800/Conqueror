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
#include "PluginFacebook/PluginFacebook.h"

using namespace network;
USING_NS_CC;

class UserSessionBean:public Ref, sdkbox::FacebookListener{
public:
        static UserSessionBean* getInstance();
        UserSessionBean();
        ~UserSessionBean();
        bool init();
        
        static bool checkResponse(HttpResponse*, rapidjson::Value&); 
public:
        std::string getUserId();
        void initSession(); 
        
        void onLogin(bool, const std::string&)override;
        void onSharedSuccess(const std::string&)override;
        void onSharedFailed(const std::string&)override;
        void onSharedCancel()override;
        void onAPI(const std::string&, const std::string&)override;
        void onPermission(bool, const std::string&)override;
        void onFetchFriends(bool, const std::string&)override;
        void onRequestInvitableFriends(const sdkbox::FBInvitableFriendsInfo&)override;
        void onInviteFriendsWithInviteIdsResult(bool, const std::string&)override;
        void onInviteFriendsResult(bool, const std::string&)override;
        void onGetUserInfo(const sdkbox::FBGraphUser&)override;
        
        void onHttpRequestCompleted(HttpClient *sender,
                                    HttpResponse *response);
        
private:
        void fillSessionByFBInfo();
private:
        std::string     _fbUserId; 
        std::string     _accessToken;
        std::string     _fbUserAvatarPath;
        std::string     _fbAvatarUrl; 
};

#endif /* UserSessionBean_hpp */
