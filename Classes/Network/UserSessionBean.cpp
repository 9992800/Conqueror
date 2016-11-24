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
        _fbUserAvatarPath    = "";
        _fbAvatarUrl = "";
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
        if (_fbUserAvatarPath.length() == 0){
                sdkbox::FBAPIParam params;
                params["fields"] = "picture";
                params["type"] = "small";
                params["redirect"] = "false";
                PluginFacebook::api("me", "GET", params, "__fetch_picture_tag__");
        }else{
                this->_fbUserAvatarPath = UserDefault::getInstance()->getStringForKey(FACEBOOK_INFO_USER_AVATAR_KEY, "");
        }
}

std::string UserSessionBean::getUserId(){        
        return this->_fbUserId;
}

void UserSessionBean::onHttpRequestCompleted(HttpClient *sender,
                            HttpResponse *response){
        
        if (!response->isSucceed()) {
                CCLOG("ERROR, remote sprite load image failed");
                return ;
        }
        std::vector<char> *buffer = response->getResponseData();
        Image img;
        img.initWithImageData(reinterpret_cast<unsigned char*>(&(buffer->front())), buffer->size());
        
        // save image file to device.
        std::string path = FileUtils::getInstance()->getWritablePath() + this->_fbUserId + ".png";
        CCLOG("save image path = %s", path.data());
        bool ret = img.saveToFile(path);
        CCLOG("save file %s", ret ? "success" : "failure");
        this->_fbUserAvatarPath = path;
        UserDefault::getInstance()->setStringForKey(FACEBOOK_INFO_USER_AVATAR_KEY, path);
        UserDefault::getInstance()->flush();
}

void UserSessionBean::reloadFBAvatar(){
        sdkbox::FBAPIParam params;
        params["fields"] = "picture";
        params["type"] = "small";
        params["redirect"] = "false";
        PluginFacebook::api("me", "GET", params, "__fetch_picture_tag__");
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
                picojson::value v;
                picojson::parse(v, jsonData);
                if (!v.contains("picture")){
                        return;
                }
                std::string url = v.get("picture").get("data").get("url").to_str();
                CCLOG("picture's url = %s", url.data());
                
                this->_fbAvatarUrl = url;
                network::HttpRequest* request = new network::HttpRequest();
                request->setUrl(url.data()); 
                request->setRequestType(network::HttpRequest::Type::GET);
                request->setResponseCallback(CC_CALLBACK_2(UserSessionBean::onHttpRequestCompleted, this));
                network::HttpClient::getInstance()->send(request);
                request->release();
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

bool UserSessionBean::checkResponse(HttpResponse *response, picojson::value& data){
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
        
        picojson::value json_result;
        std::string error = picojson::parse(json_result, response_str);
        if (error.length() > 0) {
                CCLOGWARN("---data from game server failed: %s\n",error.c_str());
                return false;
        }
        
        picojson::object result = json_result.get<picojson::object>();
        
        int api_code = result["apicode"].get<double>();
        std::string  message = result["message"].get<std::string>();
        CCLOGINFO("[api code:%d message:%s]", api_code, message);
        if (NETWORK_WORK_WELL != api_code){
                CCLOGWARN("---game server network faild(%d):%s", api_code, response_str.c_str());
                return false;
        }
        
        if (json_result.contains("data")){
                data = result["data"];
        }
        
        return true;
}


