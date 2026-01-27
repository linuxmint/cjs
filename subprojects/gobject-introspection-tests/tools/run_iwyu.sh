#!/bin/bash

# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2024 Philip Chimento <philip.chimento@gmail.com>

IWYU=${IWYU:-include-what-you-use}
CFLAGS=$(pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0 cairo-gobject)
for file in *.c; do
  $IWYU -Xiwyu --mapping_file=tools/iwyu.imp -xc $CFLAGS $file
done
