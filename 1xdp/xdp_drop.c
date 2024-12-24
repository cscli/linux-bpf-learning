#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

//#define SEC(NAME) __attribute__((section(NAME), used))

SEC("xdp_drop")
int xdp_drop_all(struct xdp_md *ctx) {
	    return XDP_DROP;
}

char _license[] SEC("license") = "GPL";


// clang -O2 -target bpf -c xdp_drop.c -o xdp_drop.o

// ip link set dev ens38 xdp obj xdp_drop.o sec xdp_drop
// ip link set dev ens38 xdp off     用于禁用指定网络接口上的 eXpress Data Path (XDP) 程序