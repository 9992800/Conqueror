//
//  PopupOkDialog.cpp
//  DiceWars
//
//  Created by Wansheng Li on 2016/10/30.
//
//

#include "PopUpOkDialog.hpp"
PopUpOkDialog* PopUpOkDialog::create(BaseDialogConfig config){
        PopUpOkDialog *pRet = new(std::nothrow) PopUpOkDialog(config);
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


PopUpOkDialog* PopUpOkDialog::create(BaseDialogConfig config, ccMenuCallback callback){
        PopUpOkDialog* dialog = PopUpOkDialog::create(config);
        dialog->setCallBack(callback);
        return dialog;
}

PopUpOkDialog::PopUpOkDialog(BaseDialogConfig config):PopUpBaseDialog(config){
        
}

PopUpOkDialog::~PopUpOkDialog(){
        
}
bool PopUpOkDialog::init(){
        Size s = this->getContentSize();
        LayerColor::initWithColor(Color4B(255, 100, 100, 128), s.width, s.height);
        
        _okButton = MenuItemImage::create("DIALOG_OK.png", "DIALOG_OK_SEL.png");
        Size ok_size = _okButton->getContentSize();
        _okButton->setPosition(Vec2(s.width / 2, ok_size.height / 2 + 30));
        
        auto menu = Menu::create(_okButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1, kTagOkButton);
        
        return true;
}

void PopUpOkDialog::setCallBack(ccMenuCallback callback){
        _okButton->setCallback(callback);
}


