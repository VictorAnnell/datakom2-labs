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

#include <fcntl.h>  // O_NONBLOCK
#include <unistd.h> // fcntl

#include "network.h"
#include "simulate.h"
#include "window.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

static const int SCREEN_WIDTH = 720;
static const int SCREEN_HEIGHT = 640;
static const int SIM_INTERVAL = 30;
static const int MAX_PACKET_SIZE = 10;

typedef struct {
  bool cmd;
  bool ack;
  bool own_cmd;
} Turn;

int main(int argc, char *argv[argc + 1]) {
  unsigned short port_me = atoi(argv[1]);
  const char *addr_other_str = argv[2];
  unsigned short port_other = atoi(argv[3]);
  int player = atol(argv[4]);

  State state = SimulateInit(SCREEN_WIDTH, SCREEN_HEIGHT);
  WinInit(SCREEN_WIDTH, SCREEN_HEIGHT);

  /* Create socket */
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0) {
    perror("socket() failed");
    exit(EXIT_FAILURE);
  }

  /* Create socket address for this player and the opponent */
  struct sockaddr_in si_me, si_other;
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port_me);
  si_me.sin_addr.s_addr = inet_addr("127.0.0.1");

  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port_other);
  si_other.sin_addr.s_addr = inet_addr(addr_other_str);

  /* Bind the socket to the socket address for this player */
  if (bind(sockfd, (struct sockaddr *)&si_me, sizeof(si_me)) < 0) {
    perror("bind() failed");
    exit(EXIT_FAILURE);
  }

  /* Make socket nonblocking */
  struct timeval read_timeout;
  read_timeout.tv_sec = 0;
  read_timeout.tv_usec = 200;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout,
             sizeof read_timeout);

  uint32_t last_sim_tick = 0;
  uint16_t turn = 0;
  Turn turn_state = {false, false, false};
  Cmd cmds[2] = {0, 0};

  /* Create a buffer and packet to store the received data */
  unsigned char buff[MAX_PACKET_SIZE];
  Packet pkt;

  while (true) {
    uint32_t current_tick = WinTick();
    if (current_tick - last_sim_tick > SIM_INTERVAL) {
      last_sim_tick = current_tick;

      WindowEvent e = WinPullEvent();
      if (e.quit)
        break;

      /* Add conditions for simulation. To simulate and move onto the next
         turn, we must have received the command packet and the acknowledge
         packet from the other player. */
      if (turn_state.ack == true && turn_state.cmd == true &&
          turn_state.own_cmd == true) {
        state = Simulate(state, cmds, 8);
        ++turn;
        turn_state.own_cmd = false;
        turn_state.ack = false;
        turn_state.cmd = false;
      }

      /* Receive packets from the socket and assemble it.
       *
       * If we receive a command packet, send an acknowledgement packet, mark
       * its flag in turn_state, and set the command in cmds array. If we
       * receive a acknowledge packet, just mark its flag in turn_state. */

      /* Receive data from the socket */
      if (turn_state.ack == false || turn_state.cmd == false) {
        ssize_t recv_len = recvfrom(sockfd, buff, sizeof(buff), 0, NULL, NULL);

        if (recv_len > 0) {
          /* Assemble the received data into a Packet structure */
          Assemble(&pkt, buff);

          if (turn == pkt.turn) {
            /* Check the type of the received packet */
            if (pkt.type == CMD) {
              /* Mark the flag in turn_state */
              turn_state.cmd = true;

              /* Set the command in cmds array */
              cmds[!player] = pkt.cmd;

              /* Send an acknowledgement packet */
              pkt.type = ACK;
              pkt.turn = turn;
              Serialise((unsigned char *)buff, &pkt);
              if (sendto(sockfd, buff, sizeof(buff), 0,
                         (struct sockaddr *)&si_other, sizeof(si_other)) < 0) {
                fprintf(stderr, "Failed to send data from the socket: %s\n",
                        strerror(errno));
              };
            } else if (pkt.type == ACK) {
              /* Mark the flag in turn_state */
              turn_state.ack = true;
            }
          }
          continue;
        }
      }

      if (turn_state.own_cmd == false) {
        cmds[player] = (e.up ^ e.down) * (e.up * CMD_UP + e.down * CMD_DOWN);
        turn_state.own_cmd = true;
      }

      /* Send command packet. */
      if (turn_state.ack == false) {
        pkt.turn = turn;
        pkt.cmd = cmds[player];
        pkt.type = CMD;
        Serialise((unsigned char *)buff, &pkt);
        if (sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&si_other,
                   sizeof(si_other)) < 0) {
          fprintf(stderr, "Failed to send data from the socket: %s\n",
                  strerror(errno));
        };
      }
    }

    WinRender(&state);
  }

  /* Close the socket */
  if (close(sockfd) < 0) {
    perror("close() failed");
    exit(EXIT_FAILURE);
  }
  WinFini();
  return 0;
}
