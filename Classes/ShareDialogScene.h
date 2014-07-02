//
//  ShareDialogScene.h
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#ifndef __EziSocialPluginDevelopment__ShareDialogScene__
#define __EziSocialPluginDevelopment__ShareDialogScene__

#include "BaseScene.h"

class ShareDialogScene : public BaseScene
{
public:
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(ShareDialogScene);
    
    virtual void buildScene();
    
    virtual void fbShareDialogCallback(int responseCode, const char* responseMessage);
    virtual void fbPageLikeCallback(int responseCode, const char* responseMessage);
    
private:
    
    void publishMessage(Ref* sender);
    void publishLink(Ref* sender);
    void publishOpenGraphStory(Ref* sender);
    void checkIfFacebookAppInstalled(Ref* sender);
    void openFacebookPage(Ref* sender);
    void checkIfUserLikeMyPage(Ref* sender);
    
};


#endif /* defined(__EziSocialPluginDevelopment__ShareDialogScene__) */
