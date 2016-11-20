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
        return true;
}

UserSessionBean::UserSessionBean(){
        _fbUserId = "this_is_test";
}

UserSessionBean::~UserSessionBean(){
}


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
