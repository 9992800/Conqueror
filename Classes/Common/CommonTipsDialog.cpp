//
//  CommonTipsDialog.cpp
//  Conqueror
//
//  Created by wsli on 16/12/23.
//
//

#include "CommonTipsDialog.hpp"

CommonTipsDialog* CommonTipsDialog::showModalDialog(cocos2d::Node *parent, std::string text, ui::AbstractCheckButton::ccWidgetClickCallback okCallBack){
        if (!parent)
                return nullptr;
        
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(parent, true);
        
        Director::getInstance()->pause();
        auto tips = CommonTipsDialog::create(text);
        tips->initWithCallback(okCallBack);
        parent->addChild(tips, SUPER_LAYER_PRIVILIEGE, COMMON_TIPS_NODETAG);
        return tips;
}

void CommonTipsDialog::dismissDialog(cocos2d::Node *parent){
        auto tips = parent->getChildByTag(COMMON_TIPS_NODETAG);
        if (!tips) return;
        ((CommonTipsDialog*)tips)->dismiss();
}

void CommonTipsDialog::dismiss(){
        auto  parent = this->getParent();
        if (!parent) return;
        
        this->removeFromParent();
        
        Director::getInstance()->resume();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(parent, true);
}

void CommonTipsDialog::dismiss2(cocos2d::ui::AbstractCheckButton::ccWidgetClickCallback okCallback
                                , Ref* btn){
        
        
        auto  parent = this->getParent();
        if (!parent) return;
        
        this->removeFromParent();
        
        if (nullptr != okCallback){
                okCallback(btn);
        }
        
        Director::getInstance()->resume();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(parent, true);
}

bool CommonTipsDialog::initWithCallback(ui::AbstractCheckButton::ccWidgetClickCallback okCallBack){
        if (nullptr == okCallBack){
                auto tips_back_size = _tipsBack->getContentSize();
                auto ok_button = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
                ok_button->setPosition(Vec2(tips_back_size.width / 2, ok_button->getContentSize().height *1.2f));
                ok_button->setTitleText("OK");
                ok_button->setTitleFontSize(28);
                ok_button->setTitleColor(Color3B::BLACK);
                ok_button->setTitleFontName("fonts/arial.ttf");
                ok_button->addClickEventListener(CC_CALLBACK_0(CommonTipsDialog::dismiss, this));
                
                _tipsBack->addChild(ok_button);
        }else{
                
                auto tips_back_size = _tipsBack->getContentSize();
                auto cancel_button = ui::Button::create("DIALOG_CANCEL.png", "DIALOG_CANCEL_SEL.png");
                cancel_button->setPosition(Vec2(tips_back_size.width / 2+ cancel_button->getContentSize().width , cancel_button->getContentSize().height *1.2f));
                cancel_button->setTitleText("NO");
                cancel_button->setTitleFontSize(28);
                cancel_button->setTitleColor(Color3B::BLACK);
                cancel_button->setTitleFontName("fonts/arial.ttf");
                cancel_button->addClickEventListener(CC_CALLBACK_0(CommonTipsDialog::dismiss, this));
                
                _tipsBack->addChild(cancel_button);
                
                
                auto ok_button = cocos2d::ui::Button::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
                ok_button->setPosition(Vec2(tips_back_size.width / 2 - ok_button->getContentSize().width, ok_button->getContentSize().height *1.2f));
                ok_button->setTitleText("YES");
                ok_button->setTitleFontSize(28);
                ok_button->setTitleColor(Color3B::BLACK);
                ok_button->setTitleFontName("fonts/arial.ttf");
                ok_button->addClickEventListener(CC_CALLBACK_0(CommonTipsDialog::dismiss2, this, okCallBack, (Ref*)ok_button));
                
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
        
        
        auto content = Label::createWithSystemFont(_contentTxt, "fonts/arial.ttf", 24);
        auto content_size = content->getContentSize();
        
        content->setAnchorPoint(Vec2(0.f, 1.0f));
        content->setDimensions(content_back_size.width * 0.9,
                                 content_back_size.height * 0.9);
        
        content->setHorizontalAlignment(TextHAlignment::LEFT);
        content->setColor(Color3B::BLACK);
        content->setPosition(Vec2(content_back_size.width * 0.05,
                                    content->getContentSize().height  * .95f));
        content_back->addChild(content);
        
        this->addChild(_tipsBack);
        
        return true;
}
