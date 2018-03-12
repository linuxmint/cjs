gjs_public_headers =		\
	cjs/context.h		\
	cjs/coverage.h		\
	cjs/gjs.h		\
	cjs/macros.h		\
	cjs/profiler.h		\
	util/error.h		\
	$(NULL)

# For historical reasons, some files live in gi/
# Some headers in the following list were formerly
# public

gjs_srcs =				\
	gi/arg.cpp			\
	gi/arg.h			\
	gi/boxed.cpp			\
	gi/boxed.h			\
	gi/closure.cpp			\
	gi/closure.h			\
	gi/enumeration.cpp		\
	gi/enumeration.h		\
	gi/foreign.cpp			\
	gi/foreign.h			\
	gi/fundamental.cpp		\
	gi/fundamental.h		\
	gi/function.cpp			\
	gi/function.h			\
	gi/gerror.cpp			\
	gi/gerror.h			\
	gi/gjs_gi_trace.h		\
	gi/gtype.cpp			\
	gi/gtype.h			\
	gi/interface.cpp		\
	gi/interface.h			\
	gi/ns.cpp			\
	gi/ns.h	        		\
	gi/object.cpp			\
	gi/object.h			\
	gi/param.cpp			\
	gi/param.h			\
	gi/proxyutils.cpp		\
	gi/proxyutils.h			\
	gi/repo.cpp			\
	gi/repo.h			\
	gi/toggle.cpp			\
	gi/toggle.h			\
	gi/union.cpp			\
	gi/union.h			\
	gi/value.cpp			\
	gi/value.h			\
	cjs/byteArray.cpp		\
	cjs/byteArray.h			\
	cjs/context.cpp			\
	cjs/context-private.h		\
	cjs/coverage.cpp 		\
	cjs/engine.cpp			\
	cjs/engine.h			\
	cjs/global.cpp			\
	cjs/global.h			\
	cjs/importer.cpp		\
	cjs/importer.h			\
	cjs/jsapi-class.h		\
	cjs/jsapi-dynamic-class.cpp	\
	cjs/jsapi-util.cpp		\
	cjs/jsapi-util.h		\
	cjs/jsapi-util-args.h		\
	cjs/jsapi-util-error.cpp	\
	cjs/jsapi-util-root.h		\
	cjs/jsapi-util-string.cpp	\
	cjs/jsapi-wrapper.h		\
	cjs/mem.h			\
	cjs/mem.cpp			\
	cjs/module.h			\
	cjs/module.cpp			\
	cjs/native.cpp			\
	cjs/native.h			\
	cjs/profiler.cpp		\
	cjs/profiler-private.h		\
	cjs/stack.cpp			\
	modules/modules.cpp		\
	modules/modules.h		\
	util/error.cpp			\
	util/glib.cpp			\
	util/glib.h			\
	util/log.cpp			\
	util/log.h			\
	util/misc.cpp			\
	util/misc.h			\
	$(NULL)

# These files were part of a separate library
gjs_private_srcs =				\
	libgjs-private/gjs-gdbus-wrapper.cpp	\
	libgjs-private/gjs-gdbus-wrapper.h	\
	libgjs-private/gjs-util.cpp		\
	libgjs-private/gjs-util.h		\
	libgjs-private/gjs-gtk-util.h		\
	$(NULL)

gjs_gtk_private_srcs =			\
	libgjs-private/gjs-gtk-util.c	\
	$(NULL)

gjs_console_srcs =	\
	cjs/console.cpp	\
	$(NULL)

gjs_sysprof_srcs =			\
	util/sp-capture-types.h		\
	util/sp-capture-writer.c	\
	util/sp-capture-writer.h	\
	$(NULL)
