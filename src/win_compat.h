// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_WIN_COMPAT
#define RPGNG_WIN_COMPAT

#ifdef _MSC_VER
#define localtime_r localtime_s
#define strerror_r(errnum, buf, buflen) strerror_s(buf, buflen, errnum)
#endif

#endif
