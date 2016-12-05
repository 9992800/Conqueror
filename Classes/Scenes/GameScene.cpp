//
//  DiceGameScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//
#include "GameData.hpp"
#include "MapCreator.hpp"
#include "GameScene.hpp"
#include "ScreenCoordinate.hpp"
#include "SimpleAudioEngine.h"
#include "PopUpOkCancelDialog.hpp"
#include "PopUpOkDialog.hpp"
#include "ui/CocosGUI.h" 


static int READY_DISTANCE_POS = 242;

Vec2 invader_fight_pos[] = {{242,250}, {203,265}, {182,236}, {156,280}, {136,250}, {116,221},{91, 265}, {70,236}, {30,250}};
Vec2 keeper_fight_pos[]  = {{-242,250}, {-203,265}, {-182,236}, {-156,280}, {-136,250}, {-116,221},{-91, 265}, {-70,236}, {-30,250}};
static std::string DICE_PIC_NAME_STR[8][6] = {
        {"Orange0001.png", "Orange0002.png", "Orange0003.png", "Orange0004.png", "Orange0005.png", "Orange0006.png"},
        {"red0001.png", "red0002.png", "red0003.png", "red0004.png", "red0005.png", "red0006.png"},
        {"yellow0001.png", "yellow0002.png", "yellow0003.png", "yellow0004.png", "yellow0005.png", "yellow0006.png"},
        {"Bluegreen0001.png", "Bluegreen0002.png", "Bluegreen0003.png", "Bluegreen0004.png", "Bluegreen0005.png", "Bluegreen0006.png"},
        {"green0001.png", "green0002.png", "green0003.png", "green0004.png", "green0005.png", "green0006.png"},
        {"Blue0001.png", "Blue0002.png", "Blue0003.png", "Blue0004.png", "Blue0005.png", "Blue0006.png"},
        {"Pink0001.png", "Pink0002.png", "Pink0003.png", "Pink0004.png", "Pink0005.png", "Pink0006.png"},
        {"Violet0001.png", "Violet0002.png", "Violet0003.png", "Violet0004.png", "Violet0005.png", "Violet0006.png"}};

enum{
        ZORDER_BACK_GROUND = 0,
        ZORDER_MAP_GROUND,
        ZORDER_CRTL_LAYERS,
        ZORDER_ANIM_LAYER,
        ZORDER_DICE_LAYER,
        ZORDER_DIALOG_LAYER
};
enum{
        key_map_tag             = 1,
        key_ctrl_layer_tag,
        key_anim_layer_tag,
        key_dice_layer_tag,
        key_dialog_layer_tag
};

static std::string ANIM_NAME_FIGHT_RUN[]        = {"zhanshi_run", "xunshoushi_run", "xunshoushi_run", "xunshoushi_run",
        "xunshoushi_run", "xunshoushi_run", "xunshoushi_run", "xunshoushi_run"};

static std::string ANIM_NAME_FIGHT_STAND[]      = {"zhanshi_sd", "xunshoushi_sd", "xunshoushi_sd", "xunshoushi_sd",
        "xunshoushi_sd", "xunshoushi_sd", "xunshoushi_sd", "xunshoushi_sd"};


#pragma mark - constructor
Scene* GameScene::createScene(int gameLevel)
{
        auto scene = Scene::create();
        auto layer = GameScene::create(gameLevel);
        
        scene->addChild(layer); 
        return scene;
}

GameScene::~GameScene(){
        _theGameLogic->release();
        
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


bool GameScene::init()
{
        if (!Layer::init()){
                return false;
        }
        
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
        
        this->initMapLayer();
        
        this->initControlLayer();
        
        this->initAnimationLayer();
        
        return true;
}

#pragma mark - initilization
void GameScene::initMapLayer(){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        _theGameLogic = DiceGame::create();
        _theGameLogic->retain();
        
        auto data = _theGameLogic->initGameData(_playerNumber);
        
        auto map_size = visibleSize * 1.2f;
        
        auto back_layer = LayerColor::create(TILE_COLOR_BACKGRUND,
                                             map_size.width + MAP_GAM_WIDTH ,
                                             map_size.height + MAP_GAM_HEIGHT);
      
        
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        data->reshDataByBackGrnd(back_layer);
        back_layer->setPosition(-MAP_GAM_WIDTH / 2, -MAP_GAM_HEIGHT / 2);
        this->addChild(back_layer, ZORDER_MAP_GROUND, key_map_tag); 
}

void GameScene::initControlLayer(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        
        _animationIsOn = UserDefault::getInstance()->getBoolForKey(ANIMATION_SWITCH_KEY, true);
        int game_speed = _animationIsOn ?  3 : UserDefault::getInstance()->getIntegerForKey(GAME_SPEED_KEY, 1);
        
        Director::getInstance()->getScheduler()->setTimeScale(game_speed);
        
        
        _controlLayer = Layer::create();
        _endTurnMenuItem = MenuItemImage::create("NextButton.png", "NextButton.png",
                                             CC_CALLBACK_1(GameScene::menuEndTurn, this));
        _endTurnMenuItem->setPosition(Vec2(origin.x + visibleSize.width - _endTurnMenuItem->getContentSize().width,
                                       origin.y + _endTurnMenuItem->getContentSize().height));
        _endTurnMenuItem->setVisible(false);
        
        _startPlayMenuItem = MenuItemImage::create("start.png", "start.png",
                                             CC_CALLBACK_1(GameScene::menuStartGame, this));
        _startPlayMenuItem->setPosition(Vec2(origin.x + visibleSize.width - _startPlayMenuItem->getContentSize().width,
                                       origin.y +_startPlayMenuItem->getContentSize().height));
        
        auto return_back = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                             CC_CALLBACK_1(GameScene::menuExit, this));
        return_back->setPosition(Vec2(origin.x + return_back->getContentSize().width + 10,
                                       origin.y + visibleSize.height - return_back->getContentSize().height - 10));
        
        auto colse_anim = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                              CC_CALLBACK_1(GameScene::menuAnimSwitch, this));
        colse_anim->setPosition(Vec2(visibleSize.width - colse_anim->getContentSize().width - 10,
                                      visibleSize.height - colse_anim->getContentSize().height - 10));
        
        auto label = Label::createWithSystemFont("关闭动画", "", 32);
        label->setTextColor(Color4B::RED);
        colse_anim->addChild(label, 1, 111);
        if (!_animationIsOn){
                label->setString("打开动画");
        }
        auto menu = Menu::create(_endTurnMenuItem, _startPlayMenuItem, return_back, colse_anim, NULL);
        menu->setPosition(Vec2::ZERO);
        _controlLayer->addChild(menu);
        
        this->addChild(_controlLayer, ZORDER_CRTL_LAYERS, key_ctrl_layer_tag);
        
        Director::getInstance()->setDepthTest(true);
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void GameScene::loadZhanshi(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        Vector<SpriteFrame*> animFrames_back(12);
        char str[100] = {0};
        for(int i = 1; i <= 12; i++){
                sprintf(str, "renwurun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "zhanshi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "renwu_sd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "zhanshi_sd");
        
        auto frame = frameCache->getSpriteFrameByName("renwurun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto zhanshi = Sprite::create();
                zhanshi->setSpriteFrame(frame);
                _allFightingCharacters[PLAYER_ROLE_TYPE_ZHANSHI][i] = zhanshi;
                zhanshi->setVisible(false);
                _animationLayer->addChild(zhanshi);
                zhanshi->setVisible(false);
        }
}

void GameScene::loadXunShouShi(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
                 
        auto frame = frameCache->getSpriteFrameByName("xssrun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto xunshoushi = Sprite::create();
                xunshoushi->setSpriteFrame(frame);
                _allFightingCharacters[PLAYER_ROLE_TYPE_XUNSHOUSHI][i] = xunshoushi;
                _animationLayer->addChild(xunshoushi);
                xunshoushi->setVisible(false);
        }
}

void GameScene::loadQiShi(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        auto frame = frameCache->getSpriteFrameByName("xssrun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto xunshoushi = Sprite::create();
                xunshoushi->setSpriteFrame(frame);
                _allFightingCharacters[PLAYER_ROLE_TYPE_QISHI][i] = xunshoushi;
                _animationLayer->addChild(xunshoushi);
                xunshoushi->setVisible(false);
        }
}

void GameScene::loadGongJianShou(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        auto frame = frameCache->getSpriteFrameByName("xssrun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto xunshoushi = Sprite::create();
                xunshoushi->setSpriteFrame(frame);
                _allFightingCharacters[PLAYER_ROLE_TYPE_GONGJIANSHOU][i] = xunshoushi;
                _animationLayer->addChild(xunshoushi);
                xunshoushi->setVisible(false);
        }
}

void GameScene::loadPaoShou(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        auto frame = frameCache->getSpriteFrameByName("xssrun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto xunshoushi = Sprite::create();
                xunshoushi->setSpriteFrame(frame);
                _allFightingCharacters[PLAYER_ROLE_TYPE_PAOSHOU][i] = xunshoushi;
                _animationLayer->addChild(xunshoushi);
                xunshoushi->setVisible(false);
        }
}

void GameScene::loadShouRen(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        auto frame = frameCache->getSpriteFrameByName("xssrun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto xunshoushi = Sprite::create();
                xunshoushi->setSpriteFrame(frame);
                _allFightingCharacters[PLAYER_ROLE_TYPE_SHOUREN][i] = xunshoushi;
                _animationLayer->addChild(xunshoushi);
                xunshoushi->setVisible(false);
        }
}

void GameScene::loadMoNv(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        auto frame = frameCache->getSpriteFrameByName("xssrun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto xunshoushi = Sprite::create();
                xunshoushi->setSpriteFrame(frame);
                _allFightingCharacters[PLAYER_ROLE_TYPE_MONV][i] = xunshoushi;
                _animationLayer->addChild(xunshoushi);
                xunshoushi->setVisible(false);
        }
}

void GameScene::loadDaoZei(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(12);
        char str[100] = {0};
        
        for (int i = 1; i<= 12; i++){
                sprintf(str, "xssrun%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_run");
        
        
        animFrames.clear();
        for (int i = 1; i<= 20; i++){
                sprintf(str, "xsssd%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_sd");
        
        auto frame = frameCache->getSpriteFrameByName("xssrun0001.png");
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                auto xunshoushi = Sprite::create();
                xunshoushi->setSpriteFrame(frame);
                _allFightingCharacters[PLAYER_ROLE_TYPE_DAOZEI][i] = xunshoushi;
                _animationLayer->addChild(xunshoushi);
                xunshoushi->setVisible(false);
        }
}

void GameScene::loadFightCloud(){
        
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(26);
        char str[100] = {0};
        
        for (int i = 1; i<= 26; i++){
                sprintf(str, "yw%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "finght_cloud");
        
        auto frame = frameCache->getSpriteFrameByName("yw0001.png");
        auto xingyun = Sprite::create();
        xingyun->setSpriteFrame(frame);
        _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0] = xingyun;
        xingyun->setVisible(false);
        _animationLayer->addChild(xingyun);
}

void GameScene::loadFightResult(){
        auto frameCache = SpriteFrameCache::getInstance();
        
        Vector<SpriteFrame*> animFrames(9);
        char str[100] = {0};
        
        for (int i = 1; i<= 9; i++){
                sprintf(str, "XX%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "finght_occupay");
        
//        auto frame = frameCache->getSpriteFrameByName("XX0001.png");
//        auto fire = Sprite::create();
//        fire->setSpriteFrame(frame);
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "zhanshi_HIT%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "zhangshi_hit1");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "zhanshi_HIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "zhangshi_hit2");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "xunshoushi_HIT%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_hit1");
        
        animFrames.clear();
        for (int i = 1; i <= 14; i++){
                sprintf(str, "xunshoushi_HIT2%04d.png", i);
                auto frame = frameCache->getSpriteFrameByName(str);
                animFrames.pushBack(frame);
        }
        animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        AnimationCache::getInstance()->addAnimation(animation, "xunshoushi_hit2");
}

void GameScene::loadDiceResultLayer(){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto anim_back_size = _animationLayer->getContentSize();
        _diceResultLayer = Layer::create();
        auto dice_size = Size(anim_back_size.width, anim_back_size.height / 3);
        _diceResultLayer->setContentSize(dice_size);
        _diceResultLayer->setPosition(Vec2((visibleSize - dice_size) / 2));
        this->addChild(_diceResultLayer, ZORDER_DICE_LAYER, key_dice_layer_tag);
        _diceResultLayer->setVisible(false);
        
        float scale_factor = Director::getInstance()->getContentScaleFactor();
        for (int i = 0; i < MAX_PLAYER; i++){
                invader_fight_pos[i] *= (1.f / scale_factor);
                keeper_fight_pos[i] *= (1.f / scale_factor);
                keeper_fight_pos[i].x = anim_back_size.width + keeper_fight_pos[i].x;
        }
        
        READY_DISTANCE_POS *= (1.f / scale_factor);

}

void GameScene::initAnimationLayer(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        _animationLayer = Sprite::create("zhandou_beijing.png");
        _animationLayer->setScale(0.1f, 0.1f);
        
        
        _animationLayer->setPosition(Vec2(visibleSize / 2));
        _animationLayer->setVisible(false);
        this->addChild(_animationLayer, ZORDER_ANIM_LAYER, key_anim_layer_tag);
        
        this->loadDiceResultLayer();
        
        this->loadZhanshi();
        
        this->loadXunShouShi();
        
        this->loadZhanshi2();
        
        this->loadZhanshi3();
        
        this->loadZhanshi4();
        
        this->loadZhanshi5();
        
        this->loadZhanshi6();
        
        this->loadZhanshi7();
        
        this->loadFightCloud();
        
        this->loadFightResult();
        
}
#pragma mark - touch and menu event

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
        
        auto touch = touches[0];
        auto diff = touch->getDelta();

        if (diff.x >= 0.01f || diff.y >= 0.01
            || diff.x <= -0.01f ||diff.y <= -0.01f){
                _isMoved = true;
        }
        
        diff.x = 0;
        auto map = this->getChildByTag(key_map_tag);
        auto currentPos = map->getPosition();
        auto origin = Director::getInstance()->getVisibleOrigin(); 
        
        if ( origin.y < (currentPos.y + diff.y)){
                diff.y = 0;
        }
        
        if ((currentPos.y + diff.y) < -MAP_GAM_HEIGHT){
                diff.y = 0;
        }
        
        map->setPosition(currentPos + diff);
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *event){        
        
        if (_isMoved || _isPalyingAnim
            ||_gameStatus != GAME_STATUS_INUSERTURN) {
                _isMoved = false;
                return;
        }
        
        auto touch = touches[0];
        auto position = touch->getLocation();
        auto map = this->getChildByTag(key_map_tag);
        Vec2 pos_in_map = map->convertToNodeSpace(position);
        
        if (pos_in_map.x < 0 || pos_in_map.y < 0){
                return;
        }
        int cell_id = ScreenCoordinate::getInstance()->getSelectedCell(pos_in_map);
        if (cell_id < 0){
                return;
        }
        
        FightResultData* result = _theGameLogic->startPlayerAttack(cell_id);
        
        if (nullptr != result){
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterPlayerBattle, this, result));
                this->playBattleAnimation(result, callback, true);
        }
}

void GameScene::tryAgain(){
        _gameStatus = GAME_STATUS_INIT;
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto layer = this->getChildByTag(key_map_tag);
        layer->removeFromParentAndCleanup(true);
        
        auto data = _theGameLogic->resetInitData();
        
        auto back_layer = LayerColor::create(TILE_COLOR_BACKGRUND, visibleSize.width + 10, visibleSize.height + 20);
        ScreenCoordinate::getInstance()->configScreen(visibleSize);
        data->reshDataByBackGrnd(back_layer);
        this->addChild(back_layer, ZORDER_MAP_GROUND, key_map_tag);


        _startPlayMenuItem->setVisible(true);
        _endTurnMenuItem->setVisible(false);
}

#pragma mark - animation 

void GameScene::afterPlayerBattle(FightResultData* result){
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
        std::map<int, int> survival = _theGameLogic->cleanUpBattleField(result);
        if (survival.size() == 1){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_FINISH_WIN);
                Director::getInstance()->pause();
                BaseDialogConfig config("胜利了!",
                                        "娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香");
                PopUpOkCancelDialog *dialog = PopUpOkCancelDialog::create(config,
                                                                          CC_CALLBACK_1(GameScene::gameOver, this, 1),
                                                                          CC_CALLBACK_1(GameScene::gameOver, this, 0));
                this->addChild(dialog, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
                return;
        }
}

void GameScene::afterRobootBattle(FightResultData* result){
        _diceResultLayer->setVisible(false);
        _diceResultLayer->removeAllChildren();
        _theGameLogic->cleanUpBattleField(result);
        
        int user_tc = _theGameLogic->getUserTC();
        if (0 == user_tc){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_FINISH_LOSE);
                Director::getInstance()->pause();
                BaseDialogConfig config("失败，需要重试吗？",
                                        "娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香");
                PopUpOkCancelDialog *dialog = PopUpOkCancelDialog::create(config,
                                                                          CC_CALLBACK_1(GameScene::gameOver, this, 1),
                                                                          CC_CALLBACK_1(GameScene::gameOver, this, 0));
                this->addChild(dialog, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
                return;
        }
        
        this->gameAction();
}


void GameScene::afterPlayerSupply(){
        _theGameLogic->next_player();
        this->gameAction();
}
void GameScene::afterRobootSupply(){
        _theGameLogic->next_player();
        this->gameAction();
}

void GameScene::gameAction(){
        FightResultData* res_data = _theGameLogic->startRobootAttack();
        if (nullptr == res_data || res_data->_result == ATTACK_RES_NONE){
                
                _gameStatus = GAME_STATUS_INUSERTURN;
                _endTurnMenuItem->setVisible(true);
                return;
        }
        
        if (ATTACK_RES_WIN == res_data->_result || ATTACK_RES_DEFEATED == res_data->_result){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootBattle, this, res_data));
                this->playBattleAnimation(res_data, callback, false);
                
        }else if(ATTACK_RES_GOTSUPPLY == res_data->_result){
                
                CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootSupply, this));
                this->playSupplyAnimation(callback);
        }
}

void GameScene::afterFightFinished(FightResultData* resut_data, CallFunc* cb){
        for (int i = 0; i < MAX_DICE_PER_AREA; i++){
                _allFightingCharacters[resut_data->_fromPlayer][i]->setVisible(false);
                _allFightingCharacters[resut_data->_toPlayer][i]->setVisible(false);
        }
        _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0]->setVisible(false);
        
        auto occupay_cc = CallFunc::create(std::bind(&DiceGame::occupayAnimation, _theGameLogic, resut_data, cb));
        
        auto hide = ScaleTo::create(1.0f, .1f);
        _animationLayer->runAction(Sequence::create(hide,
                CallFunc::create( [&](){
                _animationLayer->setVisible(false);
                _isPalyingAnim = false;
        }), occupay_cc, NULL));
}


void GameScene::WinnerBack(FightResultData* res_data, CallFunc* cb){
        
        this->ShowResultData(res_data);
        
        if (ATTACK_RES_WIN == res_data->_result){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_WIN);
        }else if (ATTACK_RES_DEFEATED == res_data->_result){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_DEFEAT);
        }
        
        
        _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0]->setVisible(false);
        
        Size back_size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        auto when_back_home = CallFunc::create(std::bind(&GameScene::afterFightFinished, this,res_data, cb));
        
        int player_uid = ATTACK_RES_WIN == res_data->_result ? res_data->_fromPlayer : res_data->_toPlayer;
        auto run_back_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid]);
        run_back_anim->setRestoreOriginalFrame(true);
        auto run_back_act = Animate::create(run_back_anim);
        
        auto back_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid]);
        back_wait->setRestoreOriginalFrame(true);
        
        
        if (ATTACK_RES_WIN == res_data->_result){
                
                int keeper_uid = res_data->_toPlayer;
                for (int i = 0; i < res_data->_to.size(); i++){
                        auto keeper = _allFightingCharacters[keeper_uid][i];
                        keeper->setVisible(false);
                }
                
                auto moveby = MoveBy::create(2, Vec2(READY_DISTANCE_POS - back_size.width / 2,0));
                auto run_back = Spawn::create(run_back_act, FlipX::create(true), moveby, NULL);
                
                auto fight_back = Animate::create(back_wait);
                
                auto move = MoveBy::create(2, Vec2(-READY_DISTANCE_POS,0));
                Spawn* back_home = Spawn::create(run_back_act->clone(), move, NULL);
                
                Sequence* invade_back = Sequence::create(run_back, fight_back,  back_home, NULL);
                
                for (int i = 0; i < res_data->_from.size(); i++){
                        auto invader = _allFightingCharacters[player_uid][i];
                        
                        if (i == 0){
                                auto s_with_cc = Sequence::create(run_back, fight_back,  back_home, when_back_home, NULL);
                                invader->runAction(s_with_cc);
                        }else{
                                invader->runAction(invade_back->clone());
                        }
                }
        }else{
                
                int invader_uid = res_data->_fromPlayer;
                for (int i = 0; i < res_data->_from.size(); i++){
                        auto invader = _allFightingCharacters[invader_uid][i];
                        invader->setVisible(false);
                }
                
                auto moveby = MoveBy::create(2, Vec2(back_size.width / 2 - READY_DISTANCE_POS, 0));
                auto run_back = Spawn::create(run_back_act, FlipX::create(false), moveby, NULL);
                
                auto fight_back = Animate::create(back_wait);
                
                auto move = MoveBy::create(2, Vec2(READY_DISTANCE_POS, 0));
                Spawn* back_home = Spawn::create(run_back_act->clone(), move, NULL);
                
                Sequence* keeper_back = Sequence::create(run_back, fight_back, back_home, NULL);

                for (int i = 0; i < res_data->_to.size(); i++){
                        auto keeper = _allFightingCharacters[player_uid][i];
                        
                        if (0 == i){
                                auto s_with_cc = Sequence::create(run_back, fight_back,  back_home, when_back_home, NULL);
                                keeper->runAction(s_with_cc);
                        }else{
                                keeper->runAction(keeper_back->clone());
                        }
                }
        }
}

void GameScene::ShowResultData(FightResultData* resut_data){
        _diceResultLayer->setVisible(true);
        auto back_size = _diceResultLayer->getContentSize();
        auto frameCache = SpriteFrameCache::getInstance();
        
        
        for (int i = 0; i < resut_data->_from.size(); i++){
                int value = resut_data->_from[i];
                auto str = DICE_PIC_NAME_STR[resut_data->_fromPlayer][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                dice->setPosition(Vec2(dice_size.width * (i + 1), dice_size.height));
                _diceResultLayer->addChild(dice);
        }
        
        for (int i = 0; i < resut_data->_to.size(); i++){
                int value = resut_data->_to[i];
                auto str = DICE_PIC_NAME_STR[resut_data->_toPlayer][value - 1];
                auto frame = frameCache->getSpriteFrameByName(str);
                auto dice = Sprite::create();
                dice->setSpriteFrame(frame);
                auto dice_size = dice->getContentSize();
                dice->setPosition(Vec2(back_size.width - dice_size.width * (i + 1),  dice_size.height));
                _diceResultLayer->addChild(dice);
        }
        
        auto from_value = Label::createWithSystemFont(StringUtils::format("%d", resut_data->_fromSum), "", 78);
        from_value->setColor(Color3B::BLUE);
        from_value->setPosition(Vec2(back_size.width / 2 - 100, back_size.height - 20 ));
        _diceResultLayer->addChild(from_value);
        
        auto to_value = Label::createWithSystemFont(StringUtils::format("%d", resut_data->_toSum), "", 78);
        to_value->setColor(Color3B::BLUE);
        to_value->setPosition(Vec2(back_size.width / 2 + 100, back_size.height - 20));
        _diceResultLayer->addChild(to_value);
}

void GameScene::Fighting(FightResultData* resut_data, CallFunc* cb){
        
        auto winner_back = CallFunc::create(std::bind(&GameScene::WinnerBack, this, resut_data, cb));
        
        auto fight_cloud = _allFightingCharacters[FIGHT_ANIM_TYPE_XINYUN][0];
        fight_cloud->setVisible(true);
        
        auto size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        
        fight_cloud->setPosition(Vec2(size.width / 2, size.height * 0.4));
        fight_cloud->setScale(2.0f);
        auto anim_cloud = cache->getAnimation("finght_cloud");
        anim_cloud->setRestoreOriginalFrame(true);
        auto anim_action = Animate::create(anim_cloud);
        
        fight_cloud->runAction(Sequence::create(anim_action, winner_back, NULL));
}

void GameScene::afterShowFightBg(FightResultData* res_data, CallFunc* cb){
        Size back_size = _animationLayer->getContentSize();
        auto cache = AnimationCache::getInstance();
        
        //FIGHTER
        {
                int player_uid = res_data->_fromPlayer;
        
                auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid]);
                run_anim->setRestoreOriginalFrame(true);
                auto run_action = Animate::create(run_anim);
                auto move = MoveBy::create(2, Vec2(READY_DISTANCE_POS,0));
                Spawn* get_ready = Spawn::create(run_action, FlipX::create(false), move, NULL);
                
                
                
                auto stand_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid]);
                stand_wait->setRestoreOriginalFrame(true);
                auto fight_wait = Animate::create(stand_wait);
                
                
                auto moveby = MoveBy::create(2, Vec2(back_size.width / 2 - READY_DISTANCE_POS,0));
                auto run_to_fight = Spawn::create(run_action->clone(), moveby, NULL);
                
        
                Sequence* invade_seq = Sequence::create(get_ready, fight_wait,  run_to_fight, NULL);
                
                for (int i = 0; i < res_data->_from.size(); i++){
                        
                        auto invader = _allFightingCharacters[res_data->_fromPlayer][i];
                        invader->setVisible(true);
                        invader->setPosition(invader_fight_pos[i] - Vec2(READY_DISTANCE_POS, 0));
                        
                        if (i == 0){
                                auto fighting = CallFunc::create(std::bind(&GameScene::Fighting, this,res_data, cb));
                                auto s_with_cc = Sequence::create(get_ready, fight_wait,  run_to_fight, fighting, NULL);
                                invader->runAction(s_with_cc);
                        }else{
                                invader->runAction(invade_seq->clone());
                        }
                }
        }
        
        
        //KEEPER
        {
                int player_uid = res_data->_toPlayer;
                
                auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[player_uid]);
                run_anim->setRestoreOriginalFrame(true);
                auto run_act = Animate::create(run_anim);
                
                auto move = MoveBy::create(2, Vec2(-READY_DISTANCE_POS,0));
                Spawn* get_ready = Spawn::create(run_act, FlipX::create(true), move, NULL);
                
                
                auto stand_wait = cache->getAnimation(ANIM_NAME_FIGHT_STAND[player_uid]);
                stand_wait->setRestoreOriginalFrame(true);
                auto fight_wait = Animate::create(stand_wait);
                
                
                auto moveby = MoveBy::create(2, Vec2(READY_DISTANCE_POS - back_size.width / 2 ,0));
                auto run_to_fight = Spawn::create(run_act, moveby, NULL);
                
                Sequence* keeper_seq = Sequence::create(get_ready, fight_wait, run_to_fight, NULL);
                
                for (int i = 0; i < res_data->_to.size(); i++){
                        auto keeper = _allFightingCharacters[player_uid][i];
                        keeper->setVisible(true);
                        keeper->setPosition(keeper_fight_pos[i] + Vec2(READY_DISTANCE_POS, 0));
                        keeper->runAction(keeper_seq->clone());
                }
        }
}

void GameScene::playBattleAnimation(FightResultData* resut_data, CallFunc* callback, bool isMaunual){
        
        if (_animationIsOn && isMaunual){
                _isPalyingAnim = true;
                _animationLayer->setVisible(true);
                auto show = ScaleTo::create(.5f, 1.0f);
                callback->retain();
                resut_data->retain();
                auto cc = CallFunc::create(std::bind(&GameScene::afterShowFightBg, this, resut_data, callback));
                _animationLayer->runAction(Sequence::create(show, cc, NULL));
                
        }else{
                this->ShowResultData(resut_data);
                callback->retain();
                resut_data->retain();
                this->afterFightFinished(resut_data, callback); 
        }
}

void GameScene::playSupplyAnimation(CallFunc* callback){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_SUPPLY);
        _theGameLogic->starSupplyDice(callback);
}

#pragma mark - menu callback actions
void GameScene::menuEndTurn(Ref* pSender){
        if (_isPalyingAnim){
                return;
        }
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
        
        ((MenuItemImage*)pSender)->setVisible(false);
        _theGameLogic->clearManulAction();
        _gameStatus = GAME_STATUS_AIRUNNING;
        CallFunc* callback = CallFunc::create(std::bind(&GameScene::afterRobootSupply, this));
        this->playSupplyAnimation(callback);
}

void GameScene::menuStartGame(Ref* pSender){
        _theGameLogic->initHistoryRecord();
        ((MenuItemImage*)pSender)->setVisible(false);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
        _gameStatus = GAME_STATUS_AIRUNNING;
        this->gameAction();
}

void GameScene::menuExit(Ref* pSender){
        
        Director::getInstance()->pause();
        BaseDialogConfig config("确认要退出吗？", "娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香");
        PopUpOkCancelDialog *dialog = PopUpOkCancelDialog::create(config,
                                                                  CC_CALLBACK_1(GameScene::gameExit, this, 1),
                                                                  CC_CALLBACK_1(GameScene::gameExit, this, 0));
        dialog->setButtonTittle("退出","取消");
        this->addChild(dialog, ZORDER_DIALOG_LAYER, key_dialog_layer_tag);
}

void GameScene::gameExit(Ref* btn, int result){
        if (result == 1){
                Director::getInstance()->popScene();
        }else{
                this->removeChildByTag(key_dialog_layer_tag);
        }
        
        Director::getInstance()->resume();        
}

void GameScene::gameOver(Ref* btn, int result){
        if (result == 0){
                _theGameLogic->finishHistoryRecord();
                Director::getInstance()->popScene();
        }else{
                this->removeChildByTag(key_dialog_layer_tag);
                this->tryAgain();
        }
        
        Director::getInstance()->resume();
}

void GameScene::menuAnimSwitch(Ref* btn){
        auto label = (Label*)((Node*)btn)->getChildByTag(111);
        if (_animationIsOn){
                _animationIsOn = false;
                label->setString("打开动画");
                Director::getInstance()->getScheduler()->setTimeScale(2);
                _animationLayer->setVisible(false);
        }else{
                label->setString("关闭动画");
                _animationIsOn = true;
                Director::getInstance()->getScheduler()->setTimeScale(3);
        }
        
        UserDefault::getInstance()->setBoolForKey(ANIMATION_SWITCH_KEY, _animationIsOn);
        UserDefault::getInstance()->flush();
}
