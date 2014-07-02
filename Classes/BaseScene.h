//
//  BaseScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__BaseScene__
#define __EziSocialPluginDevelopment__BaseScene__

#include "cocos2d.h"
#include "EziSocialDefinition.h"
#include "EziSocialObject.h"

using namespace cocos2d;

class BaseScene :   public cocos2d::Layer,
                    public EziFacebookDelegate,
                    public EziEmailDelegate,
                    public EziTwitterDelegate

{
public:
    
protected:
    Menu*     _homeMenu;
    LabelTTF* _titleLabel;
    LabelTTF* _descriptionLabel;

    MenuItemImage* buildAndPlaceTile(const char* normalFrameName, const char* selectedFrameName, int tilePosX, int tilePosY);
    void buildHeader(int highlightIndex, const char* title, const char* description);
    void hideNavigationMenu(bool toHide);
    void hideNavigationForwardButton(bool toHide);
    void hideNavigationBackButton(bool toHide);
    
    virtual void buildScene();
    virtual void addNavigationButtons();
    virtual void addTipBoard(const char* tip);
    
    virtual void backKeyAction();
    virtual void forwardKeyAction();
    
    virtual void onExit();
    
    // EziSocial Delegate Methods
    virtual void fbFriendsCallback(int responseCode, const char* responseMessage, std::vector<EziFacebookFriend *> friends);
    virtual void fbSessionCallback(int responseCode, const char* responseMessage);
    virtual void fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests);
    virtual void mailCallback(int responseCode);
    virtual void twitterCallback(int responseCode);

    void loadHomeScene();
    void loadFBActionScene();
    void loadHelpScene();

    virtual void onKeyBackClicked();

    std::vector<std::string> getRandomFriends(int howMany);
    
private:
    
    Menu*     _tileMenu;
    Menu*     _navigationMenu;

    MenuItemImage* _forwardButton;
    MenuItemImage* _backButton;
    
    // Scene Transitions...
    void buildTileMenuSkelton();
    
    int  _currentMenuIndex;
    int  _randomFriendCount;
    
};

#endif /* defined(__EziSocialPluginDevelopment__BaseScene__) */
