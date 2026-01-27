/*
SPDX-License-Identifier: MIT
SPDX-FileCopyrightText: 2024 Simon McVittie
*/

#pragma once

#include <glib.h>

#ifdef G_OS_UNIX

#include <sys/socket.h>
#include <sys/types.h>

#include "gitestmacros.h"

GI_TEST_EXTERN
dev_t regress_unix_test_devt (dev_t in);

GI_TEST_EXTERN
gid_t regress_unix_test_gidt (gid_t in);

GI_TEST_EXTERN
pid_t regress_unix_test_pidt (pid_t in);

GI_TEST_EXTERN
socklen_t regress_unix_test_socklent (socklen_t in);

GI_TEST_EXTERN
uid_t regress_unix_test_uidt (uid_t in);

#endif
