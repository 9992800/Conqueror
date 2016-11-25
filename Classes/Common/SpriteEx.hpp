//
//  SpriteEx.hpp
//  Conqueror
//
//  Created by wsli on 16/11/25.
//
//

#ifndef SpriteEx_hpp
#define SpriteEx_hpp

#include "picojson.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "PluginFacebook/PluginFacebook.h"


USING_NS_CC;
using namespace sdkbox;

class SpriteEx : public Sprite
{
public:
        static SpriteEx* createWithUrl(const std::string& url) ;
        static SpriteEx* create();
        virtual bool init() {
                _image_size.x = 0;
                _image_size.y = 0;
                return Sprite::init();
        }
        
        void setSize(Vec2 s) {
                _image_size = s;
        }
        
        void updateWithUrl(const std::string& url);
        
private:
        cocos2d::Vec2 _image_size;
};

#endif /* SpriteEx_hpp */
