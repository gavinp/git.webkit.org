/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <glib-object.h>
#include "config.h"

#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>
#include "DOMObjectCache.h"
#include "ExceptionCode.h"
#include "JSMainThreadExecState.h"
#include "Node.h"
#include "TestInterface.h"
#include "TestObj.h"
#include "TestSupplemental.h"
#include "WebKitDOMBinding.h"
#include "gobject/ConvertToUTF8String.h"
#include "webkit/WebKitDOMNode.h"
#include "webkit/WebKitDOMNodePrivate.h"
#include "webkit/WebKitDOMTestInterface.h"
#include "webkit/WebKitDOMTestInterfacePrivate.h"
#include "webkit/WebKitDOMTestObj.h"
#include "webkit/WebKitDOMTestObjPrivate.h"
#include "webkitdefines.h"
#include "webkitglobalsprivate.h"
#include "webkitmarshal.h"

#if ENABLE(Condition1) || ENABLE(Condition2)

namespace WebKit {

WebKitDOMTestInterface* kit(WebCore::TestInterface* obj)
{
    g_return_val_if_fail(obj, 0);

    if (gpointer ret = DOMObjectCache::get(obj))
        return static_cast<WebKitDOMTestInterface*>(ret);

    return static_cast<WebKitDOMTestInterface*>(DOMObjectCache::put(obj, WebKit::wrapTestInterface(obj)));
}

WebCore::TestInterface* core(WebKitDOMTestInterface* request)
{
    g_return_val_if_fail(request, 0);

    WebCore::TestInterface* coreObject = static_cast<WebCore::TestInterface*>(WEBKIT_DOM_OBJECT(request)->coreObject);
    g_return_val_if_fail(coreObject, 0);

    return coreObject;
}

WebKitDOMTestInterface* wrapTestInterface(WebCore::TestInterface* coreObject)
{
    g_return_val_if_fail(coreObject, 0);

    /* We call ref() rather than using a C++ smart pointer because we can't store a C++ object
     * in a C-allocated GObject structure.  See the finalize() code for the
     * matching deref().
     */
    coreObject->ref();

    return  WEBKIT_DOM_TEST_INTERFACE(g_object_new(WEBKIT_TYPE_DOM_TEST_INTERFACE,
                                               "core-object", coreObject, NULL));
}

} // namespace WebKit

#endif // ENABLE(Condition1) || ENABLE(Condition2)

G_DEFINE_TYPE(WebKitDOMTestInterface, webkit_dom_test_interface, WEBKIT_TYPE_DOM_OBJECT)

enum {
    PROP_0,
#if ENABLE(Condition11) || ENABLE(Condition12)
    PROP_SUPPLEMENTAL_STR1,
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#if ENABLE(Condition11) || ENABLE(Condition12)
    PROP_SUPPLEMENTAL_STR2,
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#if ENABLE(Condition11) || ENABLE(Condition12)
    PROP_SUPPLEMENTAL_NODE,
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
};

static void webkit_dom_test_interface_finalize(GObject* object)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebKitDOMObject* dom_object = WEBKIT_DOM_OBJECT(object);
    
    if (dom_object->coreObject) {
        WebCore::TestInterface* coreObject = static_cast<WebCore::TestInterface *>(dom_object->coreObject);

        WebKit::DOMObjectCache::forget(coreObject);
        coreObject->deref();

        dom_object->coreObject = NULL;
    }
#endif // ENABLE(Condition1) || ENABLE(Condition2)

    G_OBJECT_CLASS(webkit_dom_test_interface_parent_class)->finalize(object);
}

static void webkit_dom_test_interface_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
    WebKitDOMTestInterface* self = WEBKIT_DOM_TEST_INTERFACE(object);
    WebCore::TestInterface* coreSelf = WebKit::core(self);
    switch (prop_id) {
#if ENABLE(Condition11) || ENABLE(Condition12)
    case PROP_SUPPLEMENTAL_STR2:
    {
        WebCore::TestSupplemental::setSupplementalStr2(coreSelf, WTF::String::fromUTF8(g_value_get_string(value)));
        break;
    }
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}


static void webkit_dom_test_interface_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
    WebKitDOMTestInterface* self = WEBKIT_DOM_TEST_INTERFACE(object);
    WebCore::TestInterface* coreSelf = WebKit::core(self);
    switch (prop_id) {
#if ENABLE(Condition11) || ENABLE(Condition12)
    case PROP_SUPPLEMENTAL_STR1:
    {
        g_value_take_string(value, convertToUTF8String(WebCore::TestSupplemental::supplementalStr1(coreSelf)));
        break;
    }
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#if ENABLE(Condition11) || ENABLE(Condition12)
    case PROP_SUPPLEMENTAL_STR2:
    {
        g_value_take_string(value, convertToUTF8String(WebCore::TestSupplemental::supplementalStr2(coreSelf)));
        break;
    }
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#if ENABLE(Condition11) || ENABLE(Condition12)
    case PROP_SUPPLEMENTAL_NODE:
    {
        RefPtr<WebCore::Node> ptr = WebCore::TestSupplemental::supplementalNode(coreSelf);
        g_value_set_object(value, WebKit::kit(ptr.get()));
        break;
    }
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}


static void webkit_dom_test_interface_constructed(GObject* object)
{

    if (G_OBJECT_CLASS(webkit_dom_test_interface_parent_class)->constructed)
        G_OBJECT_CLASS(webkit_dom_test_interface_parent_class)->constructed(object);
}

static void webkit_dom_test_interface_class_init(WebKitDOMTestInterfaceClass* requestClass)
{
    GObjectClass *gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->finalize = webkit_dom_test_interface_finalize;
    gobjectClass->set_property = webkit_dom_test_interface_set_property;
    gobjectClass->get_property = webkit_dom_test_interface_get_property;
    gobjectClass->constructed = webkit_dom_test_interface_constructed;

#if ENABLE(Condition11) || ENABLE(Condition12)
    g_object_class_install_property(gobjectClass,
                                    PROP_SUPPLEMENTAL_STR1,
                                    g_param_spec_string("supplemental-str1", /* name */
                                                           "test_interface_supplemental-str1", /* short description */
                                                           "read-only  gchar* TestInterface.supplemental-str1", /* longer - could do with some extra doc stuff here */
                                                           "", /* default */
                                                           WEBKIT_PARAM_READABLE));
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_object_class_install_property(gobjectClass,
                                    PROP_SUPPLEMENTAL_STR2,
                                    g_param_spec_string("supplemental-str2", /* name */
                                                           "test_interface_supplemental-str2", /* short description */
                                                           "read-write  gchar* TestInterface.supplemental-str2", /* longer - could do with some extra doc stuff here */
                                                           "", /* default */
                                                           WEBKIT_PARAM_READWRITE));
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_object_class_install_property(gobjectClass,
                                    PROP_SUPPLEMENTAL_NODE,
                                    g_param_spec_object("supplemental-node", /* name */
                                                           "test_interface_supplemental-node", /* short description */
                                                           "read-write  WebKitDOMNode* TestInterface.supplemental-node", /* longer - could do with some extra doc stuff here */
                                                           WEBKIT_TYPE_DOM_NODE, /* gobject type */
                                                           WEBKIT_PARAM_READWRITE));
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */


}

static void webkit_dom_test_interface_init(WebKitDOMTestInterface* request)
{
}

void
webkit_dom_test_interface_supplemental_method1(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_return_if_fail(self);
    WebCore::JSMainThreadNullState state;
    WebCore::TestInterface * item = WebKit::core(self);
    WebCore::TestSupplemental::supplementalMethod1(item);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMTestObj*
webkit_dom_test_interface_supplemental_method2(WebKitDOMTestInterface* self, const gchar* str_arg, WebKitDOMTestObj* obj_arg, GError **error)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestInterface * item = WebKit::core(self);
    g_return_val_if_fail(str_arg, 0);
    g_return_val_if_fail(obj_arg, 0);
    WTF::String converted_str_arg = WTF::String::fromUTF8(str_arg);
    WebCore::TestObj * converted_obj_arg = NULL;
    if (obj_arg != NULL) {
        converted_obj_arg = WebKit::core(obj_arg);
        g_return_val_if_fail(converted_obj_arg, 0);
    }
    WebCore::ExceptionCode ec = 0;
    PassRefPtr<WebCore::TestObj> g_res = WTF::getPtr(WebCore::TestSupplemental::supplementalMethod2(item, converted_str_arg, converted_obj_arg, ec));
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
    WebKitDOMTestObj* res = WebKit::kit(g_res.get());
    return res;
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
    return NULL;
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return NULL;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void
webkit_dom_test_interface_supplemental_method4(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_return_if_fail(self);
    WebCore::JSMainThreadNullState state;
    WebCore::TestInterface * item = WebKit::core(self);
    WebCore::TestSupplemental::supplementalMethod4(item);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

gchar*
webkit_dom_test_interface_get_supplemental_str1(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestInterface * item = WebKit::core(self);
    gchar* res = convertToUTF8String(WebCore::TestSupplemental::supplementalStr1(item));
    return res;
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
    return NULL;
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return NULL;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

gchar*
webkit_dom_test_interface_get_supplemental_str2(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestInterface * item = WebKit::core(self);
    gchar* res = convertToUTF8String(WebCore::TestSupplemental::supplementalStr2(item));
    return res;
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
    return NULL;
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return NULL;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void
webkit_dom_test_interface_set_supplemental_str2(WebKitDOMTestInterface* self, const gchar* value)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_return_if_fail(self);
    WebCore::JSMainThreadNullState state;
    WebCore::TestInterface * item = WebKit::core(self);
    g_return_if_fail(value);
    WTF::String converted_value = WTF::String::fromUTF8(value);
    WebCore::TestSupplemental::setSupplementalStr2(item, converted_value);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMNode*
webkit_dom_test_interface_get_supplemental_node(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestInterface * item = WebKit::core(self);
    PassRefPtr<WebCore::Node> g_res = WTF::getPtr(WebCore::TestSupplemental::supplementalNode(item));
    WebKitDOMNode* res = WebKit::kit(g_res.get());
    return res;
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
    return NULL;
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return NULL;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void
webkit_dom_test_interface_set_supplemental_node(WebKitDOMTestInterface* self, WebKitDOMNode* value)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    g_return_if_fail(self);
    WebCore::JSMainThreadNullState state;
    WebCore::TestInterface * item = WebKit::core(self);
    g_return_if_fail(value);
    WebCore::Node * converted_value = NULL;
    if (value != NULL) {
        converted_value = WebKit::core(value);
        g_return_if_fail(converted_value);
    }
    WebCore::TestSupplemental::setSupplementalNode(item, converted_value);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

