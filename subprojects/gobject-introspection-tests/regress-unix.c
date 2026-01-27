/*
SPDX-License-Identifier: MIT
SPDX-FileCopyrightText: 2024 Simon McVittie
*/

#include "regress-unix.h"

#ifdef G_OS_UNIX

dev_t
regress_unix_test_devt (dev_t in)
{
  return in;
}

gid_t
regress_unix_test_gidt (gid_t in)
{
  return in;
}

pid_t
regress_unix_test_pidt (pid_t in)
{
  return in;
}

socklen_t
regress_unix_test_socklent (socklen_t in)
{
  return in;
}

uid_t
regress_unix_test_uidt (uid_t in)
{
  return in;
}

#endif
