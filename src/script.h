// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_SCRIPT
#define RPGNG_SCRIPT

#include <stdbool.h>

/**
 * Initializes the scripting subsystem.
 */
void script_init(void);

/**
 * Shuts down all initialized interpreters, and frees all associated memory.
 *
 * @return True on success.
 * @return False if an error was encountered.
 */
bool script_cleanup(void);

// Foos the bar
bool script_foo(void);

/**
 * Runs the Python script at the given path.
 *
 * @param path A path to a Python script.
 *
 * @return True if the entire script was successfully executed.
 * @return False if any error was encountered.
 */
bool script_run(const char* path);

#endif
