//
//  APPConstants.cpp
//  Conqueror
//
//  Created by wsli on 16/12/13.
//
//

#include "APPConstants.hpp"

int READY_DISTANCE_POS = 242;

Vec2 invader_pos[] = {{242,250}, {203,265}, {182,236}, {156,280}, {136,250}, {116,221},{91, 265}, {70,236}, {30,250}};

Vec2 keeper_pos[]  = {{-242,250}, {-203,265}, {-182,236}, {-156,280}, {-136,250}, {-116,221},{-91, 265}, {-70,236}, {-30,250}};


std::string DICE_PIC_NAME_STR[8][6] = {
        {"Orange0001.png", "Orange0002.png", "Orange0003.png", "Orange0004.png", "Orange0005.png", "Orange0006.png"},
        {"red0001.png", "red0002.png", "red0003.png", "red0004.png", "red0005.png", "red0006.png"},
        {"yellow0001.png", "yellow0002.png", "yellow0003.png", "yellow0004.png", "yellow0005.png", "yellow0006.png"},
        {"green0001.png", "green0002.png", "green0003.png", "green0004.png", "green0005.png", "green0006.png"},
        {"Violet0001.png", "Violet0002.png", "Violet0003.png", "Violet0004.png", "Violet0005.png", "Violet0006.png"},
        {"Pink0001.png", "Pink0002.png", "Pink0003.png", "Pink0004.png", "Pink0005.png", "Pink0006.png"},
        {"Bluegreen0001.png", "Bluegreen0002.png", "Bluegreen0003.png", "Bluegreen0004.png", "Bluegreen0005.png", "Bluegreen0006.png"},
        {"Blue0001.png", "Blue0002.png", "Blue0003.png", "Blue0004.png", "Blue0005.png", "Blue0006.png"}};



std::string ANIM_NAME_FIGHT_RUN[]        = {"zhanshi_run", "xunshoushi_run", "xunshoushi_run", "xunshoushi_run",
        "xunshoushi_run", "xunshoushi_run", "xunshoushi_run", "xunshoushi_run"};

std::string ANIM_NAME_FIGHT_STAND[]      = {"zhanshi_sd", "xunshoushi_sd", "xunshoushi_sd", "xunshoushi_sd",
        "xunshoushi_sd", "xunshoushi_sd", "xunshoushi_sd", "xunshoushi_sd"};



std::string CHARACTER_NAME[] = {"zhanshi_pos.png", "xunshoushi_pos.png", "xunshoushi_pos.png", "xunshoushi_pos.png",
        "xunshoushi_pos.png", "xunshoushi_pos.png","xunshoushi_pos.png", "xunshoushi_pos.png"};

std::string CHARACTER_NAME_2[] = {"zhanshi_pos2.png", "xunshoushi_pos2.png", "xunshoushi_pos2.png", "xunshoushi_pos2.png",
        "xunshoushi_pos2.png", "xunshoushi_pos2.png","xunshoushi_pos2.png", "xunshoushi_pos2.png"};

std::string ANIM_NAME_DEFEATED_SHOW[2][8] = {
        {"zhangshi_hit1", "xunshoushi_hit1","xunshoushi_hit1",
                "xunshoushi_hit1","xunshoushi_hit1", "xunshoushi_hit1",
                "xunshoushi_hit1", "xunshoushi_hit1"},
        
        {"zhangshi_hit2", "xunshoushi_hit2", "xunshoushi_hit2",
                "xunshoushi_hit2", "xunshoushi_hit2", "xunshoushi_hit2",
                "xunshoushi_hit2", "xunshoushi_hit2"}
};

Color4F AreaBackGroundColors[] = {Color4F((float)0xCE/0xff, (float)0x84/0xff, (float)0x39/0xff, 1),
        Color4F((float)0xD7/0xff, (float)0x45/0xff, (float)0x13/0xff, 1),
        Color4F((float)0xE0/0xff, (float)0xE9/0xff, (float)0x83/0xff, 1),
        Color4F((float)0x6F/0xff, (float)0xCF/0xff, (float)0x00/0xff, 1),
        Color4F((float)0x84/0xff, (float)0x6F/0xff, (float)0xE0/0xff, 1),
        Color4F((float)0xE0/0xff, (float)0x00/0xff, (float)0xBC/0xff, 1),
        Color4F((float)0x44/0xff, (float)0x6F/0xff, (float)0x00/0xff, 1),
        Color4F((float)0x6F/0xff, (float)0xA0/0xff, (float)0xE0/0xff, 1)};

Color4F border_color = Color4F(0.0, 0.0, 0.0, 1.0);
Color4F selected_color = Color4F(0.2, 0.0, 0.0, 0.7);



static GolbalAnimations* s_SharedBean;

GolbalAnimations* GolbalAnimations::getInstance()
{
        if (!s_SharedBean){
                s_SharedBean = new (std::nothrow) GolbalAnimations();
                CCASSERT(s_SharedBean, "FATAL: Not enough memory");
                s_SharedBean->init();
        }
        
        return s_SharedBean;
}

bool GolbalAnimations::init(){
        return true;
}

GolbalAnimations::GolbalAnimations(){        
        
        auto frameCache = SpriteFrameCache::getInstance();
        frameCache->addSpriteFramesWithFile("anim/hanshirun.plist", "anim/hanshirun.png");
        frameCache->addSpriteFramesWithFile("anim/hanshisd.plist", "anim/hanshisd.png");
        frameCache->addSpriteFramesWithFile("anim/xunshoushirun.plist", "anim/xunshoushirun.png");
        frameCache->addSpriteFramesWithFile("anim/xunshoushisd.plist", "anim/xunshoushisd.png");
        frameCache->addSpriteFramesWithFile("anim/yanwu.plist", "anim/yanwu.png");
        frameCache->addSpriteFramesWithFile("anim/zssl.plist", "anim/zssl.png");
        frameCache->addSpriteFramesWithFile("anim/XX.plist", "anim/XX.png");
        frameCache->addSpriteFramesWithFile("anim/xssHIT1.plist", "anim/xssHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/xssHIT2.plist", "anim/xssHIT2.png");
        frameCache->addSpriteFramesWithFile("anim/zhanshiHIT1.plist", "anim/zhanshiHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/zhanshiHIT2.plist", "anim/zhanshiHIT2.png");
        frameCache->addSpriteFramesWithFile("anim/dice_colors.plist", "anim/dice_colors.png");
}

GolbalAnimations::~GolbalAnimations(){
        
        
        auto frameCache = SpriteFrameCache::getInstance();
        frameCache->removeSpriteFramesFromFile("anim/hanshirun.plist");
        frameCache->removeSpriteFramesFromFile("anim/hanshisd.plist");
        frameCache->removeSpriteFramesFromFile("anim/xunshoushirun.plist");
        frameCache->removeSpriteFramesFromFile("anim/xunshoushisd.plist");
        frameCache->removeSpriteFramesFromFile("anim/yanwu.plist");
        frameCache->removeSpriteFramesFromFile("anim/zssl.plist");
        frameCache->removeSpriteFramesFromFile("anim/XX.plist");
        frameCache->removeSpriteFramesFromFile("anim/xssHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/xssHIT2.plist");
        frameCache->removeSpriteFramesFromFile("anim/zhanshiHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/zhanshiHIT2.plist");
        frameCache->removeSpriteFramesFromFile("anim/dice_colors.plist");
}

void GolbalAnimations::initAnimation(){
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        //战士
        Vector<SpriteFrame*> animFrames(12); 
        char str[100] = {0};
        for(int i = 1; i <= 12; i++){
                sprintf(str, "renwurun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "zhanshi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "renwu_sd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "zhanshi_sd");
        
        
        //驯兽师
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        
        //骑士
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        
        //弓箭手
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        //炮手
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        
        //兽人
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        //魔女
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        
        //盗贼
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        
        //战斗云
        animFrames.clear();
        for (int i = 1; i<= 26; i++){
                sprintf(str, "yw%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "finght_cloud");
        
        
        //地盘显示战斗结果动画
        animFrames.clear();
        for (int i = 1; i<= 9; i++){
                sprintf(str, "XX%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "finght_occupay");
        
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "zhanshi_HIT%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "zhangshi_hit1");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "zhanshi_HIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "zhangshi_hit2");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "xunshoushi_HIT%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_hit1");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "xunshoushi_HIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_hit2");
}
