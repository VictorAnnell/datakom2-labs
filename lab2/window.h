/*
 * Copyright (C) 2022  Xiaoyue Chen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RENDER_H
#define RENDER_H

#include "simulate.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool quit, up, down;
} WindowEvent;

WindowEvent WinPullEvent();

void WinInit(int width, int height);

void WinFini();

void WinRender(const State *state);

/* Return ticks in milliseconds */
uint32_t WinTick();

#endif
