//
//  GameSettings.hpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#ifndef GameSettings_hpp
#define GameSettings_hpp
#include "AppMacros.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class GameSettings : public cocos2d::LayerColor
{
public:
        virtual bool init() override;
        CREATE_FUNC(GameSettings);
private:
        void menuExit(Ref*);
        void menuSoundEffect(Ref*, int);
        void menuBackMusic(Ref*, int);
        
private:
        bool            _soundEffectB, _backMusicB;
        int             _soundEffectV, _backMusicV;
        ui::LoadingBar  *_soundEffect, *_backMusic;
};


#endif /* GameSettings_hpp */
