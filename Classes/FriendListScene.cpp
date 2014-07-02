//
//  FriendListScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 05/09/13.
//
//

#include "FriendListScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "EziFacebookFriend.h"
#include "GameConstants.h"
#include "FriendsScene.h"
//#include "CocosGUI.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define TAG_CONSUMED_ICON 100
#define TAG_USE_MENU      200
#define TAG_MESSAGE       300
#define TAG_PHOTO         400

Scene* FriendListScene::scene(FRIEND_CIRCLE friendCircle)
{
    // 'scene' is an autorelease object
    auto *scene = Scene::create();
    
    // 'layer' is an autorelease object
    FriendListScene *layer = FriendListScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    layer->setSelectedFriendCircle(friendCircle);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool FriendListScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}

void FriendListScene::setSelectedFriendCircle(FRIEND_CIRCLE selectedFriendCircle)
{
    _selectedFriendCircle = selectedFriendCircle;
    this->buildScene();
}

/**
 *  Use this method for autologin in facebook account.
 */
void FriendListScene::onEnterTransitionDidFinish()
{
    //CCLOG("[HomeScene]: Enter Transition Finished.");
    //EziSocialObject::sharedObject()->performLoginUsingFacebook(true);
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
    switch (_selectedFriendCircle)
    {
        case ALL_FRIENDS:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, 0, 40);
            break;
            
        case INSTALLED_ONLY:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_INSTALLED, 0, 500);
            break;
            
        case NON_PLAYING_ONLY:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_NOT_INSTALLED, 10, 40);
            break;
            
        case MY_DEVICE_ONLY:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_MY_DEVICES, 0, 500);
            break;
            
        default:
            EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, 0, 40);
            break;
    }
}

void FriendListScene::buildScene()
{
    _friendDictionary.clear();
    _friendList.clear();
    _tableView = NULL;
    _downloadCount = 0;
    
    const char* headerString = "";
    
    switch (_selectedFriendCircle)
    {
        case ALL_FRIENDS:
            headerString = "All Friends";
            break;
        
        case INSTALLED_ONLY:
            headerString = "Friends Playing Game";
            break;
            
        case NON_PLAYING_ONLY:
            headerString = "Friends not Playing Game";
            break;
            
        case MY_DEVICE_ONLY:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            headerString = "Friends playing on iOS Device";
#else
            headerString = "Friends playing on Android Device";
#endif
        default:
            break;
    }
    
    //_friendList = CCArray::createWithCapacity(5);
    //_friendList->retain();
    CCLOG("ALL Request count = %lu", _friendList.size());
    
    // Add background layers to the scene.
    LayerColor *bg = LayerColor::create(Color4B(255, 255, 255, 255), AppDelegate::SCREEN_WIDTH, AppDelegate::SCREEN_HEIGHT);
    this->addChild(bg);
    
    LayerColor *secondBG = LayerColor::create(Color4B(95, 95, 95, 255),
                                                  AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f),
                                                  AppDelegate::SCREEN_HEIGHT - SCALED_VALUE(20.0f));
    
    secondBG->setPosition(Point(SCALED_VALUE(10.0f), SCALED_VALUE(10.0f)));
    this->addChild(secondBG);
    
    // Set the table header.
    LayerColor *header = LayerColor::create(Color4B(66, 66, 66, 255),
                                                AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f),
                                                SCALED_VALUE(100.0f));
    header->setAnchorPoint(Point(0, 0));
    header->setPosition(Point(SCALED_VALUE(10.0f), AppDelegate::SCREEN_HEIGHT - SCALED_VALUE(110.0f)));
    this->addChild(header);
    
    // Set the header text.
    auto headerText = LabelTTF::create(headerString, "Arial", SCALED_VALUE(40.0f));
    headerText->setAnchorPoint(Point(0.0f, 0.5f));
    header->addChild(headerText);
    headerText->setPositionX(SCALED_VALUE(28.0f));
    headerText->setPositionY(header->getContentSize().height/2);
    
    // Add back button go back to previous scene
    MenuItemImage *backButton = MenuItemImage::create();
    backButton->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_table_back_nrl"));
    backButton->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_table_back_prd"));
    
    backButton->setCallback(CC_CALLBACK_1(FriendListScene::backButtonPressed, this));

    backButton->setAnchorPoint(Point(1.0f, 0.5));
    Menu* backMenu = Menu::create(backButton, NULL);
    header->addChild(backMenu);
    backMenu->setPositionX(header->getContentSize().width - SCALED_VALUE(20.0f));
    backMenu->setPositionY(header->getContentSize().height/2);
    
    
    // Create the table view.
    float tableHeight = bg->getContentSize().height - secondBG->getPositionY() - (header->getContentSize().height) - SCALED_VALUE(10);
    _tableView = TableView::create(this, Size(secondBG->getContentSize().width, tableHeight));
    _tableView->setAnchorPoint(Point(0, 0.0));
    _tableView->setDirection(ScrollView::Direction::VERTICAL);
    _tableView->setPosition(Point(SCALED_VALUE(10.0f), SCALED_VALUE(10)));
    _tableView->setDelegate(this);
    this->addChild(_tableView);
}

void FriendListScene::backButtonPressed(Ref* sender)
{
    Scene* pScene = FriendsScene::scene();
    ReplaceToLeft(pScene);
}

void FriendListScene::onKeyBackClicked()
{
    backButtonPressed(this);
}

void FriendListScene::useRequest(cocos2d::Node *sender)
{
    CCLOG("Use Button Tag = %d", sender->getTag());
}

//-------------------------------------
#pragma mark - Tableview delegates
//-------------------------------------

void FriendListScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}
Size FriendListScene::cellSizeForTable(TableView *table)
{
    return Size(AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f), SCALED_VALUE(124.0f));
}

TableViewCell* FriendListScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    int index = idx;
    EziFacebookFriend* myFriend = (EziFacebookFriend*)_friendList.at(index);
    std::string friendName = myFriend->getName();
    //const char* photoPath = myFriend->getPhotoPath().c_str();
    bool toRequestForPhoto = false;
    
    if (myFriend->getPhoto() == nullptr)
    {
        toRequestForPhoto = true;
        //EziSocialObject::sharedObject()->getProfilePicForID(this, myFriend->getFBID(), SCALED_VALUE(85.0f), SCALED_VALUE(85.0f), false);
    }
    
    // Build the table cell.
    TableViewCell *cell = table->cellAtIndex(idx);
    
    if (cell == NULL)
    {
        cell = new TableViewCell();
        cell->autorelease();
        
        LayerColor* colorLayer = LayerColor::create(Color4B(100, 100, 100, 200),
                                                        AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f),
                                                        SCALED_VALUE(124.0f));
        //cell->addChild(colorLayer);
        
        LayerColor* photoLayer = LayerColor::create(Color4B(255, 255, 255, 255),
                                                        SCALED_VALUE(100.0f),
                                                        SCALED_VALUE(100.0f));
        photoLayer->setPosition(SCALED_VALUE(40.0f), SCALED_VALUE(12.0f));
        cell->addChild(photoLayer);
        Sprite* userPhotoSprite = NULL;
        
        if (toRequestForPhoto)
        {
            userPhotoSprite = Sprite::create();
            userPhotoSprite->setContentSize(Size(SCALED_VALUE(85.0f), SCALED_VALUE(85.0f)));
        }
        else
        {
            userPhotoSprite = myFriend->getPhoto(CC_CALLBACK_2(FriendListScene::applyPhoto, this),
                                                 false,
                                                 SCALED_VALUE(85.0f),
                                                 SCALED_VALUE(85.0f));
            
            if (userPhotoSprite == nullptr) { userPhotoSprite = Sprite::create(); }
        }
        
        cell->addChild(userPhotoSprite);
        userPhotoSprite->setAnchorPoint(Point(0.0f, 0.0));
        userPhotoSprite->cocos2d::CCNode::setPosition(SCALED_VALUE(47.0f), SCALED_VALUE(18.0f));
        userPhotoSprite->setTag(TAG_PHOTO);
        
        auto message = LabelTTF::create(friendName.c_str(), "Arial", SCALED_VALUE(30.0f));
        message->setAnchorPoint(Point(0, 0.5));
        message->setPosition(Point(photoLayer->getContentSize().width + photoLayer->getPositionX() + SCALED_VALUE(20.0f),
                                 colorLayer->getContentSize().height/2));
        cell->addChild(message);
        message->setTag(TAG_MESSAGE);
        
        
        /*
        Sprite* consumedIcon = Sprite::createWithSpriteFrameName("green_tick");
        consumedIcon->setAnchorPoint(Point(1.0, 0.5));
        cell->addChild(consumedIcon);
        consumedIcon->setPosition(Point(colorLayer->getContentSize().width - SCALED_VALUE(20.0f),
                                      colorLayer->getContentSize().height/2));
        message->setTag(TAG_CONSUMED_ICON);
        
        MenuItemImage* useButton = MenuItemImage::create();
        useButton->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_use_prd"));
        useButton->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_use_nrl"));
        useButton->setAnchorPoint(Point(1.0, 0.5));
        useButton->setTarget(this, menu_selector(FriendListScene::useRequest));
        
        Menu* useMenu = Menu::create(useButton, NULL);
        cell->addChild(useMenu);
        useMenu->setPosition(Point(colorLayer->getContentSize().width - SCALED_VALUE(20.0f),
                                 colorLayer->getContentSize().height/2));
        
        useButton->setTag(index);
        useMenu->setTag(TAG_USE_MENU);
        */
        
    }
    else
    {
        Sprite* cellProfilePic = (Sprite*)cell->getChildByTag(TAG_PHOTO);
        
        if (toRequestForPhoto == false)
        {
            //cellProfilePic->setTexture(Sprite::create(myFriend->getPhotoPath())->getTexture());
            Sprite* tempSprite = myFriend->getPhoto();
            
            
            if (myFriend->getPhoto() != nullptr)
            {
                cellProfilePic->setTexture(tempSprite->getTexture());
            }
            else
            {
                myFriend->getPhoto(CC_CALLBACK_2(FriendListScene::applyPhoto, this),
                                   false,
                                   SCALED_VALUE(85.0f),
                                   SCALED_VALUE(85.0f));    
            }
            
            
        }
        
        // Update the message Label.
        auto messageLabel = (LabelTTF*)cell->getChildByTag(TAG_MESSAGE);
        CCLOG("Friend Name = %s", friendName.c_str());
        
        if (messageLabel)
        {
            messageLabel->setString(friendName.c_str());    
        }
        
        
    }
    
    return cell;
}

ssize_t FriendListScene::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _friendList.size();
}



//-------------------------------------
#pragma mark - eziSocial delegates
//-------------------------------------

void FriendListScene::fbFriendsCallback(int responseCode, const char *responseMessage, std::vector<EziFacebookFriend*> friends)
{
    MessageBox(responseMessage, "Friends Callback");
    
    // Empty the current friend list & dictionary.
    _friendList.clear();
    _friendDictionary.clear();
    
    _friendList.swap(friends);
    
    int totalFriends = _friendList.size()-1;
    _downloadCount = 0;
    
    for (int i=totalFriends; i >= 0; i--)
    {
        EziFacebookFriend* myFriend = (EziFacebookFriend*)_friendList.at(i);
        
        CCLOG("%d. %s", i+1, myFriend->getProfileID().c_str());
        
        //_friendDictionary.insert("ABC", "ABC");
        
        
        _friendDictionary.insert(myFriend->getProfileID(), myFriend);
        
        myFriend->getPhoto(CC_CALLBACK_2(FriendListScene::applyPhoto, this), false, SCALED_VALUE(85.0f), SCALED_VALUE(85.0f));
        
        //EziSocialObject::sharedObject()->getProfilePicForID(this, myFriend->getProfileID().c_str(), SCALED_VALUE(85.0f), SCALED_VALUE(85.0f), false);
    }
    
    
    _tableView->reloadData();
}

void FriendListScene::applyPhoto(Ref* sender, Sprite* photo)
{
    _downloadCount++;
    if (_downloadCount == _friendList.size())
    {
        // this is the last we fetched. We can refresh the table view now.
        CCLOG("Download completed...");
        _tableView->reloadData();
    }
    
}

void FriendListScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
    if (_friendDictionary.size() == 0)
    {
        return;
    }
    
    EziFacebookFriend* myFriend = _friendDictionary.at(fbID);
    
    if (myFriend)
    {
        myFriend->saveData(userPhotoPath, "photo");        
        CCLOG("Downloaded photo for = %s", myFriend->getName().c_str());
    }
    else
    {
        CCLOG("Friend with ID = %s not found.", fbID);
    }
    
    _downloadCount++;
    if (_downloadCount == _friendList.size())
    {
        // this is the last we fetched. We can refresh the table view now.
        CCLOG("Download completed...");
        _tableView->reloadData();
    }
    
}