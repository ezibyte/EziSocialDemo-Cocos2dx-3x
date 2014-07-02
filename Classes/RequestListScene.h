//
//  RequestListScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 10/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__RequestListScene__
#define __EziSocialPluginDevelopment__RequestListScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "EziSocialDelegate.h"
#include "EziFBIncomingRequest.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum FRIEND_CIRCLE {
    ALL_FRIENDS      = 0,
    INSTALLED_ONLY   = 1,
    NON_PLAYING_ONLY = 2,
    MY_DEVICE_ONLY   = 3
};

class RequestListScene :    public Layer,
public TableViewDataSource,
public TableViewDelegate,
public EziFacebookDelegate

{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(RequestListScene);
    
    // TableView Delegate Methods...
    virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {}
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    // Facebook Delegate Methods...
    //virtual void fbFriendsCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friends);
    virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
    
    virtual void onKeyBackClicked();
    virtual void onExit();
    
private:
    
    std::vector<EziFBIncomingRequest*> _fbMessagesList;
    std::map<std::string, EziFacebookFriend*> _friendDictionary;
    TableView* _tableView;
    
    int _downloadCount;
    
    void useRequest(Node* sender);
    void backButtonPressed(Ref* sender);
    void buildScene();
    void acceptAllCheckboxPressed(Ref* sender);
    void acceptButtonPressed(Ref* sender);
    
    std::string getFancyMessage(EziFBIncomingRequest *request);
    std::string getFancyTitle(EziFBIncomingRequest *request);
    std::string getRequetIconFrameName(EziFBIncomingRequest *request);
    
    MenuItemImage* _allButton;
    bool _allSelected;
    std::unordered_map<std::string, EziFBIncomingRequest*> _selectedRequestItems;
    
};

#endif /* defined(__EziSocialPluginDevelopment__RequestListScene__) */
