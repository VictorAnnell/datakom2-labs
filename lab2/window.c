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

#include "window.h"

#include <SDL.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static int win_width, win_height;

void WinInit(int width, int height) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("xpong", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height,
                            SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, 0);
  win_width = width;
  win_height = height;
}

void WinFini() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

WindowEvent WinPullEvent() {
  WindowEvent we = {0};
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      we.quit = true;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_ESCAPE:
        we.quit = true;
      case SDLK_UP:
        we.up = true;
        break;
      case SDLK_DOWN:
        we.down = true;
        break;
      default:
        break;
      }
    }
  }
  return we;
}

Vec VecMap(Vec gpos) {
  Vec spos = {gpos.x + win_width / 2, -gpos.y + win_height / 2};
  return spos;
}

SDL_Rect Rect(Vec pos, Vec size) {
  Vec spos = VecMap(pos);
  SDL_Rect rect = {spos.x - size.x / 2, spos.y - size.y / 2, size.x, size.y};
  return rect;
}

void RenderPaddle(SDL_Renderer *renderer, const Paddle *paddle) {
  SDL_Rect rect = Rect(paddle->pos, paddle->size);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderFillRect(renderer, &rect);
}

void RenderBounds(SDL_Renderer *renderer, Vec bound) {
  Vec size = {2 * bound.x, 10};
  Vec up_pos = {0, bound.y + 5};
  SDL_Rect rect_up = Rect(up_pos, size);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderFillRect(renderer, &rect_up);

  Vec down_pos = {0, -bound.y - 5};
  SDL_Rect rect_down = Rect(down_pos, size);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderFillRect(renderer, &rect_down);
}

void RenderBall(SDL_Renderer *renderer, const Ball *ball) {
  Vec size = {ball->radius * 2, ball->radius * 2};
  SDL_Rect rect = Rect(ball->pos, size);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderFillRect(renderer, &rect);
}

void WinRender(const State *state) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
  SDL_RenderClear(renderer);

  RenderBounds(renderer, state->bound);

  for (size_t i = 0; i < NPLAYER; ++i) {
    RenderPaddle(renderer, &state->paddle[i]);
  }

  RenderBall(renderer, &state->ball);

  SDL_RenderPresent(renderer);
}

uint32_t WinTick() { return SDL_GetTicks(); }
