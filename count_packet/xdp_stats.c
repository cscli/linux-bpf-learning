#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/types.h>
#include <linux/if_ether.h>  // 包含 struct ethhdr 定义

struct stats_t {
    __u64 packets;
    __u64 bytes;
};

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32);
    __type(value, struct stats_t);
    __uint(max_entries, 1);
} stats_map SEC(".maps");

SEC("xdp_ss")
int xdp_stats(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    // Ensure the packet has at least an Ethernet header
    if (data + sizeof(struct ethhdr) > data_end) {
        return XDP_PASS;
    }

    // Increment the packet count
    __u32 key = 0;
    struct stats_t *stats = bpf_map_lookup_elem(&stats_map, &key);
    if (!stats) {
        return XDP_ABORTED;
    }

    stats->packets++;
    stats->bytes += ctx->data_end - ctx->data;

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";



