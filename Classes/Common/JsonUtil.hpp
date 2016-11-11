//
//  JsonUtil.hpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#ifndef JsonUtil_hpp
#define JsonUtil_hpp

#include "json/document.h"

class JsonUtil{
public:
        static rapidjson::Document parseString(std::string);
        static std::string      parseObject(rapidjson::Document);
        static std::string      parseObject(std::map<std::string, void>);
};

#endif /* JsonUtil_hpp */
