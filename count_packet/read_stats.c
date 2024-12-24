#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

#define MAP_PATH "/sys/fs/bpf/stats_map"
//   make -f makefile_r
//   make -f makefile_r  clean
struct stats_t {
    unsigned long long packets;
    unsigned long long bytes;
};

int main(int argc, char **argv) {
    int fd = bpf_obj_get(MAP_PATH);
    if (fd < 0) {
        perror("Failed to open map");
        exit(EXIT_FAILURE);
    }

    printf("Monitoring traffic on ens38...\n");
    while (1) {
        struct stats_t stats;
        memset(&stats, 0, sizeof(stats));
        if (bpf_map_lookup_elem(fd, &(unsigned int){0}, &stats) != 0) {
            perror("Failed to lookup element in map");
            close(fd);
            exit(EXIT_FAILURE);
        }
        printf("Packets: %llu, Bytes: %llu\n", stats.packets, stats.bytes);
        sleep(1);
    }

    close(fd);
    return 0;
}




