
var HelloWorldLayer = cc.Layer.extend({
    sprite:null,
    ctor:function () {
        //////////////////////////////
        // 1. super init first
        this._super();
        var size = cc.winSize;

        sdkbox.PluginFacebook.init();
        sdkbox.PluginFacebook.setListener({
            onLogin: function(isLogin, msg) {
              if(isLogin){
                cc.log("login successful");
              }
              else {
                cc.log("login failed");
              }
            },
            onAPI: function(tag, data) {
              cc.log("============");
              cc.log(tag);
              cc.log(data);
            },
            onSharedSuccess: function(data) {
              cc.log("share successful");
            },
            onSharedFailed: function(data) {
              cc.log("share failed");
            },
            onSharedCancel: function() {
              cc.log("share canceled");
            },
            onPermission: function(isLogin, msg) {
              if(isLogin) {
                cc.log("request permission successful");
              }
              else {
                cc.log("request permission failed");
              }
            }
        });

        var btnLogin = new cc.MenuItemFont("Login", function(){
          sdkbox.PluginFacebook.login();
        }, this);

        var btnLogout = new cc.MenuItemFont("Logout", function(){
          sdkbox.PluginFacebook.logout();
        }, this);

        var btnCheck = new cc.MenuItemFont("Check", function(){
          cc.log("==============")
          cc.log("isLogin: " + sdkbox.PluginFacebook.isLoggedIn());
          cc.log("userid: " + sdkbox.PluginFacebook.getUserID());
          cc.log("permissions: ");
          var perms = sdkbox.PluginFacebook.getPermissionList();
          for (var i = 0; i < perms.length; i++) {
            cc.log("===> " + perms[i]);
          }
          cc.log("==============")
        }, this);

        var btnReadPerm = new cc.MenuItemFont("Read Perm", function(){
          sdkbox.PluginFacebook.requestReadPermissions(["public_profile", "email"]);
        }, this);

        var btnWritePerm = new cc.MenuItemFont("Write Perm", function(){
          sdkbox.PluginFacebook.requestPublishPermissions(["publish_actions"]);
        }, this);

        var btnShareLink = new cc.MenuItemFont("Share Link", function(){
          var info = new Object();
          info.type  = "link";
          info.link  = "http://www.cocos2d-x.org";
          info.title = "cocos2d-x";
          info.text  = "Best Game Engine";
          info.image = "http://cocos2d-x.org/images/logo.png";
          sdkbox.PluginFacebook.share(info);
        }, this);

        var btnDialogLink = new cc.MenuItemFont("Dialog Link", function(){
          var info = new Object();
          info.type  = "link";
          info.link  = "http://www.cocos2d-x.org";
          info.title = "cocos2d-x";
          info.text  = "Best Game Engine";
          info.image = "http://cocos2d-x.org/images/logo.png";
          sdkbox.PluginFacebook.dialog(info);
        }, this);

        var btnInvite = new cc.MenuItemFont("Invite", function () {
          sdkbox.PluginFacebook.inviteFriends(
            "https://fb.me/322164761287181",
            "http://www.cocos2d-x.org/attachments/801/cocos2dx_portrait.png");
        }, this);

        var menu = new cc.Menu(btnLogin, btnLogout, btnCheck, btnReadPerm, btnWritePerm, btnShareLink, btnDialogLink, btnInvite);
        menu.x = size.width/2;
        menu.y = size.height/2;
        menu.alignItemsVerticallyWithPadding(5);
        this.addChild(menu);

        // add a "close" icon to exit the progress. it's an autorelease object
        var closeItem = new cc.MenuItemImage(
            res.CloseNormal_png,
            res.CloseSelected_png,
            function () {
                cc.log("Menu is clicked!");
            }, this);
        closeItem.attr({
            x: size.width - 20,
            y: 20,
            anchorX: 0.5,
            anchorY: 0.5
        });

        var cl_menu = new cc.Menu(closeItem);
        cl_menu.x = 0;
        cl_menu.y = 0;
        this.addChild(cl_menu, 1);

        return true;
    }
});

var HelloWorldScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new HelloWorldLayer();
        this.addChild(layer);
    }
});
