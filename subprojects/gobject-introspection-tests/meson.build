# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2024 Philip Chimento <philip.chimento@gmail.com>

project(
    'gobject-introspection-tests', 'c', version: 'unversioned',
    meson_version: '>= 0.61',
    default_options: ['c_std=c99', 'warning_level=2']
)

gnome = import('gnome')

cc = meson.get_compiler('c')
msvc = cc.get_argument_syntax() == 'msvc'

build_cairo = get_option('cairo')
install_dir = get_option('install_dir')
install = (install_dir != '')

glib_version = '>= 2.66.8'
glib_dep = dependency('glib-2.0', version: glib_version)
gobject_dep = dependency('gobject-2.0', version: glib_version)
gio_dep = dependency('gio-2.0', version: glib_version)

if build_cairo
    cairo_dep = dependency('cairo', required: not msvc)
    cairo_gobject_dep = dependency('cairo-gobject', required: not msvc)

    # We might need to look for the headers and libs for Cairo manually on
    # MSVC/clang-cl builds...
    if not cairo_dep.found()
        cairo_dep = cc.find_library(
            'cairo',
            has_headers: ['cairo.h'],
            required: true
        )
    endif
    if not cairo_gobject_dep.found()
        cairo_gobject_dep = cc.find_library(
            'cairo-gobject',
            has_headers: ['cairo-gobject.h'],
            required: true
        )
    endif
endif

args = []

if host_machine.system() == 'windows'
    extern_def = '__declspec(dllexport) extern'
else
    extern_def = '__attribute__((visibility("default"))) extern'
endif
args += ['-DGI_TEST_EXTERN=@0@'.format(extern_def)]

if not build_cairo
    args += ['-DGI_TEST_DISABLE_CAIRO']
endif

add_project_arguments(args, language: 'c')

utility_sources = ['utility.c', 'utility.h']
regress_sources = [
    'annotation.c',
    'annotation.h',
    'drawable.c',
    'drawable.h',
    'foo.c',
    'foo.h',
    'regress.c',
    'regress.h',
    'regressextra.c',
    'regressextra.h',
]
regress_unix_sources = ['regress-unix.c', 'regress-unix.h']
warnlib_sources = ['warnlib.c', 'warnlib.h']
gimarshallingtests_sources = [
    'gimarshallingtests.c',
    'gimarshallingtests.h',
    'gimarshallingtestsextra.c',
    'gimarshallingtestsextra.h',
]

libutility = library(
    'utility',
    utility_sources,
    dependencies: [glib_dep, gobject_dep],
    gnu_symbol_visibility: 'hidden',
    install: install,
    install_dir: install_dir,
)
utility_gir = gnome.generate_gir(
    libutility,
    sources: utility_sources,
    includes: ['GObject-2.0'],
    namespace: 'Utility',
    nsversion: '1.0',
    symbol_prefix: 'utility_',
    header: 'utility.h',
    fatal_warnings: get_option('werror'),
    install_gir: false,
    install_typelib: install,
    install_dir_typelib: install_dir,
)
utility_typelib = utility_gir[1]

regress_dependencies = [glib_dep, gobject_dep, gio_dep]
regress_gir_includes = ['Gio-2.0', utility_gir[0]]
if build_cairo
    regress_gir_includes += 'cairo-1.0'
    regress_dependencies += [cairo_dep, cairo_gobject_dep]
endif

libregress = library(
    'regress',
    regress_sources + regress_unix_sources,
    dependencies: regress_dependencies,
    gnu_symbol_visibility: 'hidden',
    install: install,
    install_dir: install_dir,
    override_options: ['unity=off'],
)
regress_gir = gnome.generate_gir(
    libregress,
    sources: regress_sources,
    includes: regress_gir_includes,
    namespace: 'Regress',
    nsversion: '1.0',
    identifier_prefix: 'Regress',
    symbol_prefix: 'regress_',
    header: 'regress.h',
    extra_args: ['--warn-all'],
    include_directories: meson.current_build_dir(),
    fatal_warnings: get_option('werror'),
    install_gir: false,
    install_typelib: install,
    install_dir_typelib: install_dir,
)
regress_typelib = regress_gir[1]
if host_machine.system() != 'windows'
    regress_unix_gir = gnome.generate_gir(
        libregress,
        sources: regress_unix_sources,
        includes: regress_gir_includes,
        namespace: 'RegressUnix',
        nsversion: '1.0',
        symbol_prefix: 'regress_unix_',
        header: 'regress-unix.h',
        include_directories: meson.current_build_dir(),
        fatal_warnings: get_option('werror'),
        install_gir: false,
        install_typelib: install,
        install_dir_typelib: install_dir,
    )
else
    regress_unix_gir = [disabler(), disabler()]
endif
regress_unix_typelib = regress_unix_gir[1]

libwarnlib = library(
    'warnlib',
    warnlib_sources,
    dependencies: [glib_dep, gobject_dep, gio_dep],
    gnu_symbol_visibility: 'hidden',
    install: install,
    install_dir: install_dir,
)
# This should have --warn-all turned off, but there is currently no way to do so
# in gnome.generate_gir(). See https://github.com/mesonbuild/meson/issues/5876
warnlib_gir = gnome.generate_gir(
    libwarnlib,
    includes: ['Gio-2.0'],
    sources: warnlib_sources,
    namespace: 'WarnLib',
    nsversion: '1.0',
    symbol_prefix: 'warnlib_',
    header: 'warnlib.h',
    fatal_warnings: false,
    install_gir: false,
    install_typelib: install,
    install_dir_typelib: install_dir,
)
warnlib_typelib = warnlib_gir[1]

libgimarshallingtests = library(
    'gimarshallingtests',
    gimarshallingtests_sources,
    dependencies: [glib_dep, gobject_dep, gio_dep],
    gnu_symbol_visibility: 'hidden',
    install: install,
    install_dir: install_dir,
)
gimarshallingtests_gir = gnome.generate_gir(
    libgimarshallingtests,
    sources: gimarshallingtests_sources,
    includes: ['Gio-2.0'],
    namespace: 'GIMarshallingTests',
    nsversion: '1.0',
    symbol_prefix: 'gi_marshalling_tests_',
    fatal_warnings: get_option('werror'),
    install_gir: false,
    install_typelib: install,
    install_dir_typelib: install_dir,
)
gimarshallingtests_typelib = gimarshallingtests_gir[1]

deps = [glib_dep]
if build_cairo
    deps += [cairo_dep, cairo_gobject_dep]
endif
locations = []
foreach dep: deps
    if dep.type_name() == 'pkgconfig'
        locations += 'in @0@'.format(dep.get_variable(pkgconfig: 'prefix'))
    else
        locations += dep.type_name()
    endif
endforeach

summary({
    'Build type': get_option('buildtype'),
    'Install test libraries': install,
    'GLib': '@0@ (@1@)'.format(glib_dep.version(), locations[0]),
    'Cairo support': build_cairo,
}, bool_yn: true)
if build_cairo
    summary({
        'Cairo': '@0@ (@1@)'.format(cairo_dep.version(), locations[1]),
        'Cairo-gobject': '@0@ (@1@)'.format(cairo_gobject_dep.version(), locations[2])
    })
endif
if install
    summary('Install location', install_dir)
endif
