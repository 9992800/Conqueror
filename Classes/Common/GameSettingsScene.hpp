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
        GameSettings(Node* parent):_parentLayer(parent){
                Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_parentLayer, true);
        }
        ~GameSettings(){
                Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(_parentLayer, true);
        }
        virtual bool init() override;
        CREATE_FUNC2(GameSettings, Node*, parent);
private:
        void menuExit(Ref*);
        void menuAnimSwitch(Ref*);
        void menuSoundEffect(Ref*, int);
        void menuBackMusic(Ref*, int);
        void menuSpeedChange(Ref*, int);
private:
        bool            _soundEffectB, _backMusicB, _isSoundOn;
        int             _soundEffectV, _backMusicV;
        bool            _animSwitch;
        int             _gameSpeed;
        ui::LoadingBar  *_soundEffect, *_backMusic;
        ui::Button     *_speedBtn1, *_speedBtn2,*_speedBtn3;
        Node*           _parentLayer;
};


#endif /* GameSettings_hpp */
