//
//  FeedAPIScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "FeedAPIScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

Scene* FeedAPIScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    FeedAPIScene *layer = FeedAPIScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool FeedAPIScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->buildScene();
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void FeedAPIScene::onEnterTransitionDidFinish()
{
    CCLOG("[FeedAPIScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void FeedAPIScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that can actually publish on user wall without notifying him? Just try not so often to avoid getting your app blocked by user.");

    
    SpriteBatchNode* batchNode = SpriteBatchNode::create(PNG_FILE(kRESOURCE_FEED_API));
    this->addChild(batchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_FEED_API));
    buildHeader(-1, "Feed APIs", "Fetch the details of user from Facebook");
    
    MenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_post_message_nrl",         "btn_post_message_prd",      0, 0);
    menuItem->setCallback(CC_CALLBACK_1(FeedAPIScene::postMessageOnUserWall, this));
    //menuItem->setTarget(this, menu_selector(FeedAPIScene::postMessageOnUserWall));
    
    
    menuItem = this->buildAndPlaceTile("btn_auto_post_message_nrl",    "btn_auto_post_message_prd", 1, 1);
    menuItem->setCallback(CC_CALLBACK_1(FeedAPIScene::autoPostMessageOnUserWall, this));
    //menuItem->setTarget(this, menu_selector(FeedAPIScene::autoPostMessageOnUserWall));
    
    menuItem = this->buildAndPlaceTile("btn_post_photo_nrl",           "btn_post_photo_prd",        0, 2);
    menuItem->setCallback(CC_CALLBACK_1(FeedAPIScene::postPhotoInUserAlbum, this));
    //menuItem->setTarget(this, menu_selector(FeedAPIScene::postPhotoInUserAlbum));

    
}

void FeedAPIScene::postMessageOnUserWall(Ref *sender)
{
    EziSocialObject::sharedObject()->postMessageOnWall("EziSocial Plugin by EziByte",
                                                       "Have you tried it?",
                                                       "Try EziSocial Plugin developed by EziByte team to harness the power of Facebook APIs in your games",
                                                       "Try EziSocial Plugin developed by EziByte team to harness the power of Facebook APIs in your games",
                                                       "https://1.gravatar.com/avatar/5c85a0424f7948506baed85755bc0f8d",
                                                       "http://ezibyte.com");
}

void FeedAPIScene::autoPostMessageOnUserWall(Ref *sender)
{
    EziSocialObject::sharedObject()->autoPostMessageOnWall("EziSocial Plugin by EziByte",
                                                           "Have you tried it?",
                                                           "Using Facebook in games got easier :)",
                                                           "Try EziSocial Plugin developed by EziByte team to harness the power of Facebook APIs in your games",
                                                           "https://1.gravatar.com/avatar/5c85a0424f7948506baed85755bc0f8d",
                                                           "http://ezibyte.com");
}

void FeedAPIScene::postPhotoInUserAlbum(Ref *sender)
{
    RenderTexture *render = RenderTexture::create(AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT);
    render->setPosition(Point(AppDelegate::SCREEN_WIDTH / 2, AppDelegate::SCREEN_HEIGHT / 2));
    render->begin();
    Director::getInstance()->getRunningScene()->visit();
    render->end();
    render->saveToFile("ScreenShot.jpg", Image::Format::JPG);
    
    std::string filePathName = CCFileUtils::getInstance()->getWritablePath().append("ScreenShot.jpg");
    
    
    EziSocialObject::sharedObject()->postPhoto(filePathName.c_str(), "This is photo test message with special chars: ПРОВЕРКА");
    //EziSocialObject::sharedObject()->postPhoto("ball.png", "This is photo test message");
}

void FeedAPIScene::fbMessageCallback(int responseCode, const char* responseMessage)
{
    MessageBox(responseMessage, "Message Response");
}

void FeedAPIScene::fbPostPhotoCallback(int responseCode, const char* responseMessage)
{
    MessageBox(responseMessage, "Photo post response");
}
