//
//  CommonTipsDialog.cpp
//  Conqueror
//
//  Created by wsli on 16/12/23.
//
//

#include "ui/CocosGUI.h"
#include "CommonTipsDialog.hpp"

void CommonTipsDialog::showModalDialog(cocos2d::Node *parent, std::string text, ui::AbstractCheckButton::ccWidgetClickCallback okCallBack){
        if (!parent)
                return;
        
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(parent, true);
        auto tips = CommonTipsDialog::create(text);
        tips->initWithCallback(okCallBack);
        parent->addChild(tips, PRIVILIEGE, NODETAG);
}

void CommonTipsDialog::dismissDialog(){
        auto  parent = this->getParent();
        if (!parent) return;
        
        this->removeFromParent();
        
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(parent, true);
}

bool CommonTipsDialog::initWithCallback(ui::AbstractCheckButton::ccWidgetClickCallback okCallBack){
        if (nullptr == okCallBack){
                auto tips_back_size = _tipsBack->getContentSize();
                auto ok_button = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
                ok_button->::CommonTipsDialog::setPosition(tips_back_size.width / 2, ok_button->getContentSize().height *1.5f);
                ok_button->setTitleText("确定");
                ok_button->setTitleFontSize(28);
                ok_button->setTitleColor(Color3B::BLACK);
                ok_button->setTitleFontName("Arial");
                ok_button->addClickEventListener(CC_CALLBACK_0(CommonTipsDialog::dismissDialog, this));
                
                _tipsBack->addChild(ok_button);
        }else{
                
                auto tips_back_size = _tipsBack->getContentSize();
                auto cancel_button = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
                cancel_button->::CommonTipsDialog::setPosition(tips_back_size.width / 2+ cancel_button->getContentSize().width , cancel_button->getContentSize().height *1.5f);
                cancel_button->setTitleText("取消");
                cancel_button->setTitleFontSize(28);
                cancel_button->setTitleColor(Color3B::BLACK);
                cancel_button->setTitleFontName("Arial");
                cancel_button->addClickEventListener(CC_CALLBACK_0(CommonTipsDialog::dismissDialog, this));
                
                _tipsBack->addChild(cancel_button);
                
                
                auto ok_button = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
                ok_button->::CommonTipsDialog::setPosition(tips_back_size.width / 2 - ok_button->getContentSize().width, ok_button->getContentSize().height *1.5f);
                ok_button->setTitleText("确定");
                ok_button->setTitleFontSize(28);
                ok_button->setTitleColor(Color3B::BLACK);
                ok_button->setTitleFontName("Arial");
                ok_button->addClickEventListener(okCallBack);
                
                _tipsBack->addChild(ok_button);
        }
        
        return true;
}

bool CommonTipsDialog::init(){
        if (!Layer::init()){
                return false;
        }
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        
        _tipsBack = Sprite::create("common_dialog_back.png");
        auto tips_back_size = _tipsBack->getContentSize();
        this->setContentSize(tips_back_size);
        this->setPosition(visible_size / 2);
        
        auto tips_title = Sprite::create("common_dialog_title.png");
        tips_title->setPosition(tips_back_size.width / 2,
                tips_back_size.height + tips_title->getContentSize().height * 0.2);
        _tipsBack->addChild(tips_title);
        
        
        auto content_back = Sprite::create("common_dialog_c.png");
        content_back->setPosition(tips_back_size.width / 2,
                                 tips_back_size.height - content_back->getContentSize().height * 0.58f);
        auto content_back_size = content_back->getContentSize();
        _tipsBack->addChild(content_back);
        
        
        auto content = Label::createWithSystemFont(_contentTxt, "Arial", 24);
        auto content_size = content->getContentSize();
        
        content->setAnchorPoint(Vec2(0.f, 1.0f));
        content->setDimensions(content_back_size.width * 0.9,
                                 content_back_size.height * 0.9);
        
        content->setHorizontalAlignment(TextHAlignment::LEFT);
        content->setColor(Color3B::BLACK);
        content->setPosition(Vec2(content_back_size.width * 0.05,
                                    content->getContentSize().height  * .5f));
        content_back->addChild(content);
        
        this->addChild(_tipsBack);
        
        return true;
}
