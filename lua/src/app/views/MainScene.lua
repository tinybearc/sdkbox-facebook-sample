
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

-- MainScene.RESOURCE_FILENAME = "MainScene.csb"

function MainScene:onCreate()
    local fb = sdkbox.PluginFacebook
    fb:init()
    fb:setListener(function(event)
        dump(event)
        local funcname = event.name
        if funcname == "onLogin" then
        end
    end)

	cc.MenuItemFont:setFontName("Arial")
    cc.Menu:create(
                   cc.MenuItemFont:create("login"):onClicked(function()
                        fb:login()
                    end),
                   cc.MenuItemFont:create("logout"):onClicked(function()
                        fb:logout()
                    end),
                   cc.MenuItemFont:create("check"):onClicked(function()
                        print("==============")
                        print("isLogin: ", fb:isLoggedIn())
                        print("userId: ", fb:getUserID())
                        print("permissions: ")
                        for _,v in pairs(fb:getPermissionList()) do
                            print("===> ", v)
                        end
                        print("==============")
                    end),
                   cc.MenuItemFont:create("read perm"):onClicked(function()
                        fb:requestReadPermissions({"public_profile", "email"})
                    end),
                   cc.MenuItemFont:create("write perm"):onClicked(function()
                        fb:requestPublishPermissions({"publish_actions"})
                    end),
                   cc.MenuItemFont:create("share link"):onClicked(function()
                        local info = {}
                        info.type  = "link"
                        info.link  = "http://www.cocos2d-x.org"
                        info.title = "cocos2d-x"
                        info.text  = "Best Game Engine"
                        info.image = "http://cocos2d-x.org/images/logo.png"
                        fb:share(info)
                    end),
                   cc.MenuItemFont:create("dialog link"):onClicked(function()
                        local info = {}
                        info.type  = "link"
                        info.link  = "http://www.cocos2d-x.org"
                        info.title = "cocos2d-x"
                        info.text  = "Best Game Engine"
                        info.image = "http://cocos2d-x.org/images/logo.png"
                        fb:dialog(info)
                    end),
                   cc.MenuItemFont:create("invite"):onClicked(function()
                        fb:inviteFriends(
                            "https://fb.me/322164761287181",
                            "http://www.cocos2d-x.org/attachments/801/cocos2dx_portrait.png")
                    end)

                   )
        :move(display.cx, display.cy)
        :addTo(self)
        :alignItemsVerticallyWithPadding(20)
end

return MainScene
