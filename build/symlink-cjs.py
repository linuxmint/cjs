#!/usr/bin/env python3
# SPDX-License-Identifier: MIT OR LGPL-2.0-or-later
# SPDX-FileCopyrightText: 2019 Chun-wei Fan <fanchunwei@src.gnome.org>

import os
import shutil
import sys
import tempfile

assert(len(sys.argv) == 2)

installed_bin_dir = os.path.join(os.environ.get('MESON_INSTALL_DESTDIR_PREFIX'),
    sys.argv[1])

if os.name == 'nt':
    # Using symlinks on Windows often require administrative privileges,
    # which is not what we want.  Instead, copy cjs-console.exe.
    shutil.copyfile('cjs-console.exe', os.path.join(installed_bin_dir, 'cjs.exe'))
else:
    try:
        temp_link = tempfile.mktemp(dir=installed_bin_dir)
        os.symlink('cjs-console', temp_link)
        os.replace(temp_link, os.path.join(installed_bin_dir, 'cjs'))
    finally:
        if os.path.islink(temp_link):
            os.remove(temp_link)
