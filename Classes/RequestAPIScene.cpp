//
//  RequestAPIScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "RequestAPIScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "EziFacebookFriend.h"
#include "EziFBIncomingRequestManager.h"
#include "RequestListScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

Scene* RequestAPIScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    RequestAPIScene *layer = RequestAPIScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool RequestAPIScene::init()
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
void RequestAPIScene::onEnterTransitionDidFinish()
{
    CCLOG("[FeedAPIScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void RequestAPIScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that you can use EziSocial to send gifts, invites and challenges to user's Facebook friends? Also you can process these requests on receiver's device");

    
    SpriteBatchNode* batchNode = SpriteBatchNode::create(PNG_FILE(kRESOURCE_REQUEST));
    this->addChild(batchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_REQUEST));
    
    buildHeader(-1, "Request APIs", "Send gifts, invites and much more with Facebook");
    
    // Build the tiles.
    MenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_send_gifts_nrl",      "btn_send_gifts_prd",         0, 0);
    menuItem->setCallback(CC_CALLBACK_1(RequestAPIScene::sendGifts, this));
    
    menuItem = this->buildAndPlaceTile("btn_challenge_nrl",      "btn_challenge_prd",           2, 0);
    menuItem->setCallback(CC_CALLBACK_1(RequestAPIScene::challengeFriends, this));
    
    menuItem = this->buildAndPlaceTile("btn_invite_friends_nrl",      "btn_invite_friends_prd", 1, 1);
    menuItem->setCallback(CC_CALLBACK_1(RequestAPIScene::inviteFriends, this));
    
    menuItem = this->buildAndPlaceTile("btn_new_requests_nrl",      "btn_new_requests_prd",     0, 2);
    menuItem->setCallback(CC_CALLBACK_1(RequestAPIScene::refreshIncomingRequests, this));
    
    Size menuSize = menuItem->getContentSize();
    _pendingRequestLabel = LabelTTF::create("0", "Arial", AppDelegate::SCREEN_SCALE_FACTOR * 20);
    menuItem->addChild(_pendingRequestLabel, 5);
    _pendingRequestLabel->cocos2d::CCNode::setPosition(menuSize.width * 0.2, menuSize.height * 0.2);
    
    menuItem = this->buildAndPlaceTile("btn_view_open_nrl",      "btn_view_open_prd",           2, 2);
    menuItem->setCallback(CC_CALLBACK_1(RequestAPIScene::viewOpenRequestItems, this));
}


void RequestAPIScene::sendGifts(Ref* sender)
{
    
    EziSocialObject::sharedObject()->isFacebookSessionActive();
    std::unordered_map<std::string, std::string> giftDictionary;
    
    giftDictionary["GOLD_COINS"] = "500";
    giftDictionary["LIFE"] = "1";
    
    std::vector<std::string> preselectFriends = getRandomFriends(10);
    
    if (!preselectFriends.empty())
    {
        EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT,
                                                              "I am sending you 500 gold coins & 1 extra life. Enjoy!",
                                                              preselectFriends,
                                                              giftDictionary, "");
    }
    else
    {
        MessageBox("Unable to send gifts as friends list is empty.", "Gift Sending Error");
    }
    
}

void RequestAPIScene::inviteFriends(Ref* sender)
{
    
    std::vector<std::string> preselectFriends;
    std::unordered_map<std::string, std::string> giftDictionary;
    
    EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE,
                                                          "EziSocial Plugin is damm! easy. Try now!", preselectFriends, giftDictionary, "送禮物");
}

void RequestAPIScene::challengeFriends(Ref* sender)
{
    std::vector<std::string> preselectFriends;
    std::unordered_map<std::string, std::string> giftDictionary;

    EziSocialObject::sharedObject()->sendRequestToFriends(EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE,
                                                          "I have score 900 points in EziSocialDemo. Can you beat me?",
                                                          preselectFriends,
                                                          giftDictionary, "");

}

void RequestAPIScene::refreshIncomingRequests(Ref* sender)
{
    EziSocialObject::sharedObject()->checkIncomingRequest();
}

void RequestAPIScene::viewOpenRequestItems(Ref* sender)
{
    Scene *pScene = RequestListScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFlipAngular::create(1.0f, pScene));
}

void RequestAPIScene::fbSendRequestCallback(int responseCode, const char* responseMessage, const std::vector<std::string> &friendsGotRequests)
{
    MessageBox(responseMessage, "fbSendRequestCallback");
    
}

void RequestAPIScene::fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests)
{
    //CCString* message = CCString::createWithFormat("Total new incoming requests = %d", totalIncomingRequests);

    
    int pendingRequest = EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount();
    std::string message = "";
    
    std::stringstream ss, tt;
    ss << totalIncomingRequests;
    tt << pendingRequest;
    
    message = "Total new requests = " + ss.str() + " \n Total pending requests = " + tt.str();
    
    //CCString* message = CCString::createWithFormat("Total new requests = %d\n Total pending requests = %d", totalIncomingRequests, pendingRequest);

    MessageBox(message.c_str(), "fbIncomingRequestCallback");
}


