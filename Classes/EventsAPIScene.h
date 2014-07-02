//
//  EventsAPIScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__EventsAPIScene__
#define __EziSocialPluginDevelopment__EventsAPIScene__

#include "BaseScene.h"

class EventsAPIScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(EventsAPIScene);
    
    virtual void buildScene();
    
private:
    
    bool _storeOpen;
    bool _loggingActiveness;
    
    MenuItemImage* _carButton;
    MenuItemImage* _shieldButton;
    MenuItemImage* _lifeButton;
    MenuItemImage* _storeButton;
    MenuItemImage* _logActiveButton;
    
    void openStore(Ref* sender);
    void buyCar(Ref* sender);
    void buyShield(Ref* sender);
    void buyLife(Ref* sender);
    void logActiveness(Ref* sender);
    void showPopup(Ref* sender);
};


#endif /* defined(__EziSocialPluginDevelopment__EventsAPIScene__) */
