#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "SplashScene.h"
#include "EziSocialObject.h"


USING_NS_CC;

float AppDelegate::SCREEN_WIDTH         = 640;
float AppDelegate::SCREEN_HEIGHT        = 960;
float AppDelegate::SCREEN_SCALE_FACTOR  = 1.0f;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    
    AppDelegate::SCREEN_SCALE_FACTOR = director->getVisibleSize().width / 960.0f;
    AppDelegate::SCREEN_WIDTH  = director->getWinSizeInPixels().width;
    AppDelegate::SCREEN_HEIGHT = director->getWinSize().height;
    
    
    std::vector<std::string> searchPath;
    
    if (AppDelegate::SCREEN_HEIGHT > 768)
    {
        searchPath.push_back("HDR");
        CCLOG("Using HDR Directory");
        //pDirector->setContentScaleFactor(MAX(AppDelegate::SCREEN_WIDTH/2048.0f, AppDelegate::SCREEN_HEIGHT/1536.0f));
    }
    else if (AppDelegate::SCREEN_HEIGHT > 320)
    {
        searchPath.push_back("HD");
        //pDirector->setContentScaleFactor(MIN(AppDelegate::SCREEN_WIDTH/1024.0f, AppDelegate::SCREEN_HEIGHT/768.0f));
        CCLOG("Using HD Directory with scaleFactor = %f", director->getContentScaleFactor());
        //CCEGLView::sharedOpenGLView()->setDesignResolutionSize(AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT, kResolutionNoBorder);
    }
    else
    {
        searchPath.push_back("SD");
        CCLOG("Using SD Directory");
        //pDirector->setContentScaleFactor(AppDelegate::SCREEN_HEIGHT/320.0f);
    }
    
    FileUtils::getInstance()->setSearchPaths(searchPath);
    
    // create a scene. it's an autorelease object
    auto *pScene = SplashScene::scene();
    //auto *pScene = HelloWorld::createScene();
    
    // run
    director->runWithScene(pScene);
    
    EziSocialObject::sharedObject()->setAutoCheckIncomingRequestsOnAppLaunch(false);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
