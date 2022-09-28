/*    Copyright (C) 2022  Robert Caamano   */
 /*
  *   This program deletes a rule from a pinned 
  *   zt_tproxy_map hash table
  *
  *   This program is free software: you can redistribute it and/or modify
  *   it under the terms of the GNU General Public License as published by
  *   the Free Software Foundation, either version 3 of the License, or
  *   (at your option) any later version.

  *   This program is distributed in the hope that it will be useful,
  *   but WITHOUT ANY WARRANTY; without even the implied warranty of
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *   GNU General Public License for more details.
  *   see <https://www.gnu.org/licenses/>.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syscall.h"
#include <arpa/inet.h>


int main(int argc, char **argv){
    union bpf_attr map;
    const char *path = "/sys/fs/bpf/tc/globals/zt_tproxy_map";
    char *endPtr;
    if (argc != 2) {
                fprintf(stderr, "Usage: %s <hex_dest_prefix ex. 0a000001>\n", argv[0]);
                exit(0);
        }
    int32_t key = htonl(strtol(argv[1],&endPtr,16));
    //open tproxy map
    memset(&map, 0, sizeof(map));
    map.pathname = (uint64_t) path;
    map.bpf_fd = 0;
    int fd = bpf(BPF_OBJ_GET, &map, sizeof(map));
    if (fd == -1){
        printf("BPF_OBJ_GET: %s\n", strerror(errno));
	exit(1);
    }
    //delete element with specified key
    map.map_fd = fd;
    map.key = (uint64_t) &key;
    int result = bpf(BPF_MAP_DELETE_ELEM, &map, sizeof(map));
    if (result){
       printf("MAP_DELETE_ELEM: %s\n", strerror(errno));
       exit(1);
    }
    close(fd);
}
