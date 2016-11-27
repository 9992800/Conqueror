//
//  PopUpOkCancelDialog.hpp
//  DiceWars
//
//  Created by Wansheng Li on 2016/10/30.
//
//

#ifndef PopUpOkCancelDialog_hpp
#define PopUpOkCancelDialog_hpp

#include "PopUpBaseDialog.hpp"

class PopUpOkCancelDialog: public PopUpBaseDialog{
public:
        PopUpOkCancelDialog(BaseDialogConfig config);
        ~PopUpOkCancelDialog();
        bool init(); 
        static PopUpOkCancelDialog* create(BaseDialogConfig config);
        static PopUpOkCancelDialog*  create(BaseDialogConfig config, ccMenuCallback callback1, ccMenuCallback callback2);
        void setCallBack(ccMenuCallback callback1, ccMenuCallback callback2);
        inline void setButtonTittle(std::string o, std::string c){
                _okLabel->setString(o);
                _cancelLabel->setString(c);
        }
private:
        ccMenuCallback  _okCallback;
        ccMenuCallback  _cancelCallback;
        MenuItemImage   *_okButton;
        MenuItemImage   *_cancelButton;
        Label           *_okLabel;
        Label           *_cancelLabel;
};

#endif /* PopUpOkCancelDialog_hpp */
