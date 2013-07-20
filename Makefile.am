# http://people.gnome.org/~walters/docs/build-api.txt
.buildapi-allow-builddir:

-include $(INTROSPECTION_MAKEFILE)

bin_PROGRAMS =
lib_LTLIBRARIES =
noinst_HEADERS =
noinst_LTLIBRARIES =
dist_gjsjs_DATA =
gjsnative_LTLIBRARIES =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
cjstest_files_with_tests =
TEST_PROGS =
check_PROGRAMS = $(TEST_PROGS)
INTROSPECTION_GIRS =
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

gjsjsdir = @gjsjsdir@
gjsoverridedir = $(gjsjsdir)/overrides
gjstweenerdir = $(gjsjsdir)/tweener
gjsnativedir = @gjsnativedir@

gjs_public_includedir = $(includedir)/cjs-1.0
gjs_module_includedir = $(includedir)/cjs-1.0

########################################################################
nobase_gjs_public_include_HEADERS =	\
	cjs/context.h		\
	cjs/gjs.h

nobase_gjs_module_include_HEADERS =	\
	cjs/gjs-module.h	\
	cjs/compat.h		\
	cjs/byteArray.h		\
	cjs/importer.h		\
	cjs/jsapi-util.h	\
	cjs/type-module.h	\
	cjs/mem.h		\
	cjs/native.h	\
	gi/ns.h	        \
	gi/object.h	\
	gi/foreign.h	\
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
	cjs/profiler.h		\
	cjs/unit-test-utils.h	\
	util/crash.h		\
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
	-DGJS_JS_DIR=\"$(gjsjsdir)\"			\
	-DGJS_NATIVE_DIR=\"$(gjsnativedir)\"		\
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
	-export-symbols-regex "^[^_]" -version-info 0:0:0	\
	-no-undefined \
	-rdynamic
libcjs_la_LIBADD = 		\
	$(GJS_LIBS)

libcjs_la_SOURCES =		\
	cjs/byteArray.c		\
	cjs/context.c		\
	cjs/importer.c		\
	cjs/gi.h		\
	cjs/gi.c		\
	cjs/jsapi-private.cpp	\
	cjs/jsapi-util.c	\
	cjs/jsapi-util-array.c	\
	cjs/jsapi-util-error.c	\
	cjs/jsapi-util-string.c	\
	cjs/mem.c		\
	cjs/native.c		\
	cjs/profiler.c		\
	cjs/stack.c		\
	cjs/type-module.c	\
	cjs/unit-test-utils.c	\
	util/error.c		\
	util/glib.c		\
	util/crash.c		\
	util/log.c		\
	util/misc.c

# For historical reasons, some files live in gi/
libcjs_la_SOURCES += \
	gi/gjs_gi_trace.h \
	gi/arg.c	\
	gi/boxed.c	\
	gi/closure.c	\
	gi/enumeration.c	\
	gi/function.c	\
	gi/keep-alive.c	\
	gi/ns.c	\
	gi/object.c	\
	gi/foreign.c	\
	gi/param.c	\
        gi/repo.c	\
	gi/union.c	\
        gi/value.c	\
	gi/interface.c	\
	gi/gtype.c	\
	gi/gerror.c

# Also, these files used to be a separate library
gdbus_wrapper_source_files = \
	cjs-dbus/gjs-gdbus-wrapper.c	\
	cjs-dbus/gjs-gdbus-wrapper.h
libcjs_la_SOURCES += $(gdbus_wrapper_source_files)

CjsPrivate-1.0.gir: libcjs.la
CjsPrivate_1_0_gir_LIBS = libcjs.la
CjsPrivate_1_0_gir_INCLUDES = GObject-2.0 Gio-2.0
CjsPrivate_1_0_gir_FILES = $(gdbus_wrapper_source_files)
CjsPrivate_1_0_gir_SCANNERFLAGS = --identifier-prefix=Gjs --symbol-prefix=gjs_ --warn-all

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

cjstest_files_with_tests += 	\
	cjs/context.c		\
	cjs/jsapi-util-array.c	\
	cjs/jsapi-util-error.c	\
	cjs/jsapi-util-string.c	\
	cjs/stack.c				\
	util/glib.c

tapset_in_files = cjs/gjs.stp.in
EXTRA_DIST += $(tapset_in_files)
if ENABLE_SYSTEMTAP
cjs/gjs.stp: cjs/gjs.stp.in Makefile
	sed -e s,@EXPANDED_LIBDIR@,$(libdir), < $< > $@.tmp && mv $@.tmp $@
tapsetdir   = $(DESTDIR)$(datadir)/systemtap/tapset
tapset_DATA = $(tapset_in_files:.stp.in=.stp)
endif

include Makefile-cjs-dbus.am
include Makefile-modules.am
include Makefile-examples.am

typelibdir = $(pkglibdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

########################################################################
bin_PROGRAMS += cjs-console

cjs_console_CFLAGS = 		\
	$(AM_CFLAGS)		\
        $(GOBJECT_CFLAGS)
cjs_console_LDADD =		\
	$(JS_LIBS)		\
	$(GOBJECT_LIBS)		\
	libcjs.la
cjs_console_LDFLAGS = -rdynamic
cjs_console_SOURCES = cjs/console.c

install-exec-hook:
	(cd $(DESTDIR)$(bindir) && ln -sf cjs-console$(EXEEXT) cjs$(EXEEXT))

include Makefile-test.am

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
