//
//  GameSettings.hpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#ifndef GameSettings_hpp
#define GameSettings_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class GameSettings : public cocos2d::LayerColor
{
public:
        virtual bool init() override;
        CREATE_FUNC(GameSettings);
        
private:
        ui::LoadingBar *_soundEffectV, *_backMusicV;
};


#endif /* GameSettings_hpp */
