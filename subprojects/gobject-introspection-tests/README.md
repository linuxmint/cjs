# GObject Introspection Tests

This is a git submodule containing C source files.
These files are compiled into shared libraries intended for testing language
bindings for the GNOME platform.
They are also used to test the `g-ir-scanner` tool from [gobject-introspection].

[gobject-introspection]: https://gitlab.gnome.org/GNOME/gobject-introspection

## Available modules

**Regress** test suite — a library designed as a general regression test suite,
accumulated over the lifetime of gobject-introspection.
Consists of `regress.c`, `annotation.c`, `drawable.c`, `foo.c`, and their
associated header files.

**GIMarshallingTests** suite — a library designed to exercise how every kind
of C value used in GNOME platform APIs is converted to and from native values
from the language binding.

**Utility** test suite — a library for testing external type references.

**WarnLib** test suite — a library with a number of conditions that
`g-ir-scanner` will warn about, used to test how language bindings deal with
typelibs that contain non-ideal C code.

## How to Use in a Dynamic Language Binding

Include this repository as a git submodule in your sources.

```sh
git submodule add https://gitlab.gnome.org/GNOME/gobject-introspection-tests.git
```

If you use the Meson build system, you can also add it as a Meson subproject,
and simply write:

```meson
gi_tests = subproject('gobject-introspection-tests')
```

The subproject exports variables `gimarshallingtests_typelib`,
`regress_typelib`, `utility_typelib`, and `warnlib_typelib` which you can access
with `gi_tests.get_variable('regress_typelib')` etc.
You will have to decide which modules you want to use.
Most language bindings will want to use at least Regress and GIMarshallingTests.

Now you can write tests that load the module, call each of its functions with
the expected input, and check for the expected output.
You will need to add the build folder to the environment variables
`LD_LIBRARY_PATH` and `GI_TYPELIB_PATH`, something like this:

```meson
tests_environment = environment()
gi_tests_builddir = meson.project_build_root() / 'subprojects' / 'gobject-introspection-tests'
tests_environment.prepend('LD_LIBRARY_PATH', gi_tests_builddir)
tests_environment.prepend('GI_TYPELIB_PATH', gi_tests_builddir)
```

and then pass `env: tests_environment` in your call to `test()`.

If your language binding supports installed tests, you will have to install the
built modules manually, to a private location where your installed tests can
find them.
Do not install them in the default typelibs directory, where they may conflict
with other language bindings' installed tests.
Depending on how your build system is structured it might look something like
this, using GIMarshallingTests as an example:

```meson
gi_tests = subproject('gobject-introspection-tests',
    default_options: ['install_dir=@0@'.format(installed_tests_dir)])
```

For build systems other than Meson, simply ignore the `meson.build` file and add
code to your build system to build the modules as if it was a regular
subdirectory.
