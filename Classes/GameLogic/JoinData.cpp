//
//  JoinData.cpp
//  DiceWars
//
//  Created by Wansheng Li on 16/10/12.
//
//

#include "JoinData.hpp"
#include "GameConfig.hpp"

JoinData::JoinData(){
        _dir = std::vector<int>(DIR_INAREA);
}

JoinData::~JoinData(){
        _dir.clear();
}

void JoinData::initdir(int cellIdx){
        for (int i = 0; i < DIR_INAREA; i++){
                _dir[i] = next_cel(cellIdx, i);
        }
}


int JoinData::next_cel(int opos, int dir){
        int column = opos % XMAX;
        int rows =  opos / XMAX;

        int odd_rows = rows % 2;
        
        int iFlag = 0, jFlag = 0;
        switch (dir) {
                case CELL_DIR_RIGHT_TOP:
                        iFlag = odd_rows ? 1: 0;
                        jFlag = -1;
                        break;
                case CELL_DIR_RIGHT_MID:
                        iFlag = 1;
                        break;
                case CELL_DIR_RIGHT_BOT:
                        iFlag = odd_rows ? 1 : 0;
                        jFlag = 1;
                        break;
                case CELL_DIR_LEFT_TOP:
                        iFlag = odd_rows ? 0 : -1;
                        jFlag = 1;
                        break;
                case CELL_DIR_LEFT_MID:
                        iFlag = -1;
                        break;
                case CELL_DIR_LEFT_BOT:
                        iFlag = odd_rows ? 0 : -1;
                        jFlag = -1;
                        break;
        }
        
        column += iFlag;
        rows +=jFlag;
        
        if (column < 0 || column >= XMAX
            || rows < 0 ||rows >= YMAX){
                return -1;
        }
        
        return rows * XMAX + column;
}
