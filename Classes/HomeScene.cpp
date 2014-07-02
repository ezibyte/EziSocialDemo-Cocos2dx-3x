//
//  HomeScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 01/09/13.
//
//

#include "HomeScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

USING_NS_CC_EXT;


Scene* HomeScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    HomeScene *layer = HomeScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool HomeScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
        
    buildScene();
    
    return true;
}

/**
 *  Use this method for autologin in facebook account.
 */
void HomeScene::onEnterTransitionDidFinish()
{
    CCLOG("[HomeScene]: Enter Transition Finished.");
    // Set the delegates to receive responses.
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    EziSocialObject::sharedObject()->setEmailDelegate(this);
    EziSocialObject::sharedObject()->setTwitterDelegate(this);
    EziSocialObject::sharedObject()->setAutoCheckIncomingRequestsOnAppLaunch(true);
}

void HomeScene::buildScene()
{
    // Call suprt build scene...
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that you can empower your game with Facebook APIs using EziSocial plugin in just 10 mintues?");

    SpriteBatchNode* homeBatchNode = SpriteBatchNode::create(PNG_FILE(kRESOURCE_HOME));
    this->addChild(homeBatchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_HOME));
    
    BaseScene::buildHeader(0, "EziSocial Plugin", "Integrate power of Facebook APIs in your Cocos2d-x Games");
    
    // Load Tiles Button
    MenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_fb_login_nrl",   "btn_fb_login_prd", 0, 0);
    menuItem->setCallback(CC_CALLBACK_1(HomeScene::facebookButtonPressed, this));
    //menuItem->setTarget(this, menu_selector(HomeScene::facebookButtonPressed));
    
    menuItem = this->buildAndPlaceTile("btn_tweet_nrl",      "btn_tweet_prd",    1, 1);
    //menuItem->setTarget(this, menu_selector(HomeScene::tweetButtonPressed));
    menuItem->setCallback(CC_CALLBACK_1(HomeScene::tweetButtonPressed, this));
    
    menuItem = this->buildAndPlaceTile("btn_email_nrl",      "btn_email_prd",    0, 2);
    //menuItem->setTarget(this, menu_selector(HomeScene::emailButtonPressed));
    menuItem->setCallback(CC_CALLBACK_1(HomeScene::emailButtonPressed, this));
    
    /*
    
    // Download Photo test
    cocos2d::network::HttpRequest *request = new cocos2d::network::HttpRequest();
    request->setUrl("https://fbcdn-profile-a.akamaihd.net/hprofile-ak-frc1/t1.0-1/p120x120/998155_284465998376710_852962666_a.jpg"); // facebook profile picture
    
    //request->setUrl("http://graph.facebook.com/100000706355105/picture?width=120&height=120");
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(HomeScene::onRequestCompleted));
    cocos2d::network::HttpClient::getInstance()->send(request);
     
    */
}

void HomeScene::onRequestCompleted(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response)
{
    
    response->getResponseCode(); // is -1
    response->getErrorBuffer(); // is 'Protocol https not supported or disabled in libcurl'
    
    /*
    
    // Dump the data
    std::vector<char> *buffer = response->getResponseData();
    
    // Create the Sprite from data dump...
    
    
    
    cocos2d::Image * img=new cocos2d::Image();
    img->initWithImageData((unsigned char*)&(buffer->front()), buffer->size());
    

    
    Texture2D *tempTexture = new Texture2D();
    tempTexture->initWithImage(img);
    
    Sprite *photo = Sprite::createWithTexture(tempTexture);
    photo->setPosition(100, 100);
    this->addChild(photo, 1000);

    
    
    
    Data *data = new Data();// = new Data();
    data->fastSet((unsigned char*)&(buffer->front()), buffer->size());
    UserDefault::getInstance()->setDataForKey("paras.jpg", *data);
    
    */
    
    //delete data;
    
    //data.fastSet((unsigned char*)&(buffer->front()), buffer->size());
    
    /*
    
    UserDefault::getInstance()->setDataForKey("paras.jpg", data);
    
    // Fetch back the data from the storage.
    Data tempData;
    tempData = UserDefault::getInstance()->getDataForKey("paras.jpg");
    
    if (!tempData.isNull())
    {
        cocos2d::Image * img2=new cocos2d::Image();
        img2->initWithImageData(tempData.getBytes(), tempData.getSize());
        Texture2D *tempTexture2 = new Texture2D();
        tempTexture2->initWithImage(img2);
        
        Sprite* photo2 = Sprite::createWithTexture(tempTexture2);
        photo2->setPosition(300, 100);
        this->addChild(photo2, 1000);
    }
    
     */
    
    //std::string writablePath = FileUtils::getInstance()->getWritablePath();
    //writablePath.append("paras.jpg");
    //img->saveToFile(writablePath.c_str());
    
    //CCLOG("Path = %s", writablePath.c_str());
}

void HomeScene::facebookButtonPressed(Ref *sender)
{
    if (EziSocialObject::sharedObject()->isFacebookSessionActive())
    {
        this->loadFBActionScene();
    }
    else
    {
        EziSocialObject::sharedObject()->performLoginUsingFacebook(false);
    }
}

void HomeScene::tweetButtonPressed(Ref *sender)
{
    EziSocialObject::sharedObject()->getHighScores();
    //EziSocialObject::sharedObject()->tweet("Welcome to the world of EziSocial Plugin. Try now at http://ezibyte.com", NULL);
}

void HomeScene::emailButtonPressed(Ref *sender)
{
    EziSocialObject::sharedObject()->sendEmail("EziSocial Plugin Feedback",
                                               "<p><B>Please write your feedback here</B></p><br>",
                                               "support@ezibyte.com");
}

void HomeScene::fbHighScoresCallback(int responseCode, const char* responseMessage, const std::vector<EziFacebookFriend*> &highScores)
{
    CCLOG("response message = %s", responseMessage);
    for (int i=0; i<highScores.size(); i++)
    {
        EziFacebookFriend* tempFriend = highScores.at(i);
        CCLOG("%s = %lu", tempFriend->getName().c_str(), tempFriend->getScore());
    }
}

//-------------------------------------
    #pragma mark - eziSocial delegates
//-------------------------------------

void HomeScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
    BaseScene::fbSessionCallback(responseCode, responseMessage);
    if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL)
    {
        this->loadFBActionScene();
    }
}

