#include "AppDelegate.hpp"
#include "SplashScene.hpp"
#include "AppMacros.hpp"
#include "MapBasicInfo.hpp" 
#include "audio/include/SimpleAudioEngine.h"
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

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
        return 0; //flag for packages manager
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
        searchPath.push_back("common_anim");
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

        FileUtils::getInstance()->setSearchPaths(searchPath);
        
        register_all_packages();
        
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
