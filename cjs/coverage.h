/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
/*
 * Copyright © 2014 Endless Mobile, Inc.
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
 *
 * Authored By: Sam Spilsbury <sam@endlessm.com>
 */

#ifndef _GJS_COVERAGE_H
#define _GJS_COVERAGE_H

#include <glib-object.h>
#include <gio/gio.h>

#include "context.h"
#include "cjs/macros.h"

G_BEGIN_DECLS

#define GJS_TYPE_COVERAGE gjs_coverage_get_type()

#define GJS_COVERAGE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), \
     GJS_TYPE_COVERAGE, GjsCoverage))

#define GJS_COVERAGE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), \
     GJS_TYPE_COVERAGE, GjsCoverageClass))

#define GJS_IS_COVERAGE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
     GJS_TYPE_COVERAGE))

#define GJS_IS_COVERAGE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), \
     GJS_TYPE_COVERAGE))

#define GJS_COVERAGE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), \
     GJS_TYPE_COVERAGE, GjsCoverageClass))

typedef struct _GjsCoverage GjsCoverage;
typedef struct _GjsCoverageClass GjsCoverageClass;

struct _GjsCoverageClass {
    GObjectClass parent_class;
};

GType gjs_coverage_get_type(void);

GJS_EXPORT
void gjs_coverage_write_statistics(GjsCoverage *self);

GJS_EXPORT GJS_USE GjsCoverage* gjs_coverage_new(
    const char* const* coverage_prefixes, GjsContext* coverage_context,
    GFile* output_dir);

G_END_DECLS

#endif
