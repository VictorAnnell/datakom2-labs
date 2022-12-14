# Copyright (C) 2022  Xiaoyue Chen
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

SDL2_CONFIG = /usr/bin/sdl2-config

CFLAGS = -O3 -g -Wall $$($(SDL2_CONFIG) --cflags)

xpong: xpong.c simulate.o window.o network.o
	$(CC) -o xpong xpong.c simulate.o window.o network.o $(CFLAGS) $$($(SDL2_CONFIG) --libs)

.PHONY: clean
clean:
	rm -f *.o xpong
