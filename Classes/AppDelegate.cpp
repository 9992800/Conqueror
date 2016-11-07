#include "AppDelegate.hpp"
#include "SplashScene.hpp"
#include "AppMacros.hpp"
#include "MapBasicInfo.hpp"
#include "PluginIAP/PluginIAP.h"

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
        
        sdkbox::IAP::init();
        
        auto director = Director::getInstance();
        auto glview = director->getOpenGLView();
        if(!glview) {
                glview = GLViewImpl::create("Conqueror");
                director->setOpenGLView(glview);
        }
 
        director->setDisplayStats(true);
        director->setAnimationInterval(1.0f / 60);
     
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
        auto frameSize = glview->getFrameSize();
        
        std::vector<std::string> searchPath;
        if (frameSize.height > mediumResource.size.height)
        {
                MapResolustion::initConfig(MapResolustion::BIG_SCREEN_RESOLUTION);
                searchPath.push_back(largeResource.directory);
                director->setContentScaleFactor(largeResource.size.height/designResolutionSize.height);
        }
        else if (frameSize.height > smallResource.size.height)
        {
                MapResolustion::initConfig(MapResolustion::MEDIUM_SCREEN_RESOLUTION);
                searchPath.push_back(mediumResource.directory);
                director->setContentScaleFactor(mediumResource.size.height/designResolutionSize.height);
        }
        else
        {
                MapResolustion::initConfig(MapResolustion::SMALL_SCREEN_RESOLUTION);
                searchPath.push_back(smallResource.directory);
                director->setContentScaleFactor(smallResource.size.height/designResolutionSize.height);
        }

        FileUtils::getInstance()->setSearchPaths(searchPath);
        
        auto scene = Splash::createScene();
        director->runWithScene(scene);

        return true;
}

void AppDelegate::applicationDidEnterBackground() {
        
        // if you use SimpleAudioEngine, it must be paused
        // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}


void AppDelegate::applicationWillEnterForeground() {
        Director::getInstance()->startAnimation();

        // if you use SimpleAudioEngine, it must resume here
        // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
