//
//  ScoreAPIScene.cpp
//  EziSocialPluginDevelopment
//
//  Created by Paras Mendiratta on 02/09/13.
//
//

#include "ScoreAPIScene.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"

using namespace cocos2d;
using namespace CocosDenshion;

Scene* ScoreAPIScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    ScoreAPIScene *layer = ScoreAPIScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool ScoreAPIScene::init()
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
void ScoreAPIScene::onEnterTransitionDidFinish()
{
    CCLOG("[ScoreAPIScene]: Enter Transition Finished.");
    EziSocialObject::sharedObject()->setFacebookDelegate(this);
}

void ScoreAPIScene::buildScene()
{
    // Call super build scene.
    BaseScene::buildScene();
    BaseScene::addTipBoard("Do you know that store user score on Facebook? Compare friends score and build a leaderboard in just 1 function call");

    
    SpriteBatchNode* batchNode = SpriteBatchNode::create(PNG_FILE(kRESOURCE_SCORE));
    this->addChild(batchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_FILE(kRESOURCE_SCORE));
    buildHeader(-1, "Score APIs", "Generate interesting stories by updating scores");
    
    MenuItemImage* menuItem = NULL;
    
    menuItem = this->buildAndPlaceTile("btn_reset_score_nrl",      "btn_reset_score_prd",      1, 0);
    menuItem->setCallback(CC_CALLBACK_1(ScoreAPIScene::resetUserScore, this));
    
    menuItem = this->buildAndPlaceTile("btn_submit_score_nrl",     "btn_submit_score_prd",     1, 1);
    menuItem->setCallback(CC_CALLBACK_1(ScoreAPIScene::submitUserScore, this));
    
    //menuItem = this->buildAndPlaceTile("btn_reset_tournament_nrl", "btn_reset_tournament_prd", 1, 1);
    //menuItem->setTarget(this, menu_selector(ScoreAPIScene::resetTournamentScore));
    
    menuItem = this->buildAndPlaceTile("btn_achievement_nrl",      "btn_achievement_prd",      0, 2);
    menuItem->setCallback(CC_CALLBACK_1(ScoreAPIScene::publishAnAchievement, this));
}

void ScoreAPIScene::resetUserScore(Ref* sender)
{
    EziSocialObject::sharedObject()->deleteScore();
}

void ScoreAPIScene::resetTournamentScore(Ref* sender)
{
    //MessageBox("Not implemented yet", <#const char *pszTitle#>)
}

void ScoreAPIScene::submitUserScore(Ref* sender)
{
    int score = rand() % 1000;
    CCLOG("Posting Score = %d", score);
    EziSocialObject::sharedObject()->postScore(score);
}

void ScoreAPIScene::publishAnAchievement(Ref* sender)
{
    EziSocialObject::sharedObject()->postAchievement("https://dl.dropboxusercontent.com/u/37517735/achievementtest.html");
}


#pragma mark - Facebook Callback method

void ScoreAPIScene::fbMessageCallback(int responseCode, const char* responseMessage)
{
    MessageBox(responseMessage, "Score API Response");
}

void ScoreAPIScene::fbAchievementCallback(int responseCode, const char* responseMessage)
{
    MessageBox(responseMessage, "Achievement Submission Response");
}
