//
//  ScreenCoordinate.cpp
//  DiceWars
//
//  Created by wsli on 16/10/20.
//
//

#include "ScreenCoordinate.hpp"
#include "AppMacros.hpp"
#include "MapBasicInfo.hpp"

static ScreenCoordinate* s_SharedScreen;

ScreenCoordinate* ScreenCoordinate::getInstance(){
        if (!s_SharedScreen)
        {
                s_SharedScreen = new (std::nothrow) ScreenCoordinate();
                CCASSERT(s_SharedScreen, "FATAL: Not enough memory");
                s_SharedScreen->init();
        }
        
        return s_SharedScreen;
}

ScreenCoordinate::ScreenCoordinate(void){
}

bool ScreenCoordinate::init(){
        _cpos_x = std::vector<float>(CEL_MAX);
        _cpos_y = std::vector<float>(CEL_MAX);
        
        _axis_x = std::vector<float>(DIR_INAREA + 1);
        _axis_y = std::vector<float>(DIR_INAREA + 1);
        return true;
}

void ScreenCoordinate::configScreen(Size mapSize){
        
        float w = mapSize.width / XMAX;
        float h = mapSize.height / YMAX;
        _cellSize = Size((int)w, h);
        _mapSize = mapSize;
        
        int count = 0;
        for (int i = 0; i < YMAX; i++){
                
                for (int j = 0; j < XMAX; j++){
                        
                        _cpos_x[count] = j * _cellSize.width + MAP_GAM_WIDTH / 2;
                        if (i % 2 == 1){
                                _cpos_x[count] += _cellSize.width / 2;
                        }
                        
                        _cpos_y[count] = mapSize.height - i * _cellSize.height + (MAP_GAM_HEIGHT / 2);
                        ++count;
                }
        }
        
        
        //TODO:: why height is 3 it must h/3
        _axis_x[0] = _cellSize.width / 2;
        _axis_x[1] = _cellSize.width;
        _axis_x[2] = _cellSize.width;
        _axis_x[3] = _cellSize.width / 2;
        _axis_x[4] = 0.f;
        _axis_x[5] = 0.f;
        _axis_x[6] = _cellSize.width / 2;
        
        double s = 9 * mapSize.height / (double) (YMAX * 30);
        _axis_y[0] = s;
        _axis_y[1] = 0;
        _axis_y[2] = s - h;
        _axis_y[3] = -h;
        _axis_y[4] = s - h;
        _axis_y[5] = 0;
        _axis_y[6] = s;
}

Vec2 ScreenCoordinate::getCellPos(int cell, int dir){
        return Vec2(_cpos_x[cell] + _axis_x[dir], _cpos_y[cell] + _axis_y[dir]);
}


int ScreenCoordinate::getSelectedCell(Vec2 curreTouch){
        if (curreTouch.y >= _mapSize.height || curreTouch.x >= _mapSize.width){
                return -1;
        }
        float h = _mapSize.height - curreTouch.y;
        int row = (int)(h / _cellSize.height);
        
        if (row % 2){
                curreTouch.x -= _cellSize.width / 2;
        }
        
        int col = (int)(curreTouch.x / _cellSize.width);
        
        int cell_idx = row * XMAX + col;
        if (cell_idx < 0){
                cell_idx = 0;
        }
        if (cell_idx >= CEL_MAX){
                cell_idx = CEL_MAX - 1;
        }
        return cell_idx;
}

Vec2 ScreenCoordinate::getAreaCenterPos(int pos){
        return Vec2(_cpos_x[pos] + _cellSize.width / 2, _cpos_y[pos] - _cellSize.height / 2);
}
