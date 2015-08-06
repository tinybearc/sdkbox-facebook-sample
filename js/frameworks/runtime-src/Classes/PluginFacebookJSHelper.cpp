
#include "PluginFacebookJSHelper.h"
#include "cocos2d_specifics.hpp"
#include "SDKBoxJSHelper.h"
#include "PluginFacebook/PluginFacebook.h"

static JSContext* s_cx = nullptr;

class FacebookListenerJsHelper : public sdkbox::FacebookListener {

public:
    void setJSDelegate(JSObject* delegate)
    {
        mJsDelegate = delegate;
    }

    JSObject* getJSDelegate()
    {
        return mJsDelegate;
    }

    void onLogin(bool isLogin, const std::string& error)
    {
        std::string name("onLogin");
        jsval dataVal[2];
        dataVal[0] = BOOLEAN_TO_JSVAL(isLogin);
        dataVal[1] = c_string_to_jsval(s_cx, error.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    void onSharedSuccess(const std::string& message)
    {
        std::string name("onSharedSuccess");
        jsval dataVal[1];
        dataVal[0] = c_string_to_jsval(s_cx, message.c_str());
        invokeDelegate(name, dataVal, 1);
    }
    void onSharedFailed(const std::string& message)
    {
        std::string name("onSharedFailed");
        jsval dataVal[1];
        dataVal[0] = c_string_to_jsval(s_cx, message.c_str());
        invokeDelegate(name, dataVal, 1);
    }
    void onSharedCancel()
    {
        std::string name("onSharedCancel");
        jsval dataVal[0];
        invokeDelegate(name, dataVal, 0);
    }
    void onAPI(const std::string& tag, const std::string& jsonData)
    {
        std::string name("onSharedSuccess");
        jsval dataVal[2];
        dataVal[0] = c_string_to_jsval(s_cx, tag.c_str());
        dataVal[1] = c_string_to_jsval(s_cx, jsonData.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    void onPermission(bool isLogin, const std::string& error)
    {
        std::string name("onPermission");
        jsval dataVal[2];
        dataVal[0] = BOOLEAN_TO_JSVAL(isLogin);
        dataVal[1] = c_string_to_jsval(s_cx, error.c_str());
        invokeDelegate(name, dataVal, 2);
    }


private:
    void invokeDelegate(std::string& fName, jsval dataVal[], int argc) {
        if (!s_cx) {
            return;
        }
        JSContext* cx = s_cx;
        const char* func_name = fName.c_str();

        JS::RootedObject obj(cx, mJsDelegate);
        JSAutoCompartment ac(cx, obj);

#if MOZJS_MAJOR_VERSION >= 31
        bool hasAction;
        JS::RootedValue retval(cx);
        JS::RootedValue func_handle(cx);
#elif MOZJS_MAJOR_VERSION >= 28
        bool hasAction;
        jsval retval;
        JS::RootedValue func_handle(cx);
#else
        JSBool hasAction;
        jsval retval;
        jsval func_handle;
#endif

        if (JS_HasProperty(cx, obj, func_name, &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, func_name, &func_handle)) {
                return;
            }
            if(func_handle == JSVAL_VOID) {
                return;
            }

#if MOZJS_MAJOR_VERSION >= 31
            if (0 == argc) {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::empty(), &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::fromMarkedLocation(argc, dataVal), &retval);
            }
#else
            if (0 == argc) {
                JS_CallFunctionName(cx, obj, func_name, 0, nullptr, &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, argc, dataVal, &retval);
            }
#endif
        }
    }

private:
    JSObject* mJsDelegate;

};

static sdkbox::FBShareInfo map_to_FBShareInfo(const std::map<std::string, std::string>& dict)
{
    sdkbox::FBShareInfo info;

    if (dict.find("title") != dict.end())
    {
        info.title = dict.find("title")->second;
    }
    if (dict.find("link") != dict.end())
    {
        info.link = dict.find("link")->second;
    }
    if (dict.find("text") != dict.end())
    {
        info.text = dict.find("link")->second;
    }
    if (dict.find("image") != dict.end())
    {
        info.image = dict.find("image")->second;
    }
    if (dict.find("type") != dict.end())
    {
        std::string type = dict.find("type")->second;
        if (type.compare("link") == 0)
        {
            info.type = sdkbox::FB_LINK;
        }
        else if (type.compare("photo") == 0)
        {
            info.type = sdkbox::FB_PHOTO;
        }
    }

    return info;
}

#if MOZJS_MAJOR_VERSION >= 28
bool js_PluginFacebookJS_PluginFacebook_setListener(JSContext *cx, uint32_t argc, jsval *vp)
#else
JSBool js_PluginFacebookJS_PluginFacebook_setListener(JSContext *cx, unsigned argc, JS::Value *vp)
#endif
{
    s_cx = cx;
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    if (argc == 1) {

        if (!args.get(0).isObject())
        {
            ok = false;
        }
        JSObject *tmpObj = args.get(0).toObjectOrNull();

        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_setListener : Error processing arguments");
        FacebookListenerJsHelper* lis = new FacebookListenerJsHelper();
        lis->setJSDelegate(tmpObj);
        sdkbox::PluginFacebook::setListener(lis);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginFacebookJS_PluginFacebook_setListener : wrong number of arguments");
    return false;
}


#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_share(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_share : Error processing arguments");
        sdkbox::PluginFacebook::share(map_to_FBShareInfo(arg0));
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginFacebookJS_PluginFacebook_share : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_share(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::share(map_to_FBShareInfo(arg0));
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_dialog(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_dialog : Error processing arguments");
        sdkbox::PluginFacebook::dialog(map_to_FBShareInfo(arg0));
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginFacebookJS_PluginFacebook_dialog : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_dialog(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::dialog(map_to_FBShareInfo(arg0));
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_requestReadPermissions(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= jsval_to_std_vector_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_requestReadPermissions : Error processing arguments");
        sdkbox::PluginFacebook::requestReadPermissions(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginFacebookJS_PluginFacebook_requestReadPermissions : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_requestReadPermissions(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= sdkbox::jsval_to_std_vector_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::requestReadPermissions(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_requestPublishPermissions(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= jsval_to_std_vector_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_requestPublishPermissions : Error processing arguments");
        sdkbox::PluginFacebook::requestPublishPermissions(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginFacebookJS_PluginFacebook_requestPublishPermissions : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_requestPublishPermissions(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= sdkbox::jsval_to_std_vector_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::requestPublishPermissions(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_getPermissionList(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        std::vector<std::string> ret = sdkbox::PluginFacebook::getPermissionList();
        jsval jsret = JSVAL_NULL;
        jsret = std_vector_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_PluginFacebookJS_PluginFacebook_getPermissionList : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_getPermissionList(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc == 0) {
        std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > > ret = sdkbox::PluginFacebook::getPermissionList();
        jsval jsret;
        jsret = sdkbox::std_vector_string_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif

#define REGISTE_FACEBOOK_FUNCTIONS \
JS_DefineFunction(cx, pluginObj, "setListener", js_PluginFacebookJS_PluginFacebook_setListener, 1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "share", js_PluginFacebookJS_PluginFacebook_share, 1, JSPROP_READONLY | JSPROP_PERMANENT);             \
JS_DefineFunction(cx, pluginObj, "dialog", js_PluginFacebookJS_PluginFacebook_dialog, 1, JSPROP_READONLY | JSPROP_PERMANENT);           \
JS_DefineFunction(cx, pluginObj, "requestReadPermissions", js_PluginFacebookJS_PluginFacebook_requestReadPermissions, 1, JSPROP_READONLY | JSPROP_PERMANENT);        \
JS_DefineFunction(cx, pluginObj, "requestPublishPermissions", js_PluginFacebookJS_PluginFacebook_requestPublishPermissions, 1, JSPROP_READONLY | JSPROP_PERMANENT);  \
JS_DefineFunction(cx, pluginObj, "getPermissionList", js_PluginFacebookJS_PluginFacebook_getPermissionList, 0, JSPROP_READONLY | JSPROP_PERMANENT);

#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
void register_PluginFacebookJS_helper(JSContext* cx, JS::HandleObject global) {
    // Get the ns
    JS::RootedObject pluginObj(cx);
    sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginFacebook", &pluginObj);

    REGISTE_FACEBOOK_FUNCTIONS
}
#else
void register_PluginFacebookJS_helper(JSContext* cx, JSObject* obj) {
    // first, try to get the ns
    JS::RootedValue nsval(cx);
    JS::RootedObject pluginObj(cx);

    std::stringstream ss("sdkbox.PluginFacebook");
    std::string sub;
    const char* subChar;

    while(getline(ss, sub, '.')) {
        if(sub.empty())continue;

        subChar = sub.c_str();

        JS_GetProperty(cx, obj, subChar, &nsval);
        if (nsval == JSVAL_VOID) {
            pluginObj = JS_NewObject(cx, NULL, NULL, NULL);
            nsval = OBJECT_TO_JSVAL(pluginObj);
            JS_SetProperty(cx, obj, subChar, nsval);
        } else {
            JS_ValueToObject(cx, nsval, &pluginObj);
        }
        obj = pluginObj;
    }

    REGISTE_FACEBOOK_FUNCTIONS
}
#endif
#elif defined(JS_VERSION)
void register_PluginFacebookJS_helper(JSContext* cx, JSObject* global) {
    jsval pluginVal;
    JSObject* pluginObj;
    pluginVal = sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginFacebook", &pluginObj);

    REGISTE_FACEBOOK_FUNCTIONS
}
#endif

