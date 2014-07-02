//
//  RequestListScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 10/09/13.
//
//

#include "RequestListScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "EziFacebookFriend.h"
#include "GameConstants.h"
#include "RequestAPIScene.h"
#include "EziFBIncomingRequestManager.h"
#include "EziFBIncomingRequest.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define TAG_TICK_ICON     100
#define TAG_PHOTO         200
#define TAG_TTTLE         300
#define TAG_MESSAGE       400
#define TAG_ITEM_ICON     500

Scene* RequestListScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    RequestListScene *layer = RequestListScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool RequestListScene::init()
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
void RequestListScene::onEnterTransitionDidFinish()
{
    //CCLOG("[HomeScene]: Enter Transition Finished.");
    //EziSocialObject::sharedObject()->performLoginUsingFacebook(true);
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
    
}

void RequestListScene::onExit()
{
    Layer::onExit();
    _fbMessagesList.clear();
    _selectedRequestItems.clear();
    EziSocialObject::sharedObject()->setFacebookDelegate(NULL);
}

void RequestListScene::buildScene()
{
    _fbMessagesList.clear();
    _tableView      = NULL;
    _downloadCount  = 0;
    _friendDictionary.clear();
    _selectedRequestItems.clear();
    _allSelected = false;
    
    
    _fbMessagesList = EziFBIncomingRequestManager::sharedManager()->getAllRequests();
    
    CCLOG("ALL Request count = %lu", _fbMessagesList.size());
    
    SpriteBatchNode* batchNode = SpriteBatchNode::create(PNG_FILE(kRESOURCE_REQUEST));
    this->addChild(batchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_REQUEST));
    
    
    const char* headerString = "Your Messages";
    
    
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
    backButton->setCallback(CC_CALLBACK_1(RequestListScene::backButtonPressed, this));

    backButton->setAnchorPoint(Point(1.0f, 0.5));
    Menu* backMenu = Menu::create(backButton, NULL);
    header->addChild(backMenu);
    backMenu->setPositionX(header->getContentSize().width - SCALED_VALUE(20.0f));
    backMenu->setPositionY(header->getContentSize().height/2);
    
    // Set the footer.
    LayerColor *footer = LayerColor::create(Color4B(66, 66, 66, 255),
                                                AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f),
                                                SCALED_VALUE(100.0f));
    footer->setAnchorPoint(Point(0, 0));
    footer->setPosition(Point(SCALED_VALUE(10.0f), SCALED_VALUE(0.0f)));
    this->addChild(footer);
    
    // Add buttons to the footer.
    MenuItemImage *acceptButton = MenuItemImage::create();
    acceptButton->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_accept_nrl"));
    acceptButton->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_accept_prd"));
    acceptButton->setCallback(CC_CALLBACK_1(RequestListScene::acceptButtonPressed, this));
    acceptButton->setAnchorPoint(Point(0.0f, 0.5));
    
    _allButton = MenuItemImage::create();
    _allButton->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("check_all_unslected"));
    _allButton->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("check_all_prd"));
    _allButton->setCallback(CC_CALLBACK_1(RequestListScene::acceptAllCheckboxPressed, this));
    _allButton->setAnchorPoint(Point(1.0f, 0.5));

    
    Menu* footerMenu = Menu::create(_allButton, acceptButton, NULL);
    footerMenu->alignItemsHorizontallyWithPadding(SCALED_VALUE(20.0f));
    footer->addChild(footerMenu);
    footerMenu->setPositionX(footer->getContentSize().width/2);
    footerMenu->setPositionY(footer->getContentSize().height/2);
    
    // Create the table view.
    float tableHeight = bg->getContentSize().height - secondBG->getPositionY() - (2 * header->getContentSize().height) - SCALED_VALUE(10);
    _tableView = TableView::create(this, Size(secondBG->getContentSize().width, tableHeight));
    _tableView->setAnchorPoint(Point(0, 0.0));
    _tableView->setDirection(ScrollView::Direction::VERTICAL);
    _tableView->setPosition(Point(SCALED_VALUE(10.0f), SCALED_VALUE(10) + (header->getContentSize().height)));
    _tableView->setDelegate(this);
    this->addChild(_tableView);

}

void RequestListScene::acceptAllCheckboxPressed(Ref* sender)
{
    CCLOG("Accept All button pressed.");
    _allSelected = !_allSelected;
    
    if (_allSelected)
    {
        _allButton->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("check_all_selected"));
        int totalRequest = _fbMessagesList.size();
        bool anyChanges = false;
        
        for (int i=0; i<totalRequest; i++)
        {
            EziFBIncomingRequest* message = (EziFBIncomingRequest*)_fbMessagesList.at(i);
            
            if (_selectedRequestItems.find(message->getRequestID()) != _selectedRequestItems.end())
            {
            }
            else {
                anyChanges = true;
                _selectedRequestItems[message->getRequestID()] =  message;
                //_selectedRequestItems.insert(message->getRequestID(), message);
            }
        }
        
        if (anyChanges)
        {
            _tableView->reloadData();
        }

    }
    else
    {
        _allButton->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("check_all_unslected"));
        bool needsReload = false;
        if (_selectedRequestItems.size() > 0)
        {
            needsReload = true;
        }
        
        _selectedRequestItems.clear();
        
        if (needsReload)
        {
            _tableView->reloadData();    
        }
        
    }
}

void RequestListScene::acceptButtonPressed(Ref* sender)
{
    CCLOG("accept button pressed.");
    if (_selectedRequestItems.size() == 0)
    {
        MessageBox("Please select an message to use.", "No message selected.");
    }
    else
    {
        int totalRequest = _fbMessagesList.size();
        bool anyChanges = false;
        std::vector<EziFBIncomingRequest*> objectToKeep;
        objectToKeep.clear();
        
        for (int i=0; i<totalRequest; i++)
        {
            EziFBIncomingRequest* message = _fbMessagesList.at(i);
            if (_selectedRequestItems.find(message->getRequestID()) != _selectedRequestItems.end())
            {
                anyChanges = true;
                EziFBIncomingRequestManager::sharedManager()->consumeItem(message);
            }
            else
            {
                objectToKeep.push_back(message);
            }
        }
        
        if (anyChanges == true)
        {
            _fbMessagesList.swap(objectToKeep);
            
            /*
            int totalToRemove = objectToRemove.size();
            
            for (int i=0; i<totalToRemove; i++)
            {
                _fbMessagesList.erase(_fbMessagesList.)
            }
            
            
            _fbMessagesList->removeObjectsInArray(objectToRemove);
             */
            EziFBIncomingRequestManager::sharedManager()->clearCompletedRequestList();
            _tableView->reloadData();
        }
    }
}

void RequestListScene::backButtonPressed(Ref* sender)
{
    Scene* pScene = RequestAPIScene::scene();
    Director::getInstance()->replaceScene(CCTransitionFlipAngular::create(1.0f, pScene));
}

void RequestListScene::onKeyBackClicked()
{
    backButtonPressed(this);
}

void RequestListScene::useRequest(cocos2d::Node *sender)
{
    CCLOG("Use Button Tag = %d", sender->getTag());
}

//-------------------------------------
#pragma mark - Tableview delegates
//-------------------------------------

void RequestListScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("Table cell touched");
    Sprite* selection = (Sprite*)cell->getChildByTag(TAG_TICK_ICON);
    EziFBIncomingRequest* selectedRequestItem = (EziFBIncomingRequest*)selection->getUserData();
    
    if (selectedRequestItem == NULL)
    {
        return;
    }
    
    if (_selectedRequestItems.find(selectedRequestItem->getRequestID()) != _selectedRequestItems.end())
    {
        _allButton->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("check_all_unslected"));
        _allSelected = false;
        selection->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("row_not_selected"));
        _selectedRequestItems.erase(selectedRequestItem->getRequestID());
        //_selectedRequestItems->removeObjectForKey(selectedRequestItem->getRequestID());

    }
    else
    {
        selection->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("row_selected"));
        _selectedRequestItems[selectedRequestItem->getRequestID()] = selectedRequestItem;
        //_selectedRequestItems.insert(selectedRequestItem->getRequestID(), selectedRequestItem);
    }
    
    
}


Size RequestListScene::cellSizeForTable(TableView *table)
{
    return Size(AppDelegate::SCREEN_WIDTH - SCALED_VALUE(20.0f), SCALED_VALUE(124.0f));
}

TableViewCell* RequestListScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    int index = idx;
    EziFBIncomingRequest* messageRequest = _fbMessagesList.at(index);
    EziFacebookFriend* myFriend = messageRequest->getSender();
    
    std::string friendName = myFriend->getName();
    std::string messageTitle = this->getFancyTitle(messageRequest);
    std::string messageToDisplay = this->getFancyMessage(messageRequest);
    
    const char* photoPath = myFriend->getPhotoPath().c_str();
    bool toRequestForPhoto = false;
    
    if (strcmp(photoPath, "") == 0)
    {
        toRequestForPhoto = true;
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
        
        Sprite* tickSprite = Sprite::createWithSpriteFrameName("row_not_selected");
        
        if (_selectedRequestItems.find(messageRequest->getRequestID()) != _selectedRequestItems.end())
        {
            tickSprite->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("row_selected"));
        }
        
        cell->addChild(tickSprite);
        tickSprite->setAnchorPoint(Point(0.0, 0.5));
        tickSprite->setPositionX(SCALED_VALUE(27.0f));
        tickSprite->setScale(0.5);
        tickSprite->setPositionY(colorLayer->getContentSize().height/2);
        tickSprite->setTag(TAG_TICK_ICON);
        tickSprite->setUserData(messageRequest);
        
        LayerColor* photoLayer = LayerColor::create(Color4B(255, 255, 255, 255),
                                                        SCALED_VALUE(00.0f),
                                                        SCALED_VALUE(100.0f));
        photoLayer->setPosition(SCALED_VALUE(105.0f), SCALED_VALUE(12.0f));
        cell->addChild(photoLayer);
        Sprite* userPhotoSprite = NULL;
        
        if (toRequestForPhoto)
        {
            userPhotoSprite = Sprite::create();
            userPhotoSprite->setContentSize(Size(SCALED_VALUE(0.0f), SCALED_VALUE(85.0f)));
        }
        else
        {
            userPhotoSprite = Sprite::create(myFriend->getPhotoPath());
        }
        
        cell->addChild(userPhotoSprite);
        userPhotoSprite->setAnchorPoint(Point(0.0f, 0.0));
        userPhotoSprite->cocos2d::CCNode::setPosition(SCALED_VALUE(47.0f), SCALED_VALUE(18.0f));
        userPhotoSprite->setTag(TAG_PHOTO);
        
        // Set the message text.
        auto message = LabelTTF::create(messageToDisplay.c_str(), "Arial", SCALED_VALUE(20.0f));
        message->setAnchorPoint(Point(0, 1.0));
        message->setHorizontalAlignment(TextHAlignment::LEFT);
        message->setDimensions(Size(colorLayer->getContentSize().width * 0.5, colorLayer->getContentSize().height));
        message->setPosition(Point(photoLayer->getContentSize().width + photoLayer->getPositionX() + SCALED_VALUE(20.0f),
                                 colorLayer->getContentSize().height/2));
        cell->addChild(message);
        message->setTag(TAG_MESSAGE);
        
        // Set the title text
        CCLOG("Message title = %s", messageTitle.c_str());
        
        auto titleLabel = LabelTTF::create(messageTitle.c_str(), "Arial", SCALED_VALUE(30.0f));
        titleLabel->setAnchorPoint(Point(0, 0.0));
        titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
        //titleLabel->setDimensions(Size(colorLayer->getContentSize().width * 0.5, colorLayer->getContentSize().height));
        titleLabel->setPosition(Point(message->getPositionX(),
                                 colorLayer->getContentSize().height/2));
        cell->addChild(titleLabel);
        titleLabel->setTag(TAG_TTTLE);
        
        Sprite* consumedIcon = Sprite::createWithSpriteFrameName(getRequetIconFrameName(messageRequest).c_str());
        consumedIcon->setAnchorPoint(Point(1.0, 0.5));
        cell->addChild(consumedIcon);
        consumedIcon->setPosition(Point(colorLayer->getContentSize().width - SCALED_VALUE(20.0f),
                                      colorLayer->getContentSize().height/2));
        consumedIcon->setTag(TAG_ITEM_ICON);
        
    }
    else
    {
        //Sprite* cellProfilePic = (Sprite*)cell->getChildByTag(TAG_PHOTO);
        
        if (toRequestForPhoto == false)
        {
            //cellProfilePic->setTexture(Sprite::create(myFriend->getPhotoPath())->getTexture());
        }
        
        Sprite *tick = (Sprite*)cell->getChildByTag(TAG_TICK_ICON);
        if (tick)
            
            
        {
            if (_selectedRequestItems.find(messageRequest->getRequestID()) != _selectedRequestItems.end())
            {
                tick->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("row_selected"));
            }
            else
            {
                tick->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("row_not_selected"));
            }

        }
        
        // Update the title Label.
        auto titleLabel = (LabelTTF*)cell->getChildByTag(TAG_TTTLE);
        if (titleLabel)
        {
            titleLabel->setString(messageTitle.c_str());
        }

        // Update the message Label.
        auto messageLabel = (LabelTTF*)cell->getChildByTag(TAG_MESSAGE);
        if (messageLabel)
        {
            messageLabel->setString(messageToDisplay.c_str());
        }
        
        // Update the message Label.
        Sprite *itemIcon = (Sprite*)cell->getChildByTag(TAG_ITEM_ICON);
        if (itemIcon)
        {
            itemIcon->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(getRequetIconFrameName(messageRequest).c_str()));
        }

        
    }
    
    return cell;
}

std::string RequestListScene::getRequetIconFrameName(EziFBIncomingRequest *request)
{
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = request->getRequestType();
    std::string frameName = "invite";
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            frameName = "invite";
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            frameName = "gift";
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            frameName = "challenge";
            break;
            
        default:
            frameName = "invite";
            break;
    }
    
    return frameName;
}

std::string RequestListScene::getFancyTitle(EziFBIncomingRequest *request)
{
    // Create the fancy test.
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = request->getRequestType();
    
    std::string messageToDisplay = "";
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            messageToDisplay.append("You got an invitation!");
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            messageToDisplay.append("You got a gift!");
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            messageToDisplay.append("You got a challenge!");
            break;
            
        default:
            messageToDisplay.append("Unknown message");
            break;
    }
    
    return messageToDisplay;
}


std::string RequestListScene::getFancyMessage(EziFBIncomingRequest *request)
{
    // Create the fancy test.
    EziSocialWrapperNS::FB_REQUEST::TYPE requestType = request->getRequestType();
    EziFacebookFriend* sender                        = request->getSender();
    const char* senderName = sender->getName().c_str();
    
    std::string messageToDisplay = "";
    
    switch (requestType)
    {
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE:
            messageToDisplay.append(senderName).append(" has invited you to play with him.");
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT:
            messageToDisplay.append(senderName).append(" has gave you a life.");
            break;
            
        case EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE:
            messageToDisplay.append(senderName).append(" has challenged your skills.");
            break;
            
        default:
            messageToDisplay.append("Unknown message");
            break;
    }
    
    return messageToDisplay;
}

ssize_t RequestListScene::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _fbMessagesList.size();
}

//-------------------------------------
#pragma mark - eziSocial delegates
//-------------------------------------

/*
void RequestListScene::fbFriendsCallback(int responseCode, const char *responseMessage, cocos2d::CCArray *friends)
{
    MessageBox(responseMessage, "Friends Callback");
    
    if (_friendList)
    {
        _friendList->release();
        _friendList = NULL;
    }
    
    _friendList = CCArray::createWithArray(friends);
    _friendList->retain();
    
    if (_friendDictionary)
    {
        _friendDictionary->release();
    }
    
    _friendDictionary = NULL;
    _friendDictionary = CCDictionary::create();
    
    int totalFriends = _friendList->count()-1;
    _downloadCount = 0;
    
    for (int i=totalFriends; i >= 0; i--)
    {
        EziFacebookFriend* myFriend = (EziFacebookFriend*)_friendList->objectAtIndex(i);
        _friendDictionary->setObject(myFriend, myFriend->getFBID());
        EziSocialObject::sharedObject()->getProfilePicForID(this, myFriend->getFBID(), SCALED_VALUE(85.0f), SCALED_VALUE(85.0f), false);
    }
    
    
    _friendDictionary->retain();
    _tableView->reloadData();
}
*/

void RequestListScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
    if (_friendDictionary.size() == 0)
    {
        return;
    }
    
    EziFacebookFriend* myFriend = (EziFacebookFriend*)_friendDictionary.at(fbID);
    
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
    if (_downloadCount == _fbMessagesList.size())
    {
        // this is the last we fetched. We can refresh the table view now.
        CCLOG("Download completed...");
        _tableView->reloadData();
    }
    
}