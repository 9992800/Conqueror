//
//  PopUpBaseDialog.cpp
//  DiceWars
//
//  Created by Wansheng Li on 2016/10/30.
//
//
#include "PopUpBaseDialog.hpp"
PopUpBaseDialog::PopUpBaseDialog(BaseDialogConfig config):LayerColor::LayerColor(){
        _baseConfig = config;
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        Size  layer_size = visibleSize / 2;
        this->setContentSize(layer_size);
        this->setPosition(center);
        this->setIgnoreAnchorPointForPosition(false);
        this->setAnchorPoint(Vec2(0.5f, 0.5f));
        
        _backGound = cocos2d::ui::Scale9Sprite::create(_baseConfig._backGroundImg);
        _backGound->setContentSize(layer_size);
        _backGound->setCapInsets(Rect(12, 12, 40, 40));
        _backGound->setPosition(Vec2(layer_size/2));
        this->addChild(_backGound, 0, kTagBackGrd);
        
        
        _title = Label::createWithSystemFont(config._title, "", config._titleFontSize);
        Size title_size = _title->getContentSize();
        _title->setPosition(Vec2(layer_size.width / 2, layer_size.height - title_size.height));
        _title->setColor(Color3B::RED);
        this->addChild(_title, 1, kTagTittle);

       
        _content = Label::createWithSystemFont(config._content, "", config._contentFontSize);
        addChild(_content, 1, kTagContent);
        _content->setColor(Color3B::RED);
        Size content_size = _content->getContentSize();
        _content->setPosition(Vec2::ZERO);
        _content->setAnchorPoint(Vec2(0.f, 1.0f));
        _content->setPosition(Vec2(_baseConfig._padding,
                                   layer_size.height - title_size.height - _baseConfig._paddingTop));
        
        _content->setDimensions(layer_size.width - _baseConfig._padding * 2,
                                layer_size.height - _baseConfig._paddingTop - title_size.height);
        
        _content->setHorizontalAlignment(TextHAlignment::LEFT);
}

PopUpBaseDialog::~PopUpBaseDialog(){
        
}
