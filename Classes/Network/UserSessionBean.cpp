//
//  UserSessionBean.cpp
//  Conqueror
//
//  Created by wsli on 16/11/18.
//
//
#include "AppMacros.hpp"
#include "UserSessionBean.hpp"

using namespace sdkbox;

static UserSessionBean* s_SharedBean;

UserSessionBean* UserSessionBean::getInstance()
{
        if (!s_SharedBean){
                s_SharedBean = new (std::nothrow) UserSessionBean();
                CCASSERT(s_SharedBean, "FATAL: Not enough memory");
                s_SharedBean->init();
        }
        
        return s_SharedBean;
}

bool UserSessionBean::init(){
        PluginFacebook::setListener(this);
        PluginFacebook::init();
        this->initSession();
        return true;
}

UserSessionBean::UserSessionBean(){
        _fbUserId       = "";
        _accessToken    = "";
        _fbUserAvatar    = "";
}

UserSessionBean::~UserSessionBean(){
}

#pragma mark - intance functions

void UserSessionBean::initSession(){
        if (!PluginFacebook::isLoggedIn()){
                std::vector<std::string> permissions;
                permissions.push_back(FB_PERM_READ_PUBLIC_PROFILE);
                permissions.push_back(sdkbox::FB_PERM_READ_EMAIL);
                permissions.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
                PluginFacebook::login(permissions);
        }else{
                this->fillSessionByFBInfo();
        }
}

void UserSessionBean::fillSessionByFBInfo(){
        this->_fbUserId = PluginFacebook::getUserID();
        this->_accessToken = PluginFacebook::getAccessToken();
        if (_fbUserAvatar.length() == 0){
                sdkbox::FBAPIParam params;
                params["fields"] = "picture";
                params["type"] = "small";
                params["redirect"] = "false";
                PluginFacebook::api("me", "GET", params, "__fetch_picture_tag__");
        }else{
                this->_fbUserAvatar = UserDefault::getInstance()->getStringForKey(FACEBOOK_INFO_USER_AVATAR_KEY, "");
        }
}

std::string UserSessionBean::getUserId(){
        if (_fbUserId.length() == 0){
                this->initSession();
        }
        
        return this->_fbUserId;
}

#pragma mark - facebook callback


/*********************
 * Facebook callbacks
 *********************/
void UserSessionBean::onLogin(bool isLogin, const std::string& error)
{
        CCLOG("##FB isLogin: %d, error: %s id=%s", isLogin, error.c_str(), PluginFacebook::getUserID().c_str());
        this->fillSessionByFBInfo();
}

void UserSessionBean::onAPI(const std::string& tag, const std::string& jsonData)
{
        CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
        if (tag == "__fetch_picture_tag__"){
                //TODO:: move face book to user bean;
        }
}

void UserSessionBean::onSharedSuccess(const std::string& message)
{
        CCLOG("##FB onSharedSuccess:%s", message.c_str());
}

void UserSessionBean::onSharedFailed(const std::string& message)
{
        CCLOG("##FB onSharedFailed:%s", message.c_str());
}

void UserSessionBean::onSharedCancel()
{
        CCLOG("##FB onSharedCancel");
}

void UserSessionBean::onPermission(bool isLogin, const std::string& error)
{
        CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str()); }

void UserSessionBean::onFetchFriends(bool ok, const std::string& msg)
{
        CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
}

void UserSessionBean::onRequestInvitableFriends( const FBInvitableFriendsInfo& friends )
{
        CCLOG("Request Inviteable Friends Begin");
        for (auto it = friends.begin(); it != friends.end(); ++it) {
                CCLOG("Invitable friend: %s", it->getName().c_str());
        }
        CCLOG("Request Inviteable Friends End");
}

void UserSessionBean::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
        CCLOG("on invite friends with invite ids %s= '%s'", result?"ok":"error", msg.c_str());
}

void UserSessionBean::onInviteFriendsResult( bool result, const std::string& msg )
{
        CCLOG("on invite friends %s= '%s'", result?"ok":"error", msg.c_str());
}

void UserSessionBean::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
        CCLOG("Facebook id:'%s' name:'%s' last_name:'%s' first_name:'%s' email:'%s' installed:'%d'",
              userInfo.getUserId().c_str(),
              userInfo.getName().c_str(),
              userInfo.getFirstName().c_str(),
              userInfo.getLastName().c_str(),
              userInfo.getEmail().c_str(),
              userInfo.isInstalled ? 1 : 0
              );
}



#pragma mark - utils functions

bool UserSessionBean::checkResponse(HttpResponse *response, rapidjson::Value& data){
        if (!response){
                return false;
        }
        
        log("response code: %ld", response->getResponseCode());
        
        if (!response->isSucceed()){
                log("error buffer: %s", response->getErrorBuffer());
                return false;
        }
        std::vector<char>* buffer = response->getResponseData();
        std::string response_str(buffer->data(), buffer->size());
        log("response string: %s", response_str.c_str());
        
        if (200 != response->getResponseCode()){
                log("response not ok: %ld", response->getResponseCode());
                return false;
        }
        
        rapidjson::Document json_result;
        json_result.Parse<0>(response_str.c_str());
        if (json_result.HasParseError()) {
                CCLOGWARN("---data from game server failed: %u\n",json_result.GetParseError());
                return false;
        }
        
        int api_code = json_result["apicode"].GetInt();
        std::string  message = json_result["message"].GetString();
        CCLOGINFO("[api code:%d message:%s]", api_code, message);
        if (NETWORK_WORK_WELL != api_code){
                CCLOGWARN("---game server network faild(%d):%s", api_code, response_str.c_str());
                return false;
        }
        
        if (json_result.HasMember("data")){
                data = json_result["data"];
        }
        
        return true;
}


