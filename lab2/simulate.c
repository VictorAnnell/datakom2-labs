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

#include "simulate.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

int Normal(int x) {
  assert(x != 0);
  return x > 0 ? 1 : -1;
}

Paddle MovePaddle(Paddle paddle, Vec bound, Cmd cmd, uint32_t dt) {
  int cmd_speed[] = {0, paddle.speed, -paddle.speed};
  paddle.pos.y += cmd_speed[cmd] * dt;
  if (abs(paddle.pos.y) + paddle.size.y / 2 > bound.y) {
    paddle.pos.y = Normal(paddle.pos.y) * bound.y -
                   Normal(paddle.pos.y) * paddle.size.y / 2;
  }
  return paddle;
}

Ball MoveBall(Ball ball, Paddle paddle[2], Vec bound, uint32_t dt) {
  ball.pos.x += ball.vel.x * dt;
  ball.pos.y += ball.vel.y * dt;

  if (abs(ball.pos.y) + ball.radius > bound.y) {
    ball.pos.y =
        Normal(ball.pos.y) * bound.y - Normal(ball.pos.y) * ball.radius;
    ball.vel.y = -ball.vel.y;
  }

  if (!ball.pos.x)
    return ball;

  Paddle p = paddle[ball.pos.x > 0];
  if (p.pos.y - p.size.y / 2 < ball.pos.y &&
      ball.pos.y < p.pos.y + p.size.y / 2)
    if (abs(p.pos.x) - p.size.x / 2 - ball.radius < abs(ball.pos.x) &&
        abs(ball.pos.x) < abs(p.pos.x) + p.size.x / 2 + ball.radius) {
      ball.pos.x = p.pos.x - Normal(ball.pos.x) * ball.radius -
                   Normal(ball.pos.x) * p.size.x / 2;

      ball.vel.x *= -1;

      int hit_pos = p.pos.y - ball.pos.y;
      ball.vel.y = -hit_pos / (p.size.y / 5);
    }

  if (abs(ball.pos.x) > bound.x) {
    ball.pos.x = 0;
    ball.pos.y = 0;
    ball.vel.x = Normal(ball.vel.x);
    ball.vel.y = 0;
  }

  return ball;
}

State SimulateInit(int width, int height) {
  Ball ball = {.pos = {0, 0}, .vel = {1, 0}, .radius = 10};
  Paddle paddle0 = {.pos = {-width / 2 + 50, 0}, .size = {20, 100}, .speed = 2};
  Paddle paddle1 = paddle0;
  paddle1.pos.x *= -1;
  Vec bound = {width / 2 - 20, height / 2 - 20};

  State state = {.paddle = {paddle0, paddle1}, .ball = ball, .bound = bound};
  return state;
}

State Simulate(State state, const Cmd cmd[2], uint32_t dt) {
  for (size_t i = 0; i < NPLAYER; ++i) {
    state.paddle[i] = MovePaddle(state.paddle[i], state.bound, cmd[i], dt);
  }

  state.ball = MoveBall(state.ball, state.paddle, state.bound, dt);

  return state;
}

State Interpolate(const State *s1, const State *s2, float ratio) {
  State state = *s1;
  for (size_t i = 0; i < NPLAYER; ++i) {
    state.paddle[i].pos.y +=
        ratio * (s2->paddle[i].pos.y - s1->paddle[i].pos.y);
  }

  state.ball.pos.x += ratio * (s2->ball.pos.x - s1->ball.pos.x);
  state.ball.pos.y += ratio * (s2->ball.pos.y - s1->ball.pos.y);

  return state;
}
