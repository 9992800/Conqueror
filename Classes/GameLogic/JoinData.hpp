//
//  JoinData.hpp
//  DiceWars
//
//  Created by Wansheng Li on 16/10/12.
//
//

#ifndef JoinData_hpp
#define JoinData_hpp
#include "cocos2d.h"
#include "AppMacros.hpp"
USING_NS_CC;

class JoinData: public Ref{
public:
        CREATE_FUNC(JoinData);
        CREATE_FUNC2(JoinData, JoinData*, obj);
        JoinData();
        JoinData(JoinData* obj);
        ~JoinData();
        bool init();
        void initdir(int cellIdx);
        
        inline int getJoinDir(int dirIdx){ return _dir[dirIdx];}
private:
        int next_cel(int cellIdx, int dirIdx);
private:
        std::vector<int> _dir;
};
#endif /* JoinData_hpp */
