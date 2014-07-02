//
//  SplashScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#include "SplashScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace CocosDenshion;

Scene* SplashScene::scene()
{
    // 'scene' is an autorelease object
    auto *scene = Scene::create();
    
    // 'layer' is an autorelease object
    SplashScene *layer = SplashScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_BG));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_SPLASH));
    
    LayerColor* bgLayer = LayerColor::create(Color4B(255, 255, 255, 255), AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT);
    this->addChild(bgLayer);
    
    Sprite* ezibyteLogo = Sprite::createWithSpriteFrameName("ezibyte_logo");
    this->addChild(ezibyteLogo);
    PlaceNode(ezibyteLogo, 0.5, 0.5);

    
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void SplashScene::onEnterTransitionDidFinish()
{
    CCLOG("Enter Transition Finished.");
    
    DelayTime* waitTime  = DelayTime::create(kSPALSH_WAIT_TIME);
    CallFunc*  callFunct = CallFunc::create(this, callfunc_selector(SplashScene::loadHomeScene));
    Sequence* actionSeq  = Sequence::create(waitTime, callFunct, NULL);
    
    this->runAction(actionSeq);
    
}

void SplashScene::loadHomeScene()
{
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(PLIST_FILE(kRESOURCE_SPLASH));
    Scene *pScene = HomeScene::scene();
    ReplaceToRight(pScene);
}
