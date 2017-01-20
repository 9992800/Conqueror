//
//  PopUpOkCancelDialog.cpp
//  DiceWars
//
//  Created by Wansheng Li on 2016/10/30.
//
//
#include "AppConstants.hpp"
#include "PopUpOkCancelDialog.hpp"

PopUpOkCancelDialog* PopUpOkCancelDialog::create(BaseDialogConfig config){
        PopUpOkCancelDialog *pRet = new(std::nothrow) PopUpOkCancelDialog(config);
        if (pRet && pRet->init())
        {
                pRet->autorelease();
                return pRet;
        }
        else
        {
                delete pRet;
                pRet = nullptr;
                return nullptr;
        }
}


PopUpOkCancelDialog*  PopUpOkCancelDialog::create(BaseDialogConfig config, ccMenuCallback callback1, ccMenuCallback callback2){
        
        PopUpOkCancelDialog* dialog = PopUpOkCancelDialog::create(config);
        dialog->setCallBack(callback1, callback2);
        
        return dialog;
}
PopUpOkCancelDialog::PopUpOkCancelDialog(BaseDialogConfig config):PopUpBaseDialog(config){
        
}
PopUpOkCancelDialog::~PopUpOkCancelDialog(){
        
}

bool PopUpOkCancelDialog::init(){
        if ( !LayerColor::init() ) {
                return false;
        }
        Size s = this->_backGound->getContentSize();
        LayerColor::initWithColor(Color4B(255, 0, 0, 255), s.width, s.height);
        
        _okButton = MenuItemImage::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        Size ok_size = _okButton->getContentSize();
        float button_gap = s.width * 0.2;
        _okButton->setPosition(Vec2(s.width / 2 - button_gap,  ok_size.height / 2 + 30));
        
        _okLabel = Label::createWithSystemFont("OK", SYSTEM_FONTS, 24);
        _okButton->addChild(_okLabel);
        _okLabel->setPosition(Vec2(ok_size / 2));
        
        _cancelButton = MenuItemImage::create("DIALOG_CANCEL.png", "DIALOG_CANCEL_SEL.png");
        Size cancel_size = _cancelButton->getContentSize();
        _cancelButton->setPosition(Vec2(s.width / 2 + button_gap, cancel_size.height / 2 + 30));
        
        _cancelLabel = Label::createWithSystemFont("Cancel", SYSTEM_FONTS, 24);
        _cancelButton->addChild(_cancelLabel);
        _cancelLabel->setPosition(Vec2(cancel_size / 2));
        
        auto menu = Menu::create(_okButton, _cancelButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1, kTagOkButton);
        
        return true;
} 
void PopUpOkCancelDialog::setCallBack(ccMenuCallback callback1, ccMenuCallback callback2){
        _okButton->setCallback(callback1);
        _cancelButton->setCallback(callback2);
} 
