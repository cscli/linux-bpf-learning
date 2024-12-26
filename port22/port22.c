#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>  // 包含 ethhdr 定义
#include <linux/ip.h>        // 包含 iphdr 定义
#include <linux/tcp.h>       // 包含 tcphdr 定义
#include <linux/in.h>        // IPPROTO_TCP 定义

SEC("xdp")
int xdp_prog(struct xdp_md *ctx) {
    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;

    // 解析以太网头部
    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return XDP_DROP;

    // 检查是否为IP流量
    if (eth->h_proto != htons(ETH_P_IP))
        return XDP_PASS; // 或者其他处理逻辑

    // 解析IP头部
    struct iphdr *iph = data + sizeof(struct ethhdr);
    if ((void *)iph + sizeof(struct iphdr) > data_end)
        return XDP_DROP;

    // 检查IP版本和头部长度
    if (iph->version != 4 || iph->ihl < 5)
        return XDP_PASS; // 或者其他处理逻辑

    // 解析TCP头部
    struct tcphdr *tcph = (struct tcphdr *)((u8 *)iph + (iph->ihl * 4));
    if ((void *)tcph + sizeof(struct tcphdr) > data_end)
        return XDP_DROP;

    // 检查是否为TCP流量
    if (iph->protocol == IPPROTO_TCP) {
        // 检查目的端口或源端口是否为22（SSH）
        __be16 port_22 = bpf_htons(22);
        if (tcph->dest == port_22 || tcph->source == port_22)
            return XDP_PASS; // 允许进入内核处理
    }

    // 其他流量直接丢弃或重传
    return XDP_DROP; // 或者 XDP_TX
}

char _license[] SEC("license") = "GPL";

