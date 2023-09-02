// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "log.h"

void script_init(void) {
    Py_Initialize();
}

bool script_cleanup(void) {
    if (Py_FinalizeEx() != 0) {
        return false;
    }

    return true;
}

bool script_foo(void) {
    PyRun_SimpleString("print('FOO')");
}
