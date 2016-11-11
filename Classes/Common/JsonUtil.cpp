//
//  JsonUtil.cpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#include "JsonUtil.hpp" 
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"

rapidjson::Document JsonUtil::parseString(std::string str){
        
        CCLOG("%s\n", str.c_str());
        rapidjson::Document d;
        d.Parse<0>(str.c_str());
        if (d.HasParseError()) {
                CCLOG("GetParseError %u\n",d.GetParseError());
                return nullptr;
        }
        
        return d;
}

std::string JsonUtil::parseObject(rapidjson::Document document){
//        document.SetObject();
//        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
//        rapidjson::Value array(rapidjson::kArrayType);
//        rapidjson::Value object(rapidjson::kObjectType);
//        object.AddMember("int", 1, allocator);
//        object.AddMember("double", 1.0, allocator);
//        object.AddMember("bool", true, allocator);
//        object.AddMember("hello", "你好", allocator);
//        array.PushBack(object, allocator);
//        document.AddMember("json", "json string", allocator);
//        document.AddMember("array", array, allocator);
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        
        return buffer.GetString();
}
