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
#include "picojson.h"

using namespace network;
USING_NS_CC;
typedef std::function<void(std::string)> ccReloadAvatarCallback;
class UserSessionBean:public Ref, sdkbox::FacebookListener{
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
        inline std::string getUserURL(){
                return this->_fbBasiceInfo.getPictureURL();
        }
        inline std::string getUserAvatarImgPath(){
                return this->_fbUserAvatarPath;
        }
        
        void initSession(); 
        void reloadFBAvatar();
        void inviteMyFriendToThisBattle();

protected:
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
        std::string     _fbUserId;
        std::string     _fbUserName;
        std::string     _fbUserAvatarPath;
        sdkbox::FBGraphUser _fbBasiceInfo; 
};

#endif /* UserSessionBean_hpp */
