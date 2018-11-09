/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
/*
 * Copyright (c) 2008  litl, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __GJS_PROXYUTILS_H__
#define __GJS_PROXYUTILS_H__

#include "cjs/jsapi-util.h"
#include "cjs/macros.h"

G_BEGIN_DECLS

GJS_JSAPI_RETURN_CONVENTION
bool _gjs_proxy_to_string_func(JSContext             *context,
                               JSObject              *this_obj,
                               const char            *objtype,
                               GIBaseInfo            *info,
                               GType                  gtype,
                               gpointer               native_address,
                               JS::MutableHandleValue ret);

bool _gjs_proxy_throw_nonexistent_field(JSContext  *cx,
                                        GType       gtype,
                                        const char *field_name);

bool _gjs_proxy_throw_readonly_field(JSContext  *cx,
                                     GType       gtype,
                                     const char *field_name);

G_END_DECLS

#endif  /* __GJS_OBJECT_H__ */
