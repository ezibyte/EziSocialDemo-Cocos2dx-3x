//
//  FriendListScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 05/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__FriendListScene__
#define __EziSocialPluginDevelopment__FriendListScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "EziSocialDelegate.h"


USING_NS_CC;
USING_NS_CC_EXT;

enum FRIEND_CIRCLE {
    ALL_FRIENDS      = 0,
    INSTALLED_ONLY   = 1,
    NON_PLAYING_ONLY = 2,
    MY_DEVICE_ONLY   = 3
    };

class FriendListScene :    public Layer, public TableViewDataSource, public TableViewDelegate, public EziFacebookDelegate

{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene(FRIEND_CIRCLE friendCircle);
    
    // implement the "static node()" method manually
    CREATE_FUNC(FriendListScene);

    void setSelectedFriendCircle(FRIEND_CIRCLE selectedFriendCircle);
    
    // TableView Delegate Methods...
    virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {}
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    // Facebook Delegate Methods...
    virtual void fbFriendsCallback(int responseCode, const char* responseMessage, std::vector<EziFacebookFriend*> friends);
    virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
    
    virtual void onKeyBackClicked();
    
    void applyPhoto(Ref* sender, Sprite* photo);
    
private:
    
    FRIEND_CIRCLE _selectedFriendCircle;
    
    std::vector<EziFacebookFriend*> _friendList;
    cocos2d::Map<std::string, EziFacebookFriend*> _friendDictionary;
    //CCDictionary* _friendDictionary;
    TableView* _tableView;
    
    int _downloadCount;

    void useRequest(Node* sender);
    void backButtonPressed(Ref* sender);
    void buildScene();    
};
#endif /* defined(__EziSocialPluginDevelopment__FriendListScene__) */
