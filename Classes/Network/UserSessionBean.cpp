//
//  UserSessionBean.cpp
//  Conqueror
//
//  Created by wsli on 16/11/18.
//
//
#include "AppMacros.hpp"
#include "UserSessionBean.hpp"
  
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
        this->initSession();
        return true;
}

UserSessionBean::UserSessionBean(){
        _fbUserId       = "";
        _fbUserAvatarPath    = ""; 
}

UserSessionBean::~UserSessionBean(){
}

#pragma mark - intance functions

void UserSessionBean::initSession(){
        std::string basic_info = UserDefault::getInstance()->getStringForKey(FACEBOOK_INFO_USER_FB_BASIC, "");
        
        std::string uuid = cocos2d::UserDefault::getInstance()->getStringForKey(USER_DEVICE_UUID, "");
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
        
        network::HttpRequest* request = new network::HttpRequest(); 
        request->setRequestType(network::HttpRequest::Type::GET);
        request->setResponseCallback(CC_CALLBACK_2(UserSessionBean::onHttpRequestCompleted, this));
        network::HttpClient::getInstance()->send(request);
        request->release();
}

void UserSessionBean::inviteMyFriendToThisBattle(){
        
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


