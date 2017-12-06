/* This file contains code derived from xpcdebug.cpp in Mozilla.  The license
 * for that file follows:
 */
/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Communicator client code, released
 * March 31, 1998.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1999
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   John Bandhauer <jband@netscape.com> (original author)
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include <config.h>
#include <glib.h>
#include <string.h>
#include "context.h"
#include "jsapi-util.h"
#include "jsapi-wrapper.h"

bool
gjs_context_get_frame_info(JSContext                             *context,
                           mozilla::Maybe<JS::MutableHandleValue> stack,
                           mozilla::Maybe<JS::MutableHandleValue> fileName,
                           mozilla::Maybe<JS::MutableHandleValue> lineNumber)
{
    JSAutoRequest ar(context);
    JS::RootedObject global(context, JS::CurrentGlobalOrNull(context)),
        constructor(context);
    JSAutoCompartment ac(context, global);

    JS::RootedId error_id(context, gjs_intern_string_to_id(context, "Error"));
    if (!gjs_object_require_property(context, global, "global object",
                                     error_id, &constructor))
        return false;

    JS::RootedObject err_obj(context, JS_New(context, constructor,
                                             JS::HandleValueArray::empty()));

    if (stack &&
        !gjs_object_get_property(context, err_obj, GJS_STRING_STACK,
                                 stack.ref()))
        return false;

    if (fileName &&
        !gjs_object_get_property(context, err_obj, GJS_STRING_FILENAME,
                                 fileName.ref()))
        return false;

    if (lineNumber &&
        !gjs_object_get_property(context, err_obj, GJS_STRING_LINE_NUMBER,
                                 lineNumber.ref()))
        return false;

    return true;
}

void
gjs_context_print_stack_stderr(GjsContext *context)
{
    JSContext *cx = (JSContext*) gjs_context_get_native_context(context);
    JS::RootedValue v_stack(cx);
    GjsAutoChar stack;

    g_printerr("== Stack trace for context %p ==\n", context);

    /* Stderr is locale encoding, so we use string_to_filename here */
    if (!gjs_context_get_frame_info(cx, mozilla::Some<JS::MutableHandleValue>(&v_stack),
                                    mozilla::Nothing(), mozilla::Nothing()) ||
        !gjs_string_to_filename(cx, v_stack, &stack)) {
        g_printerr("No stack trace available\n");
        return;
    }

    g_printerr("%s\n", stack.get());
}

void
gjs_dumpstack(void)
{
    GList *contexts = gjs_context_get_all();
    GList *iter;

    for (iter = contexts; iter; iter = iter->next) {
        GjsContext *context = (GjsContext*)iter->data;
        gjs_context_print_stack_stderr(context);
        g_object_unref(context);
    }
    g_list_free(contexts);
}
