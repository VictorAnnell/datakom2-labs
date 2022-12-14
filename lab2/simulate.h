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

#ifndef SIMULATE_H
#define SIMULATE_H

#include <stdint.h>

#define NPLAYER 2

typedef enum { CMD_NONE, CMD_UP, CMD_DOWN } Cmd;

typedef struct {
  int x, y;
} Vec;

typedef struct {
  Vec pos;
  Vec size;
  int speed;
} Paddle;

typedef struct {
  Vec pos;
  Vec vel;
  int radius;
} Ball;

typedef struct {
  Paddle paddle[NPLAYER];
  Ball ball;
  Vec bound;
} State;

State SimulateInit(int width, int height);

State Simulate(State state, const Cmd cmd[2], uint32_t dt);

State Interpolate(const State *s1, const State *s2, float ratio);

#endif
