/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
// SPDX-License-Identifier: MIT OR LGPL-2.0-or-later
// SPDX-FileCopyrightText: 2010 litl, LLC.
// SPDX-FileCopyrightText: 2020 Philip Chimento <philip.chimento@gmail.com>

#include <config.h>

#include <cairo-features.h>  // for CAIRO_HAS_PDF_SURFACE
#include <cairo.h>

#include <js/GCPolicyAPI.h>
#include <js/TypeDecls.h>

#include "cjs/jsapi-util.h"

#if CAIRO_HAS_PDF_SURFACE
#    include <cairo-pdf.h>

#    include <js/PropertyDescriptor.h>  // for JSPROP_READONLY
#    include <js/PropertySpec.h>
#    include <js/RootingAPI.h>
#    include <jsapi.h>  // for JS_NewObjectWithGivenProto
#    include <jspubtd.h>  // for JSProtoKey

#    include "gjs/jsapi-util-args.h"
#    include "modules/cairo-private.h"

namespace JS {
class CallArgs;
// Avoid static_assert in MSVC builds
template <>
struct GCPolicy<void*> : public IgnoreGCPolicy<void*> {};
}

JSObject* CairoPDFSurface::new_proto(JSContext* cx, JSProtoKey) {
    JS::RootedObject parent_proto(cx, CairoSurface::prototype(cx));
    return JS_NewObjectWithGivenProto(cx, nullptr, parent_proto);
}

cairo_surface_t* CairoPDFSurface::constructor_impl(JSContext* context,
                                                   const JS::CallArgs& argv) {
    GjsAutoChar filename;
    double width, height;
    cairo_surface_t *surface;
    if (!gjs_parse_call_args(context, "PDFSurface", argv, "Fff",
                             "filename", &filename,
                             "width", &width,
                             "height", &height))
        return nullptr;

    surface = cairo_pdf_surface_create(filename, width, height);

    if (!gjs_cairo_check_status(context, cairo_surface_status(surface),
                                "surface"))
        return nullptr;

    return surface;
}

// clang-format off
JSPropertySpec gjs_cairo_pdf_surface_proto_props[] = {
    JS_STRING_SYM_PS(toStringTag, "PDFSurface", JSPROP_READONLY),
    JS_PS_END};
// clang-format on
#else
JSObject* CairoPDFSurface::from_c_ptr(JSContext* context,
                                      cairo_surface_t* surface) {
    gjs_throw(context,
        "could not create PDF surface, recompile cairo and gjs with "
        "PDF support.");
    return nullptr;
}
#endif /* CAIRO_HAS_PDF_SURFACE */
