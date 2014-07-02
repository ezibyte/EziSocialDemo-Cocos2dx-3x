//
//  FBActionScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#include "FBActionScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

#include "ProfileScene.h"
#include "FeedAPIScene.h"
#include "ScoreAPIScene.h"
#include "EventsAPIScene.h"
#include "OpenGraphAPIScene.h"
#include "RequestAPIScene.h"
#include "FriendsScene.h"
#include "ShareDialogScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

Scene* FBActionScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    FBActionScene *layer = FBActionScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool FBActionScene::init()
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
void FBActionScene::onEnterTransitionDidFinish()
{
    CCLOG("[FBActionScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
    // Check if the Facebook User is available.
    EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
    
    if (currentUser)
    {
        this->showCurrentUserData(this);
    }
}

void FBActionScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that EziSocial Plugin supports more than 30 functionalities to empower your games with Facebook APIs? Try some of them.");
    
    _firstName = NULL;
    _lastName  = NULL;
    
    SpriteBatchNode* fbActionBatchNode = SpriteBatchNode::create(PNG_FILE(kRESOURCE_FBACTION));
    this->addChild(fbActionBatchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_FBACTION));
    
    MenuItemImage* currentMenuItem = NULL;
    _profileMenuItem = NULL;
    
    BaseScene::buildHeader(1, "EziSocial Plugin", "Integrate power of Facebook APIs in your Cocos2d-x Games");

    _profileMenuItem = this->buildAndPlaceTile("btn_profile_nrl", "btn_profile_prd", 0, 0);
    _profileMenuItem->setCallback(CC_CALLBACK_1(FBActionScene::loadProfileScene, this));
    
    currentMenuItem = this->buildAndPlaceTile("btn_feed_api_nrl", "btn_feed_api_prd", 2, 0);
    currentMenuItem->setCallback(CC_CALLBACK_1(FBActionScene::loadFeedAPIScene, this));
    //currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadFeedAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_score_api_nrl", "btn_score_api_prd", 3, 0);
    currentMenuItem->setCallback(CC_CALLBACK_1(FBActionScene::loadScoreAPIScene, this));
    //currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadScoreAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_event_api_nrl", "btn_event_api_prd", 0, 1);
    currentMenuItem->setCallback(CC_CALLBACK_1(FBActionScene::loadeventAPIScene, this));
    //currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadeventAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_open_graph_api_nrl", "btn_open_graph_api_prd", 1, 1);
    currentMenuItem->setCallback(CC_CALLBACK_1(FBActionScene::loadOpenGraphAPIScene, this));
    //currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadOpenGraphAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_request_api_nrl", "btn_request_api_prd", 3, 1);
    currentMenuItem->setCallback(CC_CALLBACK_1(FBActionScene::loadRequestAPIScene, this));
    //currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadRequestAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_friends_api_nrl", "btn_friends_api_prd", 0, 2);
    currentMenuItem->setCallback(CC_CALLBACK_1(FBActionScene::loadFriendsAPIScene, this));
    //currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadFriendsAPIScene));
    
    currentMenuItem = this->buildAndPlaceTile("btn_share_dialog_api_nrl", "btn_share_dialog_api_prd", 2, 2);
    currentMenuItem->setCallback(CC_CALLBACK_1(FBActionScene::loadShareDialogScene, this));
    //currentMenuItem->setTarget(this, menu_selector(FBActionScene::loadShareDialogScene));
    
    
}

void FBActionScene::loadProfileScene(Ref *sender)
{
    Scene *pScene = ProfileScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadFeedAPIScene(Ref *sender)
{
    Scene *pScene = FeedAPIScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadScoreAPIScene(Ref *sender)
{
    Scene *pScene = ScoreAPIScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadeventAPIScene(Ref *sender)
{
    Scene *pScene = EventsAPIScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadOpenGraphAPIScene(Ref *sender)
{
    Scene *pScene = OpenGraphAPIScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadRequestAPIScene(Ref *sender)
{
    Scene *pScene = RequestAPIScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadFriendsAPIScene(Ref *sender)
{
    Scene *pScene = FriendsScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::loadShareDialogScene(Ref *sender)
{
    Scene *pScene = ShareDialogScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFadeBL::create(1.0f, pScene));
}

void FBActionScene::showCurrentUserData(Ref *sender)
{
    EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
    
    if (currentUser)
    {
        CCLOG("Username = %s", currentUser->getUserName().c_str());
        _firstName = LabelTTF::create(currentUser->getFirstName(), "Arial", SCALED_VALUE(20.0f));
        _lastName  = LabelTTF::create(currentUser->getLastName(), "Arial", SCALED_VALUE(20.0f));
        
        if (_profileMenuItem != NULL)
        {
            _profileMenuItem->addChild(_firstName);
            _profileMenuItem->addChild(_lastName);
            
            _firstName->setAnchorPoint(Point(0.0f, 0.0));
            _lastName->setAnchorPoint(Point(0.0f, 0.0));
            
            Size menuItemSize = _profileMenuItem->getContentSize();
            
            _firstName->cocos2d::CCNode::setPosition(menuItemSize.width * 0.5, menuItemSize.height * 0.22f);
            _lastName->cocos2d::CCNode::setPosition(menuItemSize.width * 0.5, menuItemSize.height * 0.08f);
            
            
            Sprite* userPhoto = currentUser->getPhoto(CC_CALLBACK_2(FBActionScene::showCurrentUserPhoto, this),
                                  false,
                                  menuItemSize.width * 0.4,
                                  menuItemSize.width * 0.4);
            
            if (userPhoto != nullptr)
            {
                this->showCurrentUserPhoto(currentUser, userPhoto);
            }
        }
    }
    else
    {
        CCLOG("Cannot find current user.");
    }
}

void FBActionScene::showCurrentUserPhoto(Ref *sender, Sprite* photo)
{
    
    if (sender != nullptr)
    {
        Size menuSize = _profileMenuItem->getContentSize();
        
        LayerColor* whiteBGLayer = LayerColor::create(Color4B(0, 0, 0, 255), menuSize.width * 0.44, menuSize.width * 0.44);
        _profileMenuItem->addChild(whiteBGLayer);
        
        float wPy = (menuSize.height - whiteBGLayer->getContentSize().height)/2;
        float wPx = wPy;
        
        whiteBGLayer->cocos2d::CCNode::setPosition(wPx, wPy);
        
        _profileSprite = photo;
        _profileSprite->setAnchorPoint(Point(0.0, 0.0));
        
        float pPy = (menuSize.height - _profileSprite->getContentSize().height)/2;
        float pPx = pPy;
        
        _profileSprite->cocos2d::CCNode::setPosition(pPx, pPy);
        
        _profileMenuItem->addChild(_profileSprite);
        
    }
    else
    {
        // Display rotating loading image.
        CCLOG("Image not available yet.");
    }
    
}


//-------------------------------------
    #pragma mark - eziSocial delegates
//-------------------------------------

void FBActionScene::fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_USER_DETAIL_SUCCESS:
            showCurrentUserData(this);
            break;
            
        default:
            MessageBox(responseMessage, "Error - User details");
            break;
    }
}