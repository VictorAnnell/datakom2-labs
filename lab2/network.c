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

#include "network.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>

void Serialise(unsigned char *buff, const Packet *pkt) {
  /* TODO: pack the packet such that it uses network endian and is compact. */
  uint16_t turn = htons(pkt->turn);
  uint8_t cmd = pkt->cmd;
  uint8_t type = pkt->type;

  memcpy(buff, &turn, sizeof(turn));
  memcpy(buff + sizeof(turn), &cmd, sizeof(cmd));
  memcpy(buff + sizeof(turn) + sizeof(cmd), &type, sizeof(type));
}

void Assemble(Packet *pkt, const unsigned char *buff) {
  /* TODO: unpack the packet into the Packet structure. */
  uint16_t turn;
  uint8_t cmd, type;

  memcpy(&turn, buff, sizeof(turn));
  memcpy(&cmd, buff + sizeof(turn), sizeof(cmd));
  memcpy(&type, buff + sizeof(turn) + sizeof(cmd), sizeof(type));
  turn = ntohs(turn);

  pkt->turn = turn;
  pkt->cmd = cmd;
  pkt->type = type;
}

