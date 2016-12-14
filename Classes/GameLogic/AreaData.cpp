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
#include "GameData.hpp"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "APPConstants.hpp" 

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
        this->_cell_idxs= std::set<int>(obj->_cell_idxs);
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



void AreaData::calcLenAndPos(int vertical, int horizen, int cell_idx, GameData* data){
        
        int dis_h = _cx - horizen;
        if (dis_h < 0) dis_h = 0 - dis_h;
        
        int dis_v = _cy - vertical;
        if (dis_v < 0) dis_v = 0 - dis_v;
        
        int dis_t = dis_v + dis_h;
        
        bool near_diff_area =  false;
        
        for (int i = 0; i < DIR_INAREA; i++){
                
                int joined_cell = data->_join[cell_idx]->getJoinDir(i);
                
                if (joined_cell > 0){
                        
                        int join_area_id = data->_cel[joined_cell];
                        if (join_area_id != data->_cel[cell_idx]){
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

void AreaData::initAreaLine(int cell, int dir, GameData* data){
        
        int cell_num = 0;
        int cur_dir = dir;
        int cur_cell = cell;
        int cur_area = data->_cel[cell];
        
        _line_cel[cell_num] = cur_cell;
        _line_dir[cell_num] = cur_dir;
        ++cell_num;
        
        for (int i = 0; i < MAX_LINE_INAREA; i++){
                
                ++cur_dir;
                if (cur_dir >= DIR_INAREA){
                        cur_dir = 0;
                }
                
                int joined_cell = data->_join[cur_cell]->getJoinDir(cur_dir);
                
                if (joined_cell >= 0 &&
                    data->_cel[joined_cell] == cur_area){
                        
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

void AreaData::changeOwner(int newOwner){
        //TODO::playanimations
        if (_arm == newOwner){
                return;
        }
        _arm = newOwner;
        this->drawAsUnselected();
}

void AreaData::drawBorder(){
        if (_size == 0 || _arm < 0)
                return;
        
        int cell = _line_cel[0];
        int dir  = _line_dir[0];
        Vec2 points[MAX_LINE_INAREA] = {{0,0}};
        
        int point_size = 0;
        
        for (int i = 1; i < MAX_LINE_INAREA; i++){
                Vec2 start = ScreenCoordinate::getInstance()->getCellPos(cell, dir);
                points[point_size++] = start;
                
                cell = _line_cel[i];
                dir  = _line_dir[i];

                if (cell == _line_cel[0] && dir == _line_dir[0]){
                        break;
                }
        }
        
        _drawNode->drawPoly(points, point_size, true, border_color);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        _drawNode->setLineWidth(3.f);
#else
        _drawNode->setLineWidth(5.f);
#endif
}


void AreaData::drawAsSelected(){
        //TODO:: add animation as Selected
        this->drawPolyGon(-1);
}
void AreaData::drawAsUnselected(){
        //TODO:: remove animations;
        _drawNode->clear();
        drawBorder();
        drawPolyGon(_arm);
}


void AreaData::drawPolyGon(int owner){
        
        if (_size == 0 || _arm < 0)
                return;
        
        Color4F fillColor;
        if (-1 == owner){
                fillColor = selected_color;
        }else{
                fillColor = AreaBackGroundColors[owner];
        }
        
        for (std::set<int>::iterator it = _cell_idxs.begin(); it != _cell_idxs.end(); ++it){
                
                Vec2 points[DIR_INAREA] = {{0,0}};
                
                for (int i = 0; i < DIR_INAREA; i++){
                        points[i] = ScreenCoordinate::getInstance()->getCellPos(*it, i);
                }
                
                _drawNode->drawPolygon(points, 6, fillColor, 0.5f, Color4F(0.0f,0.f,0.f,.5f));
        }
}
 

Sprite* AreaData::createSprite(){
        Vec2 pos = ScreenCoordinate::getInstance()->getAreaCenterPos(_cpos);
        
        std::string charactr_name = _dice > 4 ? CHARACTER_NAME_2[_arm] : CHARACTER_NAME[_arm];
        auto character = Sprite::create(charactr_name);
        auto ch_size = character->getContentSize();
        
        auto frameCache = SpriteFrameCache::getInstance();       
        std::string s = StringUtils::format("zssl%04d.png", _dice);
        auto frame = frameCache->getSpriteFrameByName(s);
        
        auto numbser = Sprite::create();
        numbser->setSpriteFrame(frame);
        character->addChild(numbser);
        numbser->setPosition(Vec2(ch_size.width / 2, ch_size.height / 4));
        
        character->setPosition(pos);
        return character;
}


void AreaData::drawSupply(Node* back){
        //TODO::play Animation.
        
        std::string filename = "particles/ExplodingRing.plist";
        _emitter = ParticleSystemQuad::create(filename);
        _emitter->retain();
        back->addChild(_emitter, 10);
        Vec2 pos = ScreenCoordinate::getInstance()->getAreaCenterPos(_cpos);
        _emitter->setPosition(pos);
}

void AreaData::updatePawn(Node* back){
        Sprite* sprite = (Sprite*)back->getChildByTag(AREA_TAG_ID_INMAP(_areaId));
        sprite->removeFromParent();
        
        sprite = this->createSprite();
        back->addChild(sprite, AREA_SPRITE_ZORDER, AREA_TAG_ID_INMAP(_areaId));
}

void AreaData::playOccupaiedAnimation(CallFunc* cb, Node* back){
        auto cache = AnimationCache::getInstance();
        
        Sprite* sprite = (Sprite*)back->getChildByTag(AREA_TAG_ID_INMAP(_areaId));
        
        auto character_hit = cache->getAnimation(ANIM_NAME_DEFEATED_SHOW[_dice > 4 ? 1 : 0][_arm]);
        character_hit->setRestoreOriginalFrame(true);
        
        auto fire = cache->getAnimation("finght_occupay");
        fire->setRestoreOriginalFrame(true);
        
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("XX0001.png");
        auto fire_spr = Sprite::create();
        fire_spr->setSpriteFrame(frame);
        fire_spr->setScale(0.5f);
        fire_spr->setPosition(Vec2(sprite->getContentSize() / 2));
        sprite->addChild(fire_spr);
        
        fire_spr->runAction(Animate::create(fire));
        Sequence* s = Sequence::create(Animate::create(character_hit), cb, NULL);
        sprite->runAction(s);
}


std::string AreaData::serializeData(){
        
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        
        
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("_size",       _size,  allocator);
        object.AddMember("_cpos",       _cpos,  allocator);
        object.AddMember("_arm",        _arm,   allocator);
        object.AddMember("_dice",       _dice,  allocator);
        object.AddMember("_left",       _left,  allocator);
        object.AddMember("_right",      _right, allocator);
        object.AddMember("_top",        _top,   allocator);
        object.AddMember("_bottom",     _bottom, allocator);
        object.AddMember("_cx",         _cx,    allocator);
        object.AddMember("_cy",         _cy,    allocator);
        object.AddMember("_len_min",    _len_min, allocator);
        object.AddMember("_areaId",     _areaId, allocator);
        
        document.AddMember("basic", object, allocator);
        
        
        rapidjson::Value line_cel(rapidjson::kArrayType);
        for (int i = 0; i < this->_line_cel.size(); i++){
                line_cel.PushBack(this->_line_cel[i], allocator); 
        }
        document.AddMember("_line_cel", line_cel, allocator);
        
        rapidjson::Value line_dir(rapidjson::kArrayType);
        for (int i = 0; i < this->_line_dir.size(); i++){
                line_dir.PushBack(this->_line_dir[i], allocator);
        }
        document.AddMember("_line_dir", line_dir, allocator);
        

        rapidjson::Value cell_idxs(rapidjson::kArrayType);
        for (std::set<int>::iterator it = this->_cell_idxs.begin();
             it != this->_cell_idxs.end(); it++){
                cell_idxs.PushBack(*it, allocator);
        }
        document.AddMember("_cell_idxs", cell_idxs, allocator);
        
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        
        return buffer.GetString();
}
