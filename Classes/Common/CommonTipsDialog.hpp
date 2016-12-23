//
//  CommonTipsDialog.hpp
//  Conqueror
//
//  Created by wsli on 16/12/23.
//
//

#ifndef CommonTipsDialog_hpp
#define CommonTipsDialog_hpp
#include "AppMacros.hpp"
#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"
USING_NS_CC;

class CommonTipsDialog: public Layer{
public:
        static void showModalDialog(Node* parent, std::string, ui::AbstractCheckButton::ccWidgetClickCallback = nullptr);
        void dismissDialog();
        CREATE_FUNC2(CommonTipsDialog, std::string, text);
        virtual bool init()override;
        bool initWithCallback(ui::AbstractCheckButton::ccWidgetClickCallback);
        CommonTipsDialog(std::string text):_contentTxt(text){
        }
private:
        Sprite* _tipsBack;
        std::string _contentTxt;
};
#endif /* CommonTipsDialog_hpp */
