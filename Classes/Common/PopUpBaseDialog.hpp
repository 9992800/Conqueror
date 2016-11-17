//
//  PopUpBaseDialog.hpp
//  DiceWars
//
//  Created by Wansheng Li on 2016/10/30.
//
//

#ifndef PopUpBaseDialog_hpp
#define PopUpBaseDialog_hpp
#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"


USING_NS_CC;

enum{
        kTagBackGrd = 1,
        kTagTittle,
        kTagContent,
        kTagOkButton
};

typedef struct BaseDialogConfig_tg {
        std::string     _backGroundImg;
        std::string     _title;
        std::string     _content;
        int             _titleFontSize;
        int             _contentFontSize;
        int             _padding;
        int             _paddingTop;
        BaseDialogConfig_tg(std::string title, std::string content){
                _title = title;
                _content = content;
                _backGroundImg = "DIALOG_BACKGROUND.png";
                _titleFontSize = 20;
                _contentFontSize = 20;
                _padding = 50;
                _paddingTop = 30;
        }
        
        
        BaseDialogConfig_tg(){
                _backGroundImg = "DIALOG_BACKGROUND.png";
                _titleFontSize = 20;
                _contentFontSize = 20;
                _padding = 50;
                _paddingTop = 30;
        }
        void setTitle(std::string t){
                _title = t;
        }
        void setContent(std::string c){
                _content = c;
        }
        
}BaseDialogConfig;

class PopUpBaseDialog: public LayerColor{
public:
        PopUpBaseDialog(BaseDialogConfig config);
        ~PopUpBaseDialog(); 
        
protected:
        BaseDialogConfig        _baseConfig;
        ui::Scale9Sprite*       _backGound;
        Label*                  _title;
        Label*                  _content;

};

#endif /* PopUpBaseDialog_hpp */
