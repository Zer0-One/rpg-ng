// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_WINDOW
#define RPGNG_WINDOW

#include <stdbool.h>

#include <SDL2/SDL.h>

extern SDL_Surface* main_window;

bool window_init(char const* title, int x, int y, int w, int h, uint32_t flags);

#endif
