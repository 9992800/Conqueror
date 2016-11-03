//
//  AreaData.cpp
//  DiceWars
//
//  Created by Wansheng Li on 16/10/12.
//
//

#include "AreaData.hpp"
#include "DiceGame.hpp"
#include "ScreenCoordinate.hpp"
#include "AppMacros.hpp"

static Color4F _areaLineColor[] = {Color4F(1.0, 0.0, 0.0, 0.02),
        Color4F(0.0, 1.0, 0.0, 0.02),Color4F(0.0, 0.0, 1.0, 0.02),
        Color4F(1.0, 1.0, 0.0, 0.02),Color4F(0.0, 1.0, 1.0, 0.02),
        Color4F(1.0, 0.0, 1.0, 0.02),Color4F(1.0, 1.0, 1.0, 0.02),
        Color4F(0.0, 0.0, 0.0, 0.02)};

static Color4F border_color = Color4F(0.0, 0.0, 0.0, 1.0);
static Color4F selected_color = Color4F(0.2, 0.0, 0.0, 0.7);

AreaData::AreaData(int id)
:_areaId(id),
_drawNode(nullptr),
_size(0),
_cpos(0),
_arm(-1),
_dice(0),
_left(XMAX),
_right(-1),
_top(YMAX),
_bottom(-1),
_cx(0),
_cy(0),
_len_min(9999){
        _join       = std::vector<bool>(AREA_MAX);
        SET_SIZE_TOZERO(_join, AREA_MAX);
        _line_cel   = std::vector<int>(MAX_LINE_INAREA);
        _line_dir   = std::vector<int>(MAX_LINE_INAREA);
        _cell_idxs  = std::set<int>();
        _fight_values = std::vector<int>(MAX_DICE_PER_AREA);
}


AreaData::~AreaData(){
        _join.clear();
        _line_cel.clear();
        _line_dir.clear();
        _cell_idxs.clear();
}


AreaData::AreaData (AreaData* obj){
        this->_size = obj->_size;
        
        this->_size     = obj->_size;
        this->_cpos     = obj->_cpos;
        this->_arm      = obj->_arm;
        this->_dice     = obj->_dice;
        this->_left     = obj->_left;
        this->_right    = obj->_right;
        this->_bottom   = obj->_bottom;
        this->_cx       = obj->_cx;
        this->_cy       = obj->_cy;
        this->_len_min  = obj->_len_min;
        this->_areaId   = obj->_areaId;
        this->_drawNode = obj->_drawNode;
        this->_emitter  = obj->_emitter;
        
        this->_join     = std::vector<bool>(obj->_join);
        this->_line_cel = std::vector<int>(obj->_line_cel);
        this->_line_dir = std::vector<int>(obj->_line_dir);
        this->_cell_idxs= std::vector<int>(obj->_cell_idxs);
        this->_fight_values= std::vector<int>(obj->_fight_values);
}

void AreaData::initBound(int vertical, int horizen){
        
        if (horizen < _left){
                _left = horizen;
        }
        
        if (horizen > _right){
                _right = horizen;
        }
        
        if (vertical < _top){
                _top = vertical;
        }
        if (vertical > _bottom){
                _bottom = vertical;
        }
}



void AreaData::calcLenAndPos(int vertical, int horizen, int cell_idx, DiceGame* game){
        
        int dis_h = _cx - horizen;
        if (dis_h < 0) dis_h = 0 - dis_h;
        
        int dis_v = _cy - vertical;
        if (dis_v < 0) dis_v = 0 - dis_v;
        
        int dis_t = dis_v + dis_h;
        
        bool near_diff_area =  false;
        
        for (int i = 0; i < DIR_INAREA; i++){
                
                int joined_cell = game->_join[cell_idx]->getJoinDir(i);
                
                if (joined_cell > 0){
                        
                        int join_area_id = game->_cel[joined_cell];
                        if (join_area_id != game->_cel[cell_idx]){
                                near_diff_area = true;
                                _join[join_area_id] = true;
                        }
                }
        }
        
        if (near_diff_area){
                dis_t += 4;
        }
        
        if (dis_t < _len_min){
                
                _len_min = dis_t;
                
                _cpos = vertical * XMAX + horizen;
        }
} 

void AreaData::initAreaLine(int cell, int dir, DiceGame* game){
        
        int cell_num = 0;
        int cur_dir = dir;
        int cur_cell = cell;
        int cur_area = game->_cel[cell];
        
        _line_cel[cell_num] = cur_cell;
        _line_dir[cell_num] = cur_dir;
        ++cell_num;
        
        for (int i = 0; i < MAX_LINE_INAREA; i++){
                
                ++cur_dir;
                if (cur_dir >= DIR_INAREA){
                        cur_dir = 0;
                }
                
                int joined_cell = game->_join[cur_cell]->getJoinDir(cur_dir);
                
                if (joined_cell >= 0 &&
                    game->_cel[joined_cell] == cur_area){
                        
                        cur_cell = joined_cell;
                        cur_dir -= 2;
                        if (cur_dir < 0){
                                cur_dir += 6;
                        }
                }
                
                _line_cel[cell_num] = cur_cell;
                _line_dir[cell_num] = cur_dir;
                ++cell_num; 
                
                if (cur_cell == cell && cur_dir == dir){
                        return;
                }
        }
}

void AreaData::drawBorder(){
        if (_size == 0 || _arm < 0)
                return;
        
        int cell = _line_cel[0];
        int dir  = _line_dir[0];
        Vec2 points[MAX_LINE_INAREA] = {{0,0}};
        
        int point_size = 0;
        
        for (int i = 1; i < MAX_LINE_INAREA; i++){
                Vec2 start = ScreenCoordinate::getInstance()->getCellPos(cell,dir);
                points[point_size++] = start;
                
                cell = _line_cel[i];
                dir  = _line_dir[i];

                if (cell == _line_cel[0] && dir == _line_dir[0]){
                        break;
                }
        }
        
        _drawNode->drawPoly(points, point_size, true, border_color);
}


void AreaData::drawPolyGon(int owner){
        
        if (_size == 0 || _arm < 0)
                return;
        
        Color4F fillColor;
        if (-1 == owner){
                fillColor = selected_color;
        }else{
                fillColor = _areaLineColor[_arm];
        }
        
        for (std::set<int>::iterator it = _cell_idxs.begin(); it != _cell_idxs.end(); ++it){
                
                Vec2 points[DIR_INAREA] = {{0,0}};
                
                for (int i = 0; i < DIR_INAREA; i++){
                        points[i] = ScreenCoordinate::getInstance()->getCellPos(*it, i);
                }
                
                _drawNode->drawPolygon(points, 6, fillColor, 0.0f, Color4F(0.0f,0.f,0.f,0.0f));
        }
}
 

Sprite* AreaData::createSprite(){
        std::ostringstream s;
        s <<"maps/"<< _dice <<".png";
        auto sprite = Sprite::create(s.str());
        Vec2 pos = ScreenCoordinate::getInstance()->getAreaCenterPos(_cpos);
        
        
        sprite->setPosition(pos);
        return sprite;
}


void AreaData::drawSupply(TMXTiledMap* map){
        //TODO::play Animation.
        
        std::string filename = "particles/ExplodingRing.plist";
        _emitter = ParticleSystemQuad::create(filename);
        _emitter->retain();
        map->addChild(_emitter, 10);
        Vec2 pos = ScreenCoordinate::getInstance()->getAreaCenterPos(_cpos);
        _emitter->setPosition(pos);
}

void AreaData::updatePawn(TMXTiledMap* map){
        Sprite* sprite = (Sprite*)map->getChildByTag(AREA_TAG_ID_INMAP(_areaId));
        sprite->removeFromParent();
        
        sprite = this->createSprite();
        map->addChild(sprite,AREA_SPRITE_ZORDER, AREA_TAG_ID_INMAP(_areaId));

}
