#include "AppDelegate.hpp"
#include "SplashScene.hpp"
#include "AppMacros.hpp"
#include "MapBasicInfo.hpp" 
#include "audio/include/SimpleAudioEngine.h"
#include "Sdkbox/Sdkbox.h" 
using namespace CocosDenshion;
AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}
void AppDelegate::initGLContextAttrs()
{
        GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

        GLView::setGLContextAttrs(glContextAttrs);
}


bool AppDelegate::applicationDidFinishLaunching() {
        
        auto director = Director::getInstance();
        auto glview = director->getOpenGLView();
        if(!glview) {
                glview = GLViewImpl::create("Conqueror");
                director->setOpenGLView(glview);
        }
 
#if COCOS2D_DEBUG >= 1
        director->setDisplayStats(true);
#endif
        director->setAnimationInterval(1.0f / 60);
     
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
        auto frameSize = glview->getFrameSize();
        
        std::vector<std::string> searchPath;
        if (frameSize.height > smallResource.size.height)
        {
                MapResolustion::initConfig(MapResolustion::BIG_SCREEN_RESOLUTION);
                searchPath.push_back(largeResource.directory);
                director->setContentScaleFactor(largeResource.size.width/designResolutionSize.width);
        } 
        else
        {
                MapResolustion::initConfig(MapResolustion::SMALL_SCREEN_RESOLUTION);
                searchPath.push_back(smallResource.directory);
                director->setContentScaleFactor(smallResource.size.width/designResolutionSize.width);
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sdkbox::init("dca2d3a93158765c0d2bf71f2cd35cbd", "d931613d4786cd34");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        sdkbox::init("36496fe1ef29addc1d4cffadef1230ab", "e9d28c96a0d91f80", "googleplay");
        sdkbox::init("a21a531fe0b8c82b5900125aa3333696", "ecec9255e641a32e", "playphone");
#endif

        FileUtils::getInstance()->setSearchPaths(searchPath);
        
        auto scene = Splash::createScene();
        director->runWithScene(scene);

        return true;
}

void AppDelegate::applicationDidEnterBackground() {
        
        Director::getInstance()->stopAnimation();
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic(); 
}


void AppDelegate::applicationWillEnterForeground() {
        Director::getInstance()->startAnimation();
        bool switch_sound = UserDefault::getInstance()->getBoolForKey(SOUND_MUSIC_TOTAL_KEY, true);
        if (switch_sound)
                SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
