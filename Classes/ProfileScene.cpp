//
//  ProfileScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "ProfileScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

Scene* ProfileScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    ProfileScene *layer = ProfileScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool ProfileScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void ProfileScene::onEnterTransitionDidFinish()
{
    CCLOG("[ProfileScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    if (EziSocialObject::sharedObject()->getCurrentFacebookUser() != NULL)
    {
        this->buildScene();
    }
    else
    {
        EziSocialObject::sharedObject()->setFacebookDelegate(this);
        EziSocialObject::sharedObject()->fetchFBUserDetails(true);
    }
}

void ProfileScene::buildScene()
{

    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that you can give personalize experience to your users? Just Fetch the details from Facebook using EziSocial. :)");
    
    SpriteBatchNode* batchNode = SpriteBatchNode::create(PNG_FILE(kRESOURCE_PROFILE));
    this->addChild(batchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_PROFILE));
    buildHeader(-1, "Profile Page", "Fetch the details of user from Facebook");
    
    _userSprite = Sprite::createWithSpriteFrameName("fb_user_icon");
    _userSprite->setAnchorPoint(Point(1.0f, 1.0f));
    _userSprite->setScale(2.0f);
    
    this->addChild(_userSprite);
    PlaceNode(_userSprite, 0.98, 0.756);
    
    EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
    
    auto userName = LabelTTF::create(currentUser->getFullName(), "Arial", SCALED_VALUE(80.0f));
    userName->setAnchorPoint(Point(1.0f, 1.0f));
    PlaceNode(userName, 0.98, 0.5);
    userName->setPositionY(_userSprite->getPositionY() - (_userSprite->getContentSize().height * _userSprite->getScaleY()));
    
    auto userEmailID = LabelTTF::create(currentUser->getEmailID(), "Arial", SCALED_VALUE(40.0f));
    userEmailID->setColor(Color3B(255, 255, 0));
    userEmailID->setAnchorPoint(Point(1.0f, 1.0f));
    PlaceNode(userEmailID, 0.98, 0.5);
    userEmailID->setPositionY(userName->getPositionY() - (userName->getContentSize().height * userName->getScaleY()));
    
    std::string textToDisplay = "";
    std::string gender = currentUser->getGender();
    
    textToDisplay.append("A '").append(gender).append("' member, born on '").append(currentUser->getBirthday()).append("', is using '").append(currentUser->getUserName()).append("' as username on Facebook, and lives in '").append(currentUser->getPresentLocation()).append("' but belongs to '").append(currentUser->getHomeTown()).append("'. Profile ID of this user is '").append(currentUser->getProfileID()).append("' and access token is ");
    
    CCLOG("Text to display = %s", textToDisplay.c_str());
    
    auto userDetails = LabelTTF::create("", "Arial", SCALED_VALUE(20.0f));
    userDetails->setHorizontalAlignment(TextHAlignment::RIGHT);
    userDetails->setDimensions(Size(AppDelegate::SCREEN_WIDTH * 0.65, AppDelegate::SCREEN_HEIGHT * 0.13));
    userDetails->setString(textToDisplay.c_str());
    userDetails->setColor(Color3B(0, 255, 255));
    userDetails->setAnchorPoint(Point(1.0f, 1.0f));
    PlaceNode(userDetails, 0.98, 0.5);
    userDetails->setPositionY(userEmailID->getPositionY() - (userEmailID->getContentSize().height * userEmailID->getScaleY()));
    
    auto accessToken = LabelTTF::create("", "Arial", SCALED_VALUE(15.0));
    accessToken->setHorizontalAlignment(TextHAlignment::RIGHT);
    accessToken->setVerticalAlignment(TextVAlignment::TOP);
    accessToken->setDimensions(Size(AppDelegate::SCREEN_WIDTH * 0.70, AppDelegate::SCREEN_HEIGHT * 0.10));
    accessToken->setString(currentUser->getAccessToken());
    accessToken->setAnchorPoint(Point(1.0f, 1.0f));
    PlaceNode(accessToken, 0.98, 0.5);
    accessToken->setPositionY(userDetails->getPositionY() - (userDetails->getContentSize().height * userDetails->getScaleY()));

    
    this->addChild(userName);
    this->addChild(userEmailID);
    this->addChild(userDetails);
    this->addChild(accessToken);
    
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
    Sprite* userPhoto = currentUser->getPhoto(CC_CALLBACK_2(ProfileScene::applyPhoto, this),
                          false,
                          _userSprite->getContentSize().width * _userSprite->getScaleX(),
                          _userSprite->getContentSize().height * _userSprite->getScaleY());
    
    if (userPhoto != nullptr)
    {
        applyPhoto(currentUser, userPhoto);
    }

}

void ProfileScene::fbUserDetailCallback(int responseCode, const char *responseMessage, EziFacebookUser *fbUser)
{
    if (fbUser != NULL)
    {
        EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);
        this->buildScene();
    }
    MessageBox(responseMessage, "User Details Response");

}

void ProfileScene::applyPhoto(Ref* sender, Sprite* photo)
{
    _userSprite->addChild(photo);
    photo->cocos2d::CCNode::setPosition(_userSprite->getContentSize().width/2, _userSprite->getContentSize().height/2);
    photo->setScale(_userSprite->getContentSize().width/photo->getContentSize().width);
}
