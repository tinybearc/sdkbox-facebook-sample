#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PluginFacebook/PluginFacebook.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace sdkbox;


static void checkFaceBookStatus()
{
    CCLOG("##FB> permission list: ");
    for (auto& permission : PluginFacebook::getPermissionList())
    {
        CCLOG("##FB>> permission %s", permission.data());
    }
    CCLOG("##FB> access token: %s", PluginFacebook::getAccessToken().c_str());
    CCLOG("##FB> user id: %s", PluginFacebook::getUserID().c_str());
    CCLOG("##FB> FBSDK version: %s", PluginFacebook::getSDKVersion().c_str());
}


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    FileUtils::getInstance()->addSearchPath("fonts");
    
    _captureFilename = "";
    Size size = Director::getInstance()->getWinSize();
    
    PluginFacebook::setListener(this);
    
    // ui
    {
        std::string defaultFont("arial.ttf");
        int defaultFontSize = 32;
        
        std::string loginStat;
        if (PluginFacebook::isLoggedIn())
        {
            loginStat = "Logout";
        }
        else
        {
            loginStat = "Login";
        }
        
        _loginItem = MenuItemLabel::create(Label::createWithTTF(loginStat, defaultFont, defaultFontSize),
                                               CC_CALLBACK_1(HelloWorld::onLoginClick, this));
        
        auto checkStatusItem = MenuItemLabel::create(Label::createWithTTF("check status", defaultFont, defaultFontSize),
                                                     CC_CALLBACK_1(HelloWorld::onCheckStatus, this));
        
        
        auto myInfoItem = MenuItemLabel::create(Label::createWithTTF("requestReadPermission", defaultFont, defaultFontSize),
                                                CC_CALLBACK_1(HelloWorld::onRequestReadPermission, this));
        
        auto myFriendsItem = MenuItemLabel::create(Label::createWithTTF("requestPublishPermission", defaultFont, defaultFontSize),
                                                   CC_CALLBACK_1(HelloWorld::onRequestPublishPermission, this));
        
        
        auto captureScreenItem = MenuItemLabel::create(Label::createWithTTF("capture screen", defaultFont, defaultFontSize),
                                                       CC_CALLBACK_1(HelloWorld::onCaptureScreen, this));
        
        
        auto shareLink = MenuItemLabel::create(Label::createWithTTF("share link", defaultFont, defaultFontSize),
                                               CC_CALLBACK_1(HelloWorld::onShareLink, this));
        auto sharePhoto = MenuItemLabel::create(Label::createWithTTF("share photo", defaultFont, defaultFontSize),
                                                CC_CALLBACK_1(HelloWorld::onSharePhoto, this));
        
        auto dialogLink = MenuItemLabel::create(Label::createWithTTF("dialog link", defaultFont, defaultFontSize),
                                                CC_CALLBACK_1(HelloWorld::onDialogLink, this));
        
        auto dialogPhoto = MenuItemLabel::create(Label::createWithTTF("dialog photo(on device)", defaultFont, defaultFontSize),
                                                 CC_CALLBACK_1(HelloWorld::onDialogPhoto, this));
        
        auto menu = Menu::create(_loginItem, checkStatusItem,
                                 myInfoItem, myFriendsItem,
                                 captureScreenItem,
                                 shareLink,sharePhoto,
                                 dialogLink,dialogPhoto,
                                 MenuItemLabel::create(Label::createWithTTF("my info", defaultFont, defaultFontSize),
                                                       CC_CALLBACK_1(HelloWorld::onGetMyInfo, this)),
                                 MenuItemLabel::create(Label::createWithTTF("my friends", defaultFont, defaultFontSize),
                                                       CC_CALLBACK_1(HelloWorld::onGetMyFriends, this)),
                                 NULL);
        menu->alignItemsVerticallyWithPadding(5);
        menu->setPosition(Vec2(size.width/2, size.height/2));
        addChild(menu);
        
        Label *userLabel = Label::createWithTTF("user: aydghli_riceberg_1435809241@tfbnw.net", defaultFont, defaultFontSize);
        userLabel->setPosition(Vec2(size.width/2, size.height - defaultFontSize));
        addChild(userLabel);
        Label *pwLabel = Label::createWithTTF("password: 123456", defaultFont, defaultFontSize);
        pwLabel->setPosition(Vec2(size.width/2, size.height - defaultFontSize*2));
        addChild(pwLabel);
    }
    
    return true;
}

void HelloWorld::onLoginClick(cocos2d::Ref *sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
        PluginFacebook::logout();
    }
    else
    {
        PluginFacebook::login();
    }
}

void HelloWorld::onCheckStatus(cocos2d::Ref* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    checkFaceBookStatus();
}
void HelloWorld::onGetMyInfo(cocos2d::Ref* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    PluginFacebook::FBAPIParam params;
    PluginFacebook::api("me", "GET", params, "me");
}
void HelloWorld::onGetMyFriends(cocos2d::Ref* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
//    PluginFacebook::fetchFriends();
}
void HelloWorld::onCaptureScreen(cocos2d::Ref *sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    utils::captureScreen(CC_CALLBACK_2(HelloWorld::afterCaptureScreen, this), "screen.png");
}
void HelloWorld::afterCaptureScreen(bool yes, const std::string &outputFilename)
{
    CCLOG("##FB afterCaptureScreen: %s", outputFilename.c_str());
    if (yes)
    {
        _captureFilename = outputFilename;
    }
}
void HelloWorld::onShareLink(cocos2d::Ref* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    FBShareInfo info;
    info.type  = FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    PluginFacebook::share(info);
}
void HelloWorld::onSharePhoto(cocos2d::Ref* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    if (!_captureFilename.empty() && FileUtils::getInstance()->isFileExist(_captureFilename))
    {
        CCLOG("##FB dialog photo: %s", _captureFilename.c_str());
        
        FBShareInfo info;
        info.type  = FB_PHOTO;
        info.title = "capture screen";
        info.image = _captureFilename;
        PluginFacebook::share(info);
    }
    else
    {
        CCLOG("##FB capture screen first");
    }
}
void HelloWorld::onDialogLink(cocos2d::Ref* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    FBShareInfo info;
    info.type  = FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    PluginFacebook::dialog(info);
}
void HelloWorld::onDialogPhoto(cocos2d::Ref* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    if (!_captureFilename.empty() && FileUtils::getInstance()->isFileExist(_captureFilename))
    {
        CCLOG("dialog photo: %s", _captureFilename.c_str());
        FBShareInfo info;
        info.type  = FB_PHOTO;
        info.title = "capture screen";
        info.image = _captureFilename;
        PluginFacebook::dialog(info);
    }
    else
    {
        CCLOG("##FB capture screen first");
    }
}
void HelloWorld::onRequestReadPermission(cocos2d::Ref *sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    PluginFacebook::requestReadPermissions({FB_PERM_READ_USER_FRIENDS});
}

void HelloWorld::onRequestPublishPermission(cocos2d::Ref *sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    PluginFacebook::requestPublishPermissions({FB_PERM_PUBLISH_POST});
}

/*********************
 * Facebook callbacks
 *********************/
void HelloWorld::onLogin(bool isLogin, const std::string& error)
{
    CCLOG("##FB isLogin: %d, error: %s", isLogin, error.c_str());
    
    if (isLogin)
    {
        _loginItem->setString("Logout");
    }
    
    std::string title = "login ";
    title.append((isLogin ? "success" : "failed"));
    MessageBox(error.c_str(), title.c_str());
}
void HelloWorld::onAPI(const std::string& tag, const std::string& jsonData)
{
    CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
}
void HelloWorld::onSharedSuccess(const std::string& message)
{
    CCLOG("##FB onSharedSuccess:%s", message.c_str());
    
    MessageBox(message.c_str(), "share success");
}
void HelloWorld::onSharedFailed(const std::string& message)
{
    CCLOG("##FB onSharedFailed:%s", message.c_str());
    
    MessageBox(message.c_str(), "share failed");
}
void HelloWorld::onSharedCancel()
{
    CCLOG("##FB onSharedCancel");
    
    MessageBox("", "share cancel");
}
void HelloWorld::onPermission(bool isLogin, const std::string& error)
{
    CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
    
    std::string title = "permission ";
    title.append((isLogin ? "success" : "failed"));
    MessageBox(error.c_str(), title.c_str());
}
void HelloWorld::onFetchFriends(bool ok, const std::string& msg)
{
    CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
    
    const std::vector<sdkbox::FBGraphUser>& friends = PluginFacebook::getFriends();
    for (int i = 0; i < friends.size(); i++)
    {
        const sdkbox::FBGraphUser& user = friends.at(i);
        CCLOG("##FB> -------------------------------");
        CCLOG("##FB>> %s", user.uid.data());
        CCLOG("##FB>> %s", user.firstName.data());
        CCLOG("##FB>> %s", user.lastName.data());
        CCLOG("##FB>> %s", user.name.data());
        CCLOG("##FB>> %s", user.isInstalled ? "app is installed" : "app is not installed");
        CCLOG("##FB");
    }
    
    MessageBox("", "fetch friends");
}
