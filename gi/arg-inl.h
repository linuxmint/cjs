/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
// SPDX-License-Identifier: MIT OR LGPL-2.0-or-later
// SPDX-FileCopyrightText: 2020 Marco Trevisan <marco.trevisan@canonical.com>

#pragma once

#include <config.h>

#include <stdint.h>

#include <cstddef>  // for nullptr_t
#include <limits>
#include <string>  // for to_string
#include <type_traits>

#include <girepository.h>
#include <glib-object.h>  // for GType
#include <glib.h>         // for gboolean
#include <js/TypeDecls.h>  // for HandleValue

#include "gi/js-value-inl.h"
#include "gi/utils-inl.h"
#include "cjs/macros.h"

// GIArgument accessor templates
//
// These are intended to make access to the GIArgument union more type-safe and
// reduce bugs that occur from assigning to one member and reading from another.
// (These bugs often work fine on one processor architecture but crash on
// another.)
//
// gjs_arg_member<T>(GIArgument*) - returns a reference to the appropriate union
//   member that would hold the type T. Rarely used, unless as a pointer to a
//   return location.
// gjs_arg_get<T>(GIArgument*) - returns the value of type T from the
//   appropriate union member.
// gjs_arg_set(GIArgument*, T) - sets the appropriate union member for type T.
// gjs_arg_unset<T>(GIArgument*) - sets the appropriate zero value in the
//   appropriate union member for type T.
// gjs_arg_steal<T>(GIArgument*) - sets the appropriate zero value in the
//   appropriate union member for type T and returns the replaced value.

template <auto GIArgument::*member>
[[nodiscard]] constexpr inline decltype(auto) gjs_arg_member(GIArgument* arg) {
    return (arg->*member);
}

/* The tag is needed to disambiguate types such as gboolean and GType
 * which are in fact typedef's of other generic types.
 * Setting a tag for a type allows to perform proper specialization. */
template <typename T, GITypeTag TAG = GI_TYPE_TAG_VOID>
[[nodiscard]] constexpr inline decltype(auto) gjs_arg_member(GIArgument* arg) {
    if constexpr (TAG == GI_TYPE_TAG_VOID) {
        if constexpr (std::is_same_v<T, bool>)
            return gjs_arg_member<&GIArgument::v_boolean>(arg);
        if constexpr (std::is_same_v<T, int8_t>)
            return gjs_arg_member<&GIArgument::v_int8>(arg);
        if constexpr (std::is_same_v<T, uint8_t>)
            return gjs_arg_member<&GIArgument::v_uint8>(arg);
        if constexpr (std::is_same_v<T, int16_t>)
            return gjs_arg_member<&GIArgument::v_int16>(arg);
        if constexpr (std::is_same_v<T, uint16_t>)
            return gjs_arg_member<&GIArgument::v_uint16>(arg);
        if constexpr (std::is_same_v<T, int32_t>)
            return gjs_arg_member<&GIArgument::v_int32>(arg);
        if constexpr (std::is_same_v<T, uint32_t>)
            return gjs_arg_member<&GIArgument::v_uint32>(arg);
        if constexpr (std::is_same_v<T, int64_t>)
            return gjs_arg_member<&GIArgument::v_int64>(arg);
        if constexpr (std::is_same_v<T, uint64_t>)
            return gjs_arg_member<&GIArgument::v_uint64>(arg);

        // gunichar is stored in v_uint32
        if constexpr (std::is_same_v<T, char32_t>)
            return gjs_arg_member<&GIArgument::v_uint32>(arg);

        if constexpr (std::is_same_v<T, float>)
            return gjs_arg_member<&GIArgument::v_float>(arg);

        if constexpr (std::is_same_v<T, double>)
            return gjs_arg_member<&GIArgument::v_double>(arg);

        if constexpr (std::is_same_v<T, char*>)
            return gjs_arg_member<&GIArgument::v_string>(arg);

        if constexpr (std::is_same_v<T, void*>)
            return gjs_arg_member<&GIArgument::v_pointer>(arg);

        if constexpr (std::is_same_v<T, std::nullptr_t>)
            return gjs_arg_member<&GIArgument::v_pointer>(arg);

        if constexpr (std::is_pointer<T>()) {
            using NonconstPtrT = std::add_pointer_t<
                std::remove_const_t<std::remove_pointer_t<T>>>;
            return reinterpret_cast<NonconstPtrT&>(
                gjs_arg_member<&GIArgument::v_pointer>(arg));
        }
    }

    if constexpr (TAG == GI_TYPE_TAG_BOOLEAN && std::is_same_v<T, gboolean>)
        return gjs_arg_member<&GIArgument::v_boolean>(arg);

    if constexpr (TAG == GI_TYPE_TAG_GTYPE && std::is_same_v<T, GType>) {
        // GType is defined differently on 32-bit vs. 64-bit architectures.
        if constexpr (std::is_same_v<GType, gsize>)
            return gjs_arg_member<&GIArgument::v_size>(arg);
        else if constexpr (std::is_same_v<GType, gulong>)
            return gjs_arg_member<&GIArgument::v_ulong>(arg);
    }

    if constexpr (TAG == GI_TYPE_TAG_INTERFACE && std::is_integral_v<T>) {
        if constexpr (std::is_signed_v<T>)
            return gjs_arg_member<&GIArgument::v_int>(arg);
        else
            return gjs_arg_member<&GIArgument::v_uint>(arg);
    }
}

typedef enum {
    GJS_TYPE_TAG_LONG = 0,
} ExtraTag;

template <typename T, ExtraTag TAG>
[[nodiscard]] constexpr inline decltype(auto) gjs_arg_member(GIArgument* arg) {
    if constexpr (TAG == GJS_TYPE_TAG_LONG &&
                  std::is_same_v<T, long>)  // NOLINT(runtime/int)
        return gjs_arg_member<&GIArgument::v_long>(arg);
    else if constexpr (TAG == GJS_TYPE_TAG_LONG &&
                       std::is_same_v<T, unsigned long>)  // NOLINT(runtime/int)
        return gjs_arg_member<&GIArgument::v_ulong>(arg);
}

template <typename T, GITypeTag TAG = GI_TYPE_TAG_VOID>
constexpr inline void gjs_arg_set(GIArgument* arg, T v) {
    if constexpr (std::is_pointer_v<T>) {
        using NonconstPtrT =
            std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<T>>>;
        gjs_arg_member<NonconstPtrT, TAG>(arg) = const_cast<NonconstPtrT>(v);
    } else {
        if constexpr (std::is_same_v<T, bool> || (std::is_same_v<T, gboolean> &&
                                                  TAG == GI_TYPE_TAG_BOOLEAN))
            v = !!v;

        gjs_arg_member<T, TAG>(arg) = v;
    }
}

template <typename T, ExtraTag TAG>
constexpr inline void gjs_arg_set(GIArgument* arg, T v) {
    gjs_arg_member<T, TAG>(arg) = v;
}

// Store function pointers as void*. It is a requirement of GLib that your
// compiler can do this
template <typename ReturnT, typename... Args>
constexpr inline void gjs_arg_set(GIArgument* arg, ReturnT (*v)(Args...)) {
    gjs_arg_member<void*>(arg) = reinterpret_cast<void*>(v);
}

template <typename T, GITypeTag TAG = GI_TYPE_TAG_VOID>
constexpr inline std::enable_if_t<std::is_integral_v<T>> gjs_arg_set(
    GIArgument* arg, void* v) {
    gjs_arg_set<T, TAG>(arg, gjs_pointer_to_int<T>(v));
}

template <typename T, GITypeTag TAG = GI_TYPE_TAG_VOID>
[[nodiscard]] constexpr inline T gjs_arg_get(GIArgument* arg) {
    if constexpr (std::is_same_v<T, bool> ||
                  (std::is_same_v<T, gboolean> && TAG == GI_TYPE_TAG_BOOLEAN))
        return T(!!gjs_arg_member<T, TAG>(arg));

    return gjs_arg_member<T, TAG>(arg);
}

template <typename T, ExtraTag TAG>
[[nodiscard]] constexpr inline T gjs_arg_get(GIArgument* arg) {
    return gjs_arg_member<T, TAG>(arg);
}

template <typename T, GITypeTag TAG = GI_TYPE_TAG_VOID>
[[nodiscard]] constexpr inline void* gjs_arg_get_as_pointer(GIArgument* arg) {
    return gjs_int_to_pointer(gjs_arg_get<T, TAG>(arg));
}

template <typename T, GITypeTag TAG = GI_TYPE_TAG_VOID>
constexpr inline void gjs_arg_unset(GIArgument* arg) {
    if constexpr (std::is_pointer_v<T>)
        gjs_arg_set<T, TAG>(arg, nullptr);
    else
        gjs_arg_set<T, TAG>(arg, static_cast<T>(0));
}

template <typename T, GITypeTag TAG = GI_TYPE_TAG_VOID>
[[nodiscard]] constexpr inline T gjs_arg_steal(GIArgument* arg) {
    auto val = gjs_arg_get<T, TAG>(arg);
    gjs_arg_unset<T, TAG>(arg);
    return val;
}

// Implementation to store rounded (u)int64_t numbers into double

template <typename BigT>
[[nodiscard]] inline constexpr std::enable_if_t<
    std::is_integral_v<BigT> && (std::numeric_limits<BigT>::max() >
                                 std::numeric_limits<int32_t>::max()),
    double>
gjs_arg_get_maybe_rounded(GIArgument* arg) {
    BigT val = gjs_arg_get<BigT>(arg);

    if (val < Gjs::min_safe_big_number<BigT>() ||
        val > Gjs::max_safe_big_number<BigT>()) {
        g_warning(
            "Value %s cannot be safely stored in a JS Number "
            "and may be rounded",
            std::to_string(val).c_str());
    }

    return static_cast<double>(val);
}

template <typename T, GITypeTag TAG = GI_TYPE_TAG_VOID>
GJS_JSAPI_RETURN_CONVENTION inline bool gjs_arg_set_from_js_value(
    JSContext* cx, const JS::HandleValue& value, GIArgument* arg,
    bool* out_of_range) {
    if constexpr (Gjs::type_has_js_getter<T>())
        return Gjs::js_value_to_c<TAG>(cx, value, &gjs_arg_member<T, TAG>(arg));

    Gjs::JsValueHolder::Relaxed<T> val{};

    if (!Gjs::js_value_to_c_checked<T, TAG>(cx, value, &val, out_of_range))
        return false;

    if (*out_of_range)
        return false;

    gjs_arg_set<T, TAG>(arg, val);

    return true;
}

// A helper function to retrieve array lengths from a GIArgument (letting the
// compiler generate good instructions in case of big endian machines)
[[nodiscard]] constexpr size_t gjs_gi_argument_get_array_length(
    GITypeTag tag, GIArgument* arg) {
    switch (tag) {
        case GI_TYPE_TAG_INT8:
            return gjs_arg_get<int8_t>(arg);
        case GI_TYPE_TAG_UINT8:
            return gjs_arg_get<uint8_t>(arg);
        case GI_TYPE_TAG_INT16:
            return gjs_arg_get<int16_t>(arg);
        case GI_TYPE_TAG_UINT16:
            return gjs_arg_get<uint16_t>(arg);
        case GI_TYPE_TAG_INT32:
            return gjs_arg_get<int32_t>(arg);
        case GI_TYPE_TAG_UINT32:
            return gjs_arg_get<uint32_t>(arg);
        case GI_TYPE_TAG_INT64:
            return gjs_arg_get<int64_t>(arg);
        case GI_TYPE_TAG_UINT64:
            return gjs_arg_get<uint64_t>(arg);
        default:
            g_assert_not_reached();
    }
}
