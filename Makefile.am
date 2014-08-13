# http://people.gnome.org/~walters/docs/build-api.txt
.buildapi-allow-builddir:

-include $(INTROSPECTION_MAKEFILE)

bin_PROGRAMS =
lib_LTLIBRARIES =
noinst_HEADERS =
noinst_LTLIBRARIES =
dist_gjsjs_DATA =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
TEST_PROGS =
check_PROGRAMS = $(TEST_PROGS)
INTROSPECTION_GIRS =
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CFLAGS = $(WARN_CFLAGS) -DG_LOG_DOMAIN=\"Cjs\"
AM_CPPFLAGS = $(WARN_CXXFLAGS) -DG_LOG_DOMAIN=\"Cjs\"

gjs_public_includedir = $(includedir)/cjs-1.0
gjs_module_includedir = $(includedir)/cjs-1.0

########################################################################
nobase_gjs_public_include_HEADERS =	\
	cjs/context.h		\
	cjs/gjs.h

nobase_gjs_module_include_HEADERS =	\
	cjs/gjs-module.h	\
	cjs/compat.h		\
	cjs/coverage.h \
	cjs/byteArray.h		\
	cjs/importer.h		\
	cjs/jsapi-util.h	\
	cjs/runtime.h		\
	cjs/type-module.h	\
	cjs/mem.h		\
	cjs/native.h	\
	gi/ns.h	        \
	gi/object.h	\
	gi/foreign.h	\
	gi/fundamental.h	\
	gi/param.h	\
	gi/repo.h	\
	gi/union.h	\
	gi/value.h	\
	gi/arg.h	\
	gi/boxed.h	\
	gi/closure.h	\
	gi/enumeration.h	\
	gi/function.h	\
	gi/keep-alive.h	\
	gi/interface.h	\
	gi/gtype.h	\
	gi/gerror.h

noinst_HEADERS +=		\
	cjs/jsapi-private.h	\
	cjs/context-private.h	\
	gi/proxyutils.h		\
	util/crash.h		\
	util/hash-x32.h		\
	util/error.h		\
	util/glib.h		\
	util/log.h		\
	util/misc.h

########################################################################
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cjs-1.0.pc cjs-internals-1.0.pc

EXTRA_DIST += 			\
	cjs-1.0.pc.in           \
	cjs-internals-1.0.pc.in

########################################################################
gjs_directory_defines = 				\
	-DGJS_TOP_SRCDIR=\"$(top_srcdir)\"		\
	-DPKGLIBDIR=\"$(pkglibdir)\"

########################################################################
lib_LTLIBRARIES += libcjs.la

libcjs_la_CPPFLAGS =		\
	$(AM_CPPFLAGS)		\
	$(GJS_CFLAGS)	\
	$(gjs_directory_defines)\
	-I$(top_srcdir)/gi	\
	-DGJS_COMPILATION
libcjs_la_CFLAGS = 		\
	$(AM_CFLAGS)
libcjs_la_LDFLAGS = 		\
	$(EXTRA_LINK_FLAGS)	\
	-export-symbols-regex "^[^_]" -version-info 0:0:0	\
	-no-undefined \
	-rdynamic
libcjs_la_LIBADD = 		\
	$(GJS_LIBS)

if ENABLE_GTK
libcjs_la_CPPFLAGS += $(GJS_GTK_CFLAGS)
libcjs_la_LIBADD += $(GJS_GTK_LIBS)
endif

libcjs_la_SOURCES =		\
	cjs/byteArray.cpp		\
	cjs/context.cpp		\
	cjs/importer.cpp		\
	cjs/gi.h		\
	cjs/gi.cpp		\
	cjs/coverage.cpp \
	cjs/jsapi-private.cpp	\
	cjs/jsapi-util.cpp	\
	cjs/jsapi-dynamic-class.cpp \
	cjs/jsapi-util-array.cpp	\
	cjs/jsapi-util-error.cpp	\
	cjs/jsapi-util-string.cpp	\
	cjs/mem.cpp		\
	cjs/native.cpp		\
	cjs/runtime.cpp		\
	cjs/stack.cpp		\
	cjs/type-module.cpp	\
	modules/modules.cpp	\
	modules/modules.h	\
	util/error.cpp		\
	util/hash-x32.cpp		\
	util/glib.cpp		\
	util/crash.cpp		\
	util/log.cpp		\
	util/misc.cpp

# For historical reasons, some files live in gi/
libcjs_la_SOURCES += \
	gi/gjs_gi_trace.h \
	gi/arg.cpp	\
	gi/boxed.cpp	\
	gi/closure.cpp	\
	gi/enumeration.cpp	\
	gi/function.cpp	\
	gi/keep-alive.cpp	\
	gi/ns.cpp	\
	gi/object.cpp	\
	gi/foreign.cpp	\
	gi/fundamental.cpp	\
	gi/param.cpp	\
	gi/proxyutils.cpp	\
        gi/repo.cpp	\
	gi/union.cpp	\
        gi/value.cpp	\
	gi/interface.cpp	\
	gi/gtype.cpp	\
	gi/gerror.cpp

# Also, these files used to be a separate library
libcjs_private_source_files = \
	libgjs-private/gjs-gdbus-wrapper.cpp	\
	libgjs-private/gjs-gdbus-wrapper.h	\
	libgjs-private/gjs-util.cpp		\
	libgjs-private/gjs-util.h		\
	libgjs-private/gjs-gtk-util.h

if ENABLE_GTK
libcjs_private_source_files += 			\
	libgjs-private/gjs-gtk-util.c		\
	$(NULL)
endif

libcjs_la_SOURCES += $(libcjs_private_source_files)

CjsPrivate-1.0.gir: libcjs.la
CjsPrivate_1_0_gir_LIBS = libcjs.la
CjsPrivate_1_0_gir_INCLUDES = GObject-2.0 Gio-2.0
CjsPrivate_1_0_gir_FILES = $(libcjs_private_source_files)
CjsPrivate_1_0_gir_SCANNERFLAGS = --identifier-prefix=Gjs --symbol-prefix=gjs_ --warn-all

if ENABLE_GTK
CjsPrivate_1_0_gir_INCLUDES += Gtk-3.0
endif

INTROSPECTION_GIRS += CjsPrivate-1.0.gir

if ENABLE_DTRACE
gjs_gi_probes.h: gi/gjs_gi_probes.d
	$(DTRACE) -C -h -s $< -o $@
gjs_gi_probes.o: gi/gjs_gi_probes.d
	$(DTRACE) -G -s $< -o $@
BUILT_SOURCES += gjs_gi_probes.h gjs_gi_probes.o
libcjs_la_LIBADD += gjs_gi_probes.o
endif
EXTRA_DIST += gi/gjs_gi_probes.d

tapset_in_files = cjs/cjs.stp.in
EXTRA_DIST += $(tapset_in_files)
if ENABLE_SYSTEMTAP
cjs/cjs.stp: cjs/cjs.stp.in Makefile
	sed -e s,@EXPANDED_LIBDIR@,$(libdir), < $< > $@.tmp && mv $@.tmp $@
tapsetdir   = $(DESTDIR)$(datadir)/systemtap/tapset
tapset_DATA = $(tapset_in_files:.stp.in=.stp)
endif

include Makefile-modules.am
include Makefile-examples.am

typelibdir = $(pkglibdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(INTROSPECTION_GIRS) $(typelib_DATA)

########################################################################
bin_PROGRAMS += cjs-console

cjs_console_CPPFLAGS = 		\
	$(AM_CPPFLAGS)		\
        $(GOBJECT_CFLAGS)
cjs_console_LDADD =		\
	$(JS_LIBS)		\
	$(GOBJECT_LIBS)		\
	libcjs.la
cjs_console_LDFLAGS = -rdynamic
cjs_console_SOURCES = cjs/console.cpp

install-exec-hook:
	(cd $(DESTDIR)$(bindir) && ln -sf cjs-console$(EXEEXT) cjs$(EXEEXT))

include Makefile-test.am
include Makefile-insttest.am

EXTRA_DIST += autogen.sh COPYING.LGPL

# Colin's handy Makefile bits for:
# 1) stuffing tarballs with pre-generated scripts from your workstation
# 2) bumping configure.ac version post-release
# 3) tagging correctly in git
# 4) uploading to gnome.org
# To use:
#  $ make check
#  $ make dist
#  $ make prepare-minor-release

# Customize to taste
TAG_PREFIX=CJS_
COMPRESSION=.bz2


PACKAGE=@PACKAGE@
VERSION=@VERSION@
DISTNAME=$(PACKAGE)-$(VERSION).tar$(COMPRESSION)
TAG_VERSION=$(shell echo $(VERSION) |sed s/\\\./_/g)

prepare-release-tag: Makefile
	git tag -m "Tag $(TAG_VERSION)" -a $(TAG_PREFIX)$(TAG_VERSION)

prepare-minor-release: $(DISTNAME) prepare-release-tag Makefile
	env top_srcdir=$(top_srcdir) python $(top_srcdir)/verbump.py

upload-release: $(DISTNAME) Makefile
	git log origin/master..master
	@echo -n "Ok to push? [y/N] "; read ans; test x$$ans == xy || exit 1
	git push --tags origin master:master
	scp $(DISTNAME) master.gnome.org:
	ssh master.gnome.org install-module $(DISTNAME)
