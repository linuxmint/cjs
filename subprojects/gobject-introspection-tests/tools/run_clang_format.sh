#!/bin/bash

# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2024 Philip Chimento <philip.chimento@gmail.com>

CLANG_FORMAT=${CLANG_FORMAT:-clang-format}
for file in *.c *.h; do
  $CLANG_FORMAT -i $file
done
