[![License](https://img.shields.io/badge/License-LGPL%20v2%2B-blue.svg)](https://gitlab.gnome.org/GNOME/gjs/blob/HEAD/COPYING)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://gitlab.gnome.org/GNOME/gjs/blob/HEAD/COPYING)

CJS fork of GJS for Cinnamon. It is functionally identical, and is only
to maintain stability with the Cinnamon release cycle, and between various
distributions.

CJS is a JavaScript runtime built on
[Firefox's SpiderMonkey JavaScript engine](https://spidermonkey.dev/) and
the [GNOME platform libraries](https://developer.gnome.org/).

## Usage

CJS includes a command-line interpreter, usually installed in
`/usr/bin/cjs`.
Type `cjs` to start it and test out your JavaScript statements
interactively.
Hit Ctrl+D to exit.

`cjs filename.js` runs a whole program.
`cjs -d filename.js` does that and starts a debugger as well.

There are also facilities for generating code coverage reports.
Type `cjs --help` for more information.

_______
.
# Upstream

## Contributing
[![Contributors](https://img.shields.io/github/contributors/GNOME/gjs.svg)](https://gitlab.gnome.org/GNOME/gjs/-/graphs/HEAD)

For instructions on how to get started contributing to GJS, please read
the contributing guide,
<https://gitlab.gnome.org/GNOME/gjs/blob/HEAD/CONTRIBUTING.md>.

## History

GJS probably started in August 2008 with [this blog post][havocp] and
[this experimental code][gscript].
GJS in its current form was first developed in October 2008 at a company
called litl, for their [litl webbook] product.
It was soon adopted as the basis of [GNOME Shell]'s UI code and
extensions system and debuted as a fundamental component of GNOME 3.0.

In February 2013 at the GNOME Developer Experience Hackfest GJS was
declared the ['first among equals'][treitter] of languages for GNOME
application development.
That proved controversial for many, and was later abandoned.

At the time of writing (2018) GJS is used in many systems including
Endless OS's [framework for offline content][eos-knowledge-lib] and, as
a forked version, [Cinnamon].

## Reading material

### Documentation

* [Get started](https://gitlab.gnome.org/GNOME/gjs/blob/HEAD/CONTRIBUTING.md)
* [Get started - Internship](https://gitlab.gnome.org/GNOME/gjs/blob/HEAD/doc/Internship-Getting-Started.md)
* [API documentation](https://gjs-docs.gnome.org/)

### JavaScript & SpiderMonkey

* https://github.com/spidermonkey-embedders/spidermonkey-embedding-examples

### GNOME Contribution

* https://wiki.gnome.org/GitLab
* https://wiki.gnome.org/Newcomers/

## License

Dual licensed under LGPL 2.0+ and MIT.

## Thanks ##

The form of this README was inspired by [Nadia Odunayo][hospitable] on
the Greater Than Code podcast.

[havocp]: https://blog.ometer.com/2008/08/25/embeddable-languages/
[gscript]: https://gitlab.gnome.org/Archive/gscript/tree/HEAD/gscript
[litl webbook]: https://en.wikipedia.org/wiki/Litl
[GNOME Shell]: https://wiki.gnome.org/Projects/GnomeShell
[treitter]: https://treitter.livejournal.com/14871.html
[eos-knowledge-lib]: http://endlessm.github.io/eos-knowledge-lib/
[Cinnamon]: https://en.wikipedia.org/wiki/Cinnamon_(software)
[hospitable]: https://www.greaterthancode.com/code-hospitality
