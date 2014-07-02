//
//  FriendsScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "FriendsScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "FriendListScene.h"

using namespace cocos2d;


Scene* FriendsScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    FriendsScene *layer = FriendsScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool FriendsScene::init()
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
void FriendsScene::onEnterTransitionDidFinish()
{
    CCLOG("[FriendsScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void FriendsScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that with EziSocial you fetch the friends details including id name, score and photos?");

    
    SpriteBatchNode* batchNode = SpriteBatchNode::create(PNG_FILE(kRESOURCE_FRIENDS));
    this->addChild(batchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_FRIENDS));
    
    buildHeader(-1, "Friends", "Get the list of facebook friends");
    
    // Build the tiles.
    MenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_all_friends_nrl",       "btn_all_friends_prd",      2, 0);
    menuItem->setCallback(CC_CALLBACK_1(FriendsScene::showAllFriends, this));
    
    menuItem = this->buildAndPlaceTile("btn_who_nrl",               "btn_who_prd",              1, 1);
    menuItem->setCallback(CC_CALLBACK_1(FriendsScene::showOnlyPlayingFriends, this));
    
    menuItem = this->buildAndPlaceTile("btn_invite_missing_nrl",    "btn_invite_missing_prd",   0, 2);
    menuItem->setCallback(CC_CALLBACK_1(FriendsScene::showNonPlayingFriends, this));
    
    
    menuItem = this->buildAndPlaceTile("btn_special_friends_nrl",   "btn_special_friends_prd",  2, 2);
    menuItem->setCallback(CC_CALLBACK_1(FriendsScene::showOnlyFriendsPlayingWithMyKindOfDevice, this));
    

}

void FriendsScene::showAllFriends(Ref* sender)
{
    Scene* pScene = FriendListScene::scene(ALL_FRIENDS);
    ReplaceToRight(pScene);
}

void FriendsScene::showNonPlayingFriends(Ref* sender)
{
    Scene* pScene = FriendListScene::scene(NON_PLAYING_ONLY);
    ReplaceToRight(pScene);
}

void FriendsScene::showOnlyPlayingFriends(Ref* sender)
{
    Scene* pScene = FriendListScene::scene(INSTALLED_ONLY);
    ReplaceToRight(pScene);
}

void FriendsScene::showOnlyFriendsPlayingWithMyKindOfDevice(Ref* sender)
{
    Scene* pScene = FriendListScene::scene(MY_DEVICE_ONLY);
    ReplaceToRight(pScene);
}
