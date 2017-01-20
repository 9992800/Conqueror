//
//  APPConstants.cpp
//  Conqueror
//
//  Created by wsli on 16/12/13.
//
//

#include "APPConstants.hpp"
#include "AppMacros.hpp"

int READY_DISTANCE_POS = 242;

Vec2 invader_pos[] = {{242,250}, {203,265}, {182,236}, {156,280}, {136,250}, {116,221},{91, 265}, {70,236}, {30,250}};

Vec2 keeper_pos[]  = {{-242,250}, {-203,265}, {-182,236}, {-156,280}, {-136,250}, {-116,221},{-91, 265}, {-70,236}, {-30,250}};


std::string DICE_PIC_NAME_STR[8][6] = {
        {"yellow0001.png", "yellow0002.png", "yellow0003.png", "yellow0004.png", "yellow0005.png", "yellow0006.png"},
        {"red0001.png", "red0002.png", "red0003.png", "red0004.png", "red0005.png", "red0006.png"},
        {"Orange0001.png", "Orange0002.png", "Orange0003.png", "Orange0004.png", "Orange0005.png", "Orange0006.png"},
        {"green0001.png", "green0002.png", "green0003.png", "green0004.png", "green0005.png", "green0006.png"},
        {"Violet0001.png", "Violet0002.png", "Violet0003.png", "Violet0004.png", "Violet0005.png", "Violet0006.png"},
        {"Pink0001.png", "Pink0002.png", "Pink0003.png", "Pink0004.png", "Pink0005.png", "Pink0006.png"},
        {"Bluegreen0001.png", "Bluegreen0002.png", "Bluegreen0003.png", "Bluegreen0004.png", "Bluegreen0005.png", "Bluegreen0006.png"},
        {"Blue0001.png", "Blue0002.png", "Blue0003.png", "Blue0004.png", "Blue0005.png", "Blue0006.png"}};



std::string ANIM_NAME_FIGHT_RUN[]        = {"zhanshi_run", "xunshoushi_run", "gongjianshou_run", "huoqiangshou_run",
        "maonv_run", "changqiangshou_run", "monv_run", "renzhe_run"};

std::string ANIM_NAME_FIGHT_STAND[]      = {"zhanshi_sd", "xunshoushi_sd", "gongjianshou_sd", "huoqiangshou_sd",
        "maonv_sd", "changqiangshou_sd", "monv_sd", "renzhe_sd"};



std::string CHARACTER_NAME[] = {"zhanshi_pos.png", "xunshoushi_pos.png", "gongjianshou_pos.png", "huoqiangshou_pos.png",
        "maonv_pos.png", "changqiangshou_pos.png","monv_pos.png", "renzhe_pos.png"};

std::string CHARACTER_NAME_2[] = {"zhanshi_pos2.png", "xunshoushi_pos2.png", "gongjianshou_pos2.png", "huoqiangshou_pos2.png",
        "maonv_pos2.png", "changqiangshou_pos2.png","monv_pos2.png", "renzhe_pos2.png"};

std::string ANIM_NAME_DEFEATED_SHOW[2][8] = {
        {"zhangshi_hit1", "xunshoushi_hit1","gongjianshou_hit1",
                "huoqiangshou_hit1","maonv_hit1", "changqiangshou_hit1",
                "monv_hit1", "renzhe_hit1"},
        
        {"zhangshi_hit2", "xunshoushi_hit2", "gongjianshou_hit2",
                "huoqiangshou_hit2", "maonv_hit2", "changqiangshou_hit2",
                "monv_hit2", "renzhe_hit2"}
};

Color4F AreaBackGroundColors[] = {Color4F((float)200/255, (float)180/255, (float)20/255, 1),//huang
        Color4F((float)200/255, 0.f, 0.f, 1),//hong
        Color4F((float)220/255, (float)100/255, (float)0.f, 1),//chengse
        Color4F((float)140/255, (float)180/255, (float)80/255, 1),//qianlv
        Color4F((float)100/255, 0.f, (float)220/255, 1),//zise
        Color4F((float)230/255, (float)50/255, (float)230/255, 1),//fenhong
        Color4F((float)10/255, (float)90/255, (float)60/255, 1),//shenglv
        Color4F((float)0.f, (float)80/255, (float)220/255, 1)};//langse

Color4F border_color = Color4F(0.0, 0.0, 0.0, 1.0);
Color4F selected_color = Color4F(0.2, 0.0, 0.0, 0.7);
std::string  SYSTEM_FONTS = "";
ValueMap LOCALIZED_STRING_MAP;

static GolbalConfig* s_SharedBean;
int GolbalConfig::CURRENT_GAME_LEVEL = CURRENT_GAME_LEVEL_EASY;

GolbalConfig* GolbalConfig::getInstance()
{
        if (!s_SharedBean){
                s_SharedBean = new (std::nothrow) GolbalConfig();
                CCASSERT(s_SharedBean, "FATAL: Not enough memory");
                s_SharedBean->init();
        }
        
        return s_SharedBean;
}

bool GolbalConfig::init(){
        this->initBasic();
        this->initAnimation();
        this->initAchievementData();
        this->initMercenaryItemData();
        return true;
}

GolbalConfig::GolbalConfig(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        frameCache->addSpriteFramesWithFile("anim/hanshirun.plist", "anim/hanshirun.png");
        frameCache->addSpriteFramesWithFile("anim/hanshisd.plist", "anim/hanshisd.png");
        frameCache->addSpriteFramesWithFile("anim/zhanshiHIT1.plist", "anim/zhanshiHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/zhanshiHIT2.plist", "anim/zhanshiHIT2.png");
        
        frameCache->addSpriteFramesWithFile("anim/xunshoushirun.plist", "anim/xunshoushirun.png");
        frameCache->addSpriteFramesWithFile("anim/xunshoushisd.plist", "anim/xunshoushisd.png");
        frameCache->addSpriteFramesWithFile("anim/xssHIT1.plist", "anim/xssHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/xssHIT2.plist", "anim/xssHIT2.png");
        
        frameCache->addSpriteFramesWithFile("anim/gongjianshou_run.plist", "anim/gongjianshou_run.png");
        frameCache->addSpriteFramesWithFile("anim/gongjianshou_sd.plist", "anim/gongjianshou_sd.png");
        frameCache->addSpriteFramesWithFile("anim/gongjianshouHIT1.plist", "anim/gongjianshouHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/gongjianshouHIT2.plist", "anim/gongjianshouHIT2.png");
        
        frameCache->addSpriteFramesWithFile("anim/huoqiangshou_run.plist", "anim/huoqiangshou_run.png");
        frameCache->addSpriteFramesWithFile("anim/huoqiangshou_sd.plist", "anim/huoqiangshou_sd.png");
        frameCache->addSpriteFramesWithFile("anim/huoqiangshouHIT1.plist", "anim/huoqiangshouHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/huoqiangshouHIT2.plist", "anim/huoqiangshouHIT2.png");
        
        frameCache->addSpriteFramesWithFile("anim/maonv_run.plist", "anim/maonv_run.png");
        frameCache->addSpriteFramesWithFile("anim/maonv_sd.plist", "anim/maonv_sd.png");
        frameCache->addSpriteFramesWithFile("anim/maonvHIT1.plist", "anim/maonvHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/maonvHIT2.plist", "anim/maonvHIT2.png");
        
        frameCache->addSpriteFramesWithFile("anim/changqiangshou_run.plist", "anim/changqiangshou_run.png");
        frameCache->addSpriteFramesWithFile("anim/changqiangshou_sd.plist", "anim/changqiangshou_sd.png");
        frameCache->addSpriteFramesWithFile("anim/changqiangshouHIT1.plist", "anim/changqiangshouHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/changqiangshouHIT2.plist", "anim/changqiangshouHIT2.png");
        
        frameCache->addSpriteFramesWithFile("anim/monv_run.plist", "anim/monv_run.png");
        frameCache->addSpriteFramesWithFile("anim/monv_sd.plist", "anim/monv_sd.png");
        frameCache->addSpriteFramesWithFile("anim/monvHIT1.plist", "anim/monvHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/monvHIT2.plist", "anim/monvHIT2.png");
        
        frameCache->addSpriteFramesWithFile("anim/renzhe_run.plist", "anim/renzhe_run.png");
        frameCache->addSpriteFramesWithFile("anim/renzhe_sd.plist", "anim/renzhe_sd.png");
        frameCache->addSpriteFramesWithFile("anim/renzheHIT1.plist", "anim/renzheHIT1.png");
        frameCache->addSpriteFramesWithFile("anim/renzheHIT2.plist", "anim/renzheHIT2.png");


        
        frameCache->addSpriteFramesWithFile("anim/yanwu.plist", "anim/yanwu.png");
        frameCache->addSpriteFramesWithFile("anim/zssl.plist", "anim/zssl.png");
        frameCache->addSpriteFramesWithFile("anim/XX.plist", "anim/XX.png");
        
        frameCache->addSpriteFramesWithFile("anim/dice_colors.plist", "anim/dice_colors.png");
        frameCache->addSpriteFramesWithFile("anim/coins_change.plist", "anim/coins_change.png");
        frameCache->addSpriteFramesWithFile("common_anim/loding.plist", "common_anim/loding.png");
}

GolbalConfig::~GolbalConfig(){
        
        
        auto frameCache = SpriteFrameCache::getInstance();
        frameCache->removeSpriteFramesFromFile("anim/hanshirun.plist");
        frameCache->removeSpriteFramesFromFile("anim/hanshisd.plist");
        frameCache->removeSpriteFramesFromFile("anim/zhanshiHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/zhanshiHIT2.plist");
        
        frameCache->removeSpriteFramesFromFile("anim/xunshoushirun.plist");
        frameCache->removeSpriteFramesFromFile("anim/xunshoushisd.plist");
        frameCache->removeSpriteFramesFromFile("anim/xssHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/xssHIT2.plist");
        
        frameCache->removeSpriteFramesFromFile("anim/gongjianshou_run.plist");
        frameCache->removeSpriteFramesFromFile("anim/gongjianshou_sd.plist");
        frameCache->removeSpriteFramesFromFile("anim/gongjianshouHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/gongjianshouHIT2.plist");
        
        frameCache->removeSpriteFramesFromFile("anim/huoqiangshou_run.plist");
        frameCache->removeSpriteFramesFromFile("anim/huoqiangshou_sd.plist");
        frameCache->removeSpriteFramesFromFile("anim/huoqiangshouHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/huoqiangshouHIT2.plist");
        
        frameCache->removeSpriteFramesFromFile("anim/maonv_run.plist");
        frameCache->removeSpriteFramesFromFile("anim/maonv_sd.plist");
        frameCache->removeSpriteFramesFromFile("anim/maonvHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/maonvHIT2.plist");
        
        frameCache->removeSpriteFramesFromFile("anim/changqiangshou_run.plist");
        frameCache->removeSpriteFramesFromFile("anim/changqiangshou_sd.plist");
        frameCache->removeSpriteFramesFromFile("anim/changqiangshouHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/changqiangshouHIT2.plist");
        
        frameCache->removeSpriteFramesFromFile("anim/monv_run.plist");
        frameCache->removeSpriteFramesFromFile("anim/monv_sd.plist");
        frameCache->removeSpriteFramesFromFile("anim/monvHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/monvHIT2.plist");
        
        frameCache->removeSpriteFramesFromFile("anim/renzhe_run.plist");
        frameCache->removeSpriteFramesFromFile("anim/renzhe_sd.plist");
        frameCache->removeSpriteFramesFromFile("anim/renzheHIT1.plist");
        frameCache->removeSpriteFramesFromFile("anim/renzheHIT2.plist");
        
        
        frameCache->removeSpriteFramesFromFile("anim/yanwu.plist");
        frameCache->removeSpriteFramesFromFile("anim/zssl.plist");
        frameCache->removeSpriteFramesFromFile("anim/XX.plist");
        
        
        frameCache->removeSpriteFramesFromFile("anim/dice_colors.plist");
        frameCache->removeSpriteFramesFromFile("anim/coins_change.plist");
        frameCache->removeSpriteFramesFromFile("common_anim/loding.plist");
}


void GolbalConfig::initZhanShi(){
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        //----------------------------------战士-----------------------------------
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
}
void GolbalConfig::initXunShouShi(){
        //------------------------------驯兽师---------------------------------
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        
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
void GolbalConfig::initGongJianShou(){
        //-------------------------弓箭手--------------------------------------
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        for (int i = 1; i<= 12; i++){
                sprintf(str, "gjsrun01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "gongjianshou_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "gjssd01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "gongjianshou_sd");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "gjsHIT1%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "gongjianshou_hit1");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "gjsHIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "gongjianshou_hit2");
}


void GolbalConfig::initHuoQianShou(){
        //-------------------------火枪手--------------------------------------
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        for (int i = 1; i<= 12; i++){
                sprintf(str, "hqsrun01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "huoqiangshou_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "hqssd01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "huoqiangshou_sd");
        
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "hqsHIT1%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "huoqiangshou_hit1");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "hqsHIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "huoqiangshou_hit2");
}



void GolbalConfig::initMaonv(){
        //-------------------------猫女--------------------------------------
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        for (int i = 1; i<= 12; i++){
                sprintf(str, "mvrun01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "maonv_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "mvsd01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "maonv_sd");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "mvHIT1%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "maonv_hit1");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "mvHIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "maonv_hit2");
}



void GolbalConfig::initChangQiangShou(){
        //-------------------------长抢手--------------------------------------
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "casrun01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "changqiangshou_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "cassd01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "changqiangshou_sd");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "cqsHIT1%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "changqiangshou_hit1");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "cqsHIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "changqiangshou_hit2");
}


void GolbalConfig::initMonv(){
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        //魔女
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "mnrun01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "monv_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "mnsd01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "monv_sd");
        
        
        animFrames.clear();
        for (int i = 1; i<= 14; i++){
                sprintf(str, "mnHIT1%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "monv_hit1");
        
        
        animFrames.clear();
        for (int i = 1; i<= 14; i++){
                sprintf(str, "mnHIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "monv_hit2");
}

void GolbalConfig::initRenZhe(){
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        //忍着
        animFrames.clear();
        for (int i = 1; i<= 12; i++){
                sprintf(str, "rzrun01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "renzhe_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "rzsd01%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "renzhe_sd");
        
        
        animFrames.clear();
        for (int i = 1; i<= 14; i++){
                sprintf(str, "rzHIT1%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "renzhe_hit1");
        
        
        animFrames.clear();
        for (int i = 1; i<= 14; i++){
                sprintf(str, "rzHIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "renzhe_hit2");
}

void GolbalConfig::initAnimation(){
        
        this->initZhanShi();
        
        this->initXunShouShi();
        
        this->initGongJianShou();
        
        this->initHuoQianShou();
        
        this->initMaonv();
        
        this->initChangQiangShou();
        
        this->initMonv();
        
        this->initRenZhe();
        
        auto frameCache = SpriteFrameCache::getInstance();
        float frame_delay = 1.f / 24;
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        //战斗云
        animFrames.clear();
        for (int i = 1; i<= 26; i++){
                sprintf(str, "yw%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
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
        
        //---------------------------------------------------------------
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "jb%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "coins_changes");
        
        //---------------------------------------------------------------
        animFrames.clear();
        for (int i = 1; i < 12; i++){
                sprintf(str, "loding%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        
        animation = Animation::createWithSpriteFrames(animFrames, frame_delay);
        AnimationCache::getInstance()->addAnimation(animation, "common_waiting");
        
}

void GolbalConfig::initAchievementData(){
        _systemAchievementData = std::map<std::string, AchievementData>();
        
        auto cache = UserDefault::getInstance();
        
        int has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_2, REWARDS_STATUS_CLOSED);
        AchievementData data_1 = {1, ACHIEVE_DATA_KEY_FIRST_WIN_2, has_got, 5, 1,
                CHARACTOR_2_LOCK_STATE_KEY, LEVEL_3_LOCK_STATE_KEY,
                "Win 2 Players",
                "Overcome enemy and occupy all islands."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_WIN_2, data_1));
        
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_3, REWARDS_STATUS_CLOSED);
        AchievementData data_2 = {2, ACHIEVE_DATA_KEY_FIRST_WIN_3,
                has_got, 5, 1, "", LEVEL_4_LOCK_STATE_KEY,
                "Win 3 Players", "Overcome 2 enemies and occupy all islands."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_WIN_3, data_2));
        
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_4, REWARDS_STATUS_CLOSED);
        AchievementData data_3 = {3, ACHIEVE_DATA_KEY_FIRST_WIN_4,
                has_got, 0, 2, "", LEVEL_5_LOCK_STATE_KEY,
                "Win 4 Players", "Overcome 3 enemies and occupy all islands."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_WIN_4, data_3));
        
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_5, REWARDS_STATUS_CLOSED);
        AchievementData data_4 = {4, ACHIEVE_DATA_KEY_FIRST_WIN_5,
                has_got, 5, 2, "", LEVEL_6_LOCK_STATE_KEY,
                "Win 5 Players", "Overcome 4 enemies and occupy all islands."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_WIN_5, data_4));;
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_6, REWARDS_STATUS_CLOSED);
        AchievementData data_5 = {5, ACHIEVE_DATA_KEY_FIRST_WIN_6,
                has_got, 10, 2, "", LEVEL_7_LOCK_STATE_KEY,
                "Win 6 Players", "Overcome 5 enemies and occupy all islands."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_WIN_6, data_5));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_7, REWARDS_STATUS_CLOSED);
        AchievementData data_6 = {6, ACHIEVE_DATA_KEY_FIRST_WIN_7,
                has_got, 0, 3, "", LEVEL_8_LOCK_STATE_KEY,
                "Win 7 Players", "Overcome 6 enemies and occupy all islands."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_WIN_7, data_6));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_8, REWARDS_STATUS_CLOSED);
        AchievementData data_7 = {7, ACHIEVE_DATA_KEY_FIRST_WIN_8,
                has_got, 5, 3, "", "",
                "Win 8 Players", "Overcome 7 enemies and occupy all islands."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_WIN_8, data_7));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_WIN_3TIMES, REWARDS_STATUS_CLOSED);
        AchievementData data_8 = {8, ACHIEVE_DATA_KEY_WIN_3TIMES,
                has_got, 9, 0, "", "",
                "3 wins", "Continuous win 3 times."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_WIN_3TIMES, data_8));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_SHARE, REWARDS_STATUS_CLOSED);
        AchievementData data_9 = {9, ACHIEVE_DATA_KEY_FIRST_SHARE,
                has_got, 19,  0, "", "",
                "Share Game", "Share this game to your FB friends."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_SHARE, data_9));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_USE_MERCENARY, REWARDS_STATUS_CLOSED);
        AchievementData data_10 = {10, ACHIEVE_DATA_KEY_FIRST_USE_MERCENARY,
                has_got, 0, 2, "", "",
                "Use Mercenary", "Use mercenary to make you powerfull."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_USE_MERCENARY, data_10));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_CLOSE_ANIM, REWARDS_STATUS_CLOSED);
        AchievementData data_11 = {11, ACHIEVE_DATA_KEY_FIRST_CLOSE_ANIM,
                has_got, 0, 1, "", "",
                "Close Animation", "Close the fighting animation in the battle when you're familar with the game."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_CLOSE_ANIM, data_11));
        
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_RETRY, REWARDS_STATUS_CLOSED);
        AchievementData data_12 = {12, ACHIEVE_DATA_KEY_FIRST_RETRY,
                has_got, 5, 1, "", "",
                "Replay", "Replay the same battle map when you lose the game."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_RETRY, data_12));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_BUY_COINS, REWARDS_STATUS_CLOSED);
        AchievementData data_13 = {13, ACHIEVE_DATA_KEY_FIRST_BUY_COINS,
                has_got, 5, 2, "", "",
                "Purchase",
                "Buy coins in the shop."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_BUY_COINS, data_13));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_CHANGE_COLOR, REWARDS_STATUS_CLOSED);
        AchievementData data_14 = {14, ACHIEVE_DATA_KEY_FIRST_CHANGE_COLOR,
                has_got, 9, 0, "", "",
                "Change Color", "Change your charactor in the battle."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_CHANGE_COLOR, data_14));
        
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_CHANGE_PLAYER, REWARDS_STATUS_CLOSED);
        AchievementData data_15 = {15, ACHIEVE_DATA_KEY_FIRST_CHANGE_PLAYER,
                has_got, 0, 1, "", "",
                "Change Charactor", "Change your charactor in the battle."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_CHANGE_PLAYER, data_15));
        
        has_got = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_ENGAGE, REWARDS_STATUS_CLOSED);
        AchievementData data_16 = {16, ACHIEVE_DATA_KEY_FIRST_ENGAGE,
                has_got, 0, 1, "", "",
                "Engage", "Engage mercenary using coins."
        };
        _systemAchievementData.insert(std::pair<std::string, AchievementData>(ACHIEVE_DATA_KEY_FIRST_ENGAGE, data_16));
}

void GolbalConfig::initMercenaryItemData(){
        _mercenaryItemPriceData = std::vector<MercenaryItem>();
        
        MercenaryItem item_1 = {1, 10, "shopping/mercenary_tips_1.png",
                "shopping/NO_1.png", "shopping/NO_10.png"};
        _mercenaryItemPriceData.push_back(item_1);
        
        MercenaryItem item_5 = {5, 50, "shopping/mercenary_tips_5.png",
                "shopping/NO_5.png", "shopping/NO_50.png"};
        _mercenaryItemPriceData.push_back(item_5);
        
        MercenaryItem item_10 = {10, 100, "shopping/mercenary_tips_10.png",
                "shopping/NO_10.png", "shopping/NO_100.png"};
        _mercenaryItemPriceData.push_back(item_10);
        
        MercenaryItem item_30 = {30, 300, "shopping/mercenary_tips_30.png",
                "shopping/NO_30.png", "shopping/NO_300.png"};
        _mercenaryItemPriceData.push_back(item_30);
        
        MercenaryItem item_60 = {60, 600, "shopping/mercenary_tips_60.png",
                "shopping/NO_60.png", "shopping/NO_600.png"};
        _mercenaryItemPriceData.push_back(item_60);
        
        MercenaryItem item_90 = {90, 900, "shopping/mercenary_tips_90.png",
                "shopping/NO_90.png", "shopping/NO_900.png"};
        _mercenaryItemPriceData.push_back(item_90);
        
        MercenaryItem item_100 = {100, 1000, "shopping/mercenary_tips_100.png",
                "shopping/NO_100.png", "shopping/NO_1000.png"};
        _mercenaryItemPriceData.push_back(item_100);
}

void GolbalConfig::initBasic(){
        auto cache = UserDefault::getInstance();
        cache->setIntegerForKey(LEVEL_2_LOCK_STATE_KEY, true);
        cache->setIntegerForKey(CHARACTOR_1_LOCK_STATE_KEY, true);
        
        Size frame_size = Director::getInstance()->getOpenGLView()->getFrameSize();
        for (int i = 0; i < MAX_PLAYER; i++){
                invader_pos[i].x = invader_pos[i].x / designResolutionSize.width * frame_size.width;
                invader_pos[i].y = invader_pos[i].y / designResolutionSize.height * frame_size.height;
        
                
                keeper_pos[i].x = keeper_pos[i].x / designResolutionSize.width * frame_size.width;
                keeper_pos[i].y = keeper_pos[i].y / designResolutionSize.height * frame_size.height;
        }
        
        CURRENT_GAME_LEVEL = cache->getIntegerForKey(GAME_DIFFICULT_LEVEL,
                                                     CURRENT_GAME_LEVEL_EASY); 
        cache->flush();
        
        const char* currentLanguageCode = Application::getInstance()->getCurrentLanguageCode();
        std::string file_path = StringUtils::format("fonts/Localized_%s.plist", currentLanguageCode); 
        
        std::string path = FileUtils::getInstance()->fullPathForFilename(file_path);
        LOCALIZED_STRING_MAP = FileUtils::getInstance()->getValueMapFromFile(path);
        LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
        switch (currentLanguageType){
                default:
                case LanguageType::ENGLISH:{
                        SYSTEM_FONTS = "fonts/arial.ttf";
                }
                        break;
                case LanguageType::CHINESE:{
                        SYSTEM_FONTS = "";
                }
                        break;
        }
}

std::vector<AchievementData> GolbalConfig::getAchievementData(){
        
        struct {
                bool operator()(AchievementData a, AchievementData b)
                {
                        return a._index < b._index;
                }
        }customLess;
        
        this->initAchievementData();
        
        std::vector<AchievementData> v;
        int counter = 0;
        for (std::map<std::string, AchievementData>::iterator it = this->_systemAchievementData.begin();
             it != this->_systemAchievementData.end(); ++it){
                v.push_back(it->second);
                if (it->second.bonus_status == REWARDS_STATUS_OPEN){
                        counter++;
                }
        }
        
        std::sort(v.begin(), v.end(), customLess);
        
        UserDefault::getInstance()->setIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, counter);
        return v;
}
