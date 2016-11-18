//
//  ModalDialog.hpp
//  Conqueror
//
//  Created by wsli on 16/11/18.
//
//

#ifndef ModalDialog_hpp
#define ModalDialog_hpp


#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"
USING_NS_CC;

class ModalLayer: public LayerColor{
public:
        static void showModalDialog(Node* parent);
        static void dismissDialog(Node* parent);
        CREATE_FUNC(ModalLayer);
        virtual bool init()override;
        
        virtual void onEnter() override;
        virtual void onExit()override;
        
        void onTouchesMoved(Touch*, Event* event);
        bool onTouchesBegan(Touch*, Event *event);
protected:
        void afterAnimation();
        
private:
        Sprite* _waitingTips;
};
#endif /* ModalDialog_hpp */
