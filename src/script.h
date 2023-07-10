// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_SCRIPT
#define RPGNG_SCRIPT

#include <stdbool.h>

/**
 * Initializes the scripting subsystem.
 *
 * @return True on success.
 * @return False on failure.
 */
bool script_init();

/**
 * Runs the Lua script at the given path.
 *
 * @param path A path to a Lua 5.4 script.
 *
 * @return True if the entire script was successfully executed.
 * @return False if any error was encountered.
 */
bool script_run(const char* path);

#endif
