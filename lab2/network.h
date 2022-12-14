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

#ifndef NETWORK_H
#define NETWORK_H

#include "simulate.h"

typedef enum { CMD, ACK } Type;

typedef struct {
  /* TODO: Declare target according to the protocol. */
  uint16_t turn;
  Cmd cmd;
  Type type;
} Packet;

void Serialise(unsigned char *buff, const Packet *pkt);

void Assemble(Packet *pkt, const unsigned char *buff);

#endif