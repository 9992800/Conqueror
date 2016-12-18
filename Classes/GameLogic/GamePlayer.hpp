//
//  GamePlayer.hpp
//  DiceWars
//
//  Created by Wansheng Li on 16/10/12.
//
//

#ifndef GamePlayer_hpp
#define GamePlayer_hpp

#include "AppMacros.hpp"
#include "cocos2d.h"
USING_NS_CC;

class GamePlayer: public Ref{
public:
        static int getGid(int owner_id){
                return owner_id + 2;
        }
        GamePlayer(int id);
        GamePlayer(GamePlayer* obj);
        virtual ~GamePlayer();
        inline void setAreaTc(int tc){
                _area_tc = tc;
        }
        inline int getAreaTc(){
                return _area_tc + _addtionalSupply;
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
                _stock += (_area_tc + _addtionalSupply);
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
        
        inline void setFlagImg(std::string str){
                this->_flagImg = str;
        }
        inline std::string getFlagImge(){
                return this->_flagImg;
        }
        
        inline void setAreaColor(Color4F c){
                this->_areaColor = c;
        }
        inline Color4F getAreaColor(){
                return this->_areaColor;
        }
        
        inline void setPosCharactorIdx(int i){
                this->_charactorIdx = i;
        }
        inline int getPosCharactorIdx(){
                return this->_charactorIdx;
        }
        
        inline void addMoreSupply(){
                _addtionalSupply += TC_VALUE_ONE_SUPPLY;
        }
        inline void useTheAddSupply(){
                _addtionalSupply = 0;
        }
private:
        int     _player_id;
        int     _area_c;
        int     _area_tc;
        int     _dice_c;
        int     _dice_jun;
        int     _stock;
        std::string _flagImg;
        Color4F _areaColor;
        int     _charactorIdx;
        int     _addtionalSupply;
private:
        int     _gid_inmap;
};
#endif /* GamePlayer_hpp */
