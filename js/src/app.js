
var HelloWorldLayer = cc.Layer.extend({
    sprite:null,
    ctor:function () {
        //////////////////////////////
        // 1. super init first
        this._super();
        var size = cc.winSize;

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
            },
          onGetUserInfo : function( graphUser ) {
              cc.log("OnGetUserInfo: ");
              for( var i in graphUser ) {
                  if ( graphUser.hasOwnProperty(i) ) {
                      cc.log("  -> "+i+"="+graphUser[i]);
                  }
              }
          },
          onInviteFriendsResult : function(result, msg ) {
              cc.log("'"+result+"'  '"+msg+"'")
          },
          onRequestInvitableFriends : function( obj ) {
            /**
             * obj is of the form:
             * { 
             *   data : [ {
             *       <requested fields in the call to requestInvitableFriends>
             *     } 
             *    ],
             *   paging : { 
             *     next_url : string,
             *     prev_url : string,
             *     next_cursor : string,
             *     prev_cursor : string
             *   }
             * }
             */
              cc.log("Invitation tokens: "+obj.data.length);
              for( var i=0; i<obj.data.length; i++ ) {
                var v= obj.data[i];
                for( var j in v ) {
                    cc.log( " "+j+"="+v[j]);
                }
              }
              cc.log("Pagination data: "+ (obj.paging ? true : false));
              if ( obj.paging ) {
                  cc.log("  next_url: "+obj.paging.next_url );
                  cc.log("  prev_url: "+obj.paging.prev_url );
                  cc.log("  next_cursor: "+obj.paging.next_cursor );
                  cc.log("  prev_cursor: "+obj.paging.prev_cursor );
              }
          }
        });
        
                                      var btnInvite = new cc.MenuItemFont("Invite", function(){
                                                                          sdkbox.PluginFacebook.inviteFriends('https://play.google.com/store/apps/details?id=com.ideateca.android.ibasket', 'http://www.crusade-fan.com/ju_nen/one/ju_nen_1.gif');
                                                                         }, this);
                                      
                                      
                                      
                                      var btnReqInvitable = new cc.MenuItemFont("RequestInvitableFriends", function(){
                                                                                sdkbox.PluginFacebook.requestInvitableFriends({ 'fields' : 'first_name,id,email' });
                                                                         }, this);
                                      

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

        var menu = new cc.Menu(btnLogin, btnLogout, btnCheck, btnReadPerm, btnWritePerm, btnShareLink, btnDialogLink, btnInvite, btnReqInvitable);
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


        sdkbox.PluginFacebook.init();
                                      
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
