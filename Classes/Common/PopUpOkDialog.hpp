//
//  PopupOkDialog.hpp
//  DiceWars
//
//  Created by Wansheng Li on 2016/10/30.
//
//

#ifndef PopupOkDialog_hpp
#define PopupOkDialog_hpp
#include "PopUpBaseDialog.hpp"

class PopUpOkDialog: public PopUpBaseDialog{
public:
        PopUpOkDialog(BaseDialogConfig config);
        ~PopUpOkDialog();
        bool init();
        void setCallBack(ccMenuCallback callback);
        static PopUpOkDialog* create(BaseDialogConfig config);
        static PopUpOkDialog* create(BaseDialogConfig config, ccMenuCallback callback);
        
protected:
        MenuItemImage *_okButton;
};

#endif /* PopupOkDialog_hpp */
