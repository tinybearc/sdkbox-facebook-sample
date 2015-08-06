#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void onLogin(cocos2d::Ref* sender);
    void onLogout(cocos2d::Ref* sender);
    void onCheckStatus(cocos2d::Ref* sender);
    void onGetMyInfo(cocos2d::Ref* sender);
    void onGetMyFriends(cocos2d::Ref* sender);
    void onCaptureScreen(cocos2d::Ref* sender);
    void onShareLink(cocos2d::Ref* sender);
    void onSharePhoto(cocos2d::Ref* sender);
    void onDialogLink(cocos2d::Ref* sender);
    void onDialogPhoto(cocos2d::Ref* sender);
    void onRequestReadPermission(cocos2d::Ref* sender);
    void onRequestPublishPermission(cocos2d::Ref* sender);
    
private:
    void afterCaptureScreen(bool, const std::string& outputFilename);
    std::string _captureFilename;
};

#endif // __HELLOWORLD_SCENE_H__
