//
//  GamePlayer.hpp
//  DiceWars
//
//  Created by Wansheng Li on 16/10/12.
//
//

#ifndef GamePlayer_hpp
#define GamePlayer_hpp

#include "GameConfig.hpp"
#include "cocos2d.h"
USING_NS_CC;

class GamePlayer: public Ref{
public:
        GamePlayer(int id);
        virtual ~GamePlayer();
        inline void setAreaTc(int tc){
                _area_tc = tc;
        }
        inline int getAreaTc(){
                return _area_tc;
        }
        inline void increaseAreaC(){
                ++_area_c;
        }
        inline void addDiceC(int dice){
                _dice_c += dice;
        }
        
        inline int getDiceC(){
                return _dice_c;
        }
        inline int getDiceJun(){
                return _dice_jun;
        }
        inline void setDiceJun(int jun){
                _dice_jun = jun;
        }
        
        inline void resetValue(){
                _area_c = 0;
                _dice_c = 0;
        }
        inline void setStock(){
                _stock += _area_tc;
                if (_stock >= STOCK_MAX){
                        _stock = STOCK_MAX;
                }
        }
        inline int getStock(){
                return _stock;
        }
        
        inline void decreaseStock(){
                --_stock;
        }
        
        inline int getGid(){
                return _gid_inmap;
        }
private:
        int     _player_id;
        int     _area_c;
        int     _area_tc;
        int     _dice_c;
        int     _dice_jun;
        int     _stock;
private:
        int     _gid_inmap;
};
#endif /* GamePlayer_hpp */
