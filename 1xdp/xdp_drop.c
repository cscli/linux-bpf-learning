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

/*


struct xdp_md *ctx 
	是 XDP（eXpress Data Path）程序中的一个关键结构体指针，它提供了关于当前数据包及其元数据的信息。
	这个结构体是 XDP 程序与内核之间的接口，允许 XDP 程序访问和操作数据包的各个部分
/usr/src/linux-headers-6.11.0-13/include/uapi/linux/bpf.h 中 struct xdp_md {
         __u32 data;
         __u32 data_end;
         __u32 data_meta;
         // Below access go through struct xdp_rxq_info 
         __u32 ingress_ifindex; // rxq->dev->ifindex 
         __u32 rx_queue_index;  // rxq->queue_index  
 
         __u32 egress_ifindex;  // txq->dev->ifindex 
 };
 struct xdp_rxq_info {
        struct net_device *dev;
        u32 queue_index;
        u32 reg_state;
        struct xdp_mem_info mem;
        unsigned int napi_id;
        u32 frag_size;
} ____cacheline_aligned; // perf critical, avoid false-sharing 

struct xdp_txq_info {
        struct net_device *dev;
};

字段解释
__u32 data
	描述：指向数据包的起始位置的指针。这个指针通常是数据包缓冲区的内存地址，XDP 程序可以通过这个地址访问数据包的内容。
	用途：XDP 程序通过该指针读取和修改数据包的负载。
__u32 data_end
	描述：指向数据包的结束位置的指针。这个指针标识了数据包的有效结束位置，通常是数据包的尾部。
	用途：用于确保读取数据时不会超出数据包的界限，从而避免内存越界访问。
__u32 data_meta
	描述：指向与数据包相关的元数据的指针。该指针可以用于存储与数据包处理相关的额外信息。
	用途：XDP 程序可以使用此字段来获取或设置与数据包处理相关的元数据。
__u32 ingress_ifindex
	描述：表示接收到数据包的网络接口的索引。你可以使用这个值来确定数据包是从哪个网卡到达的
	用途：用于多网卡环境下的流量路由决策，这对于流量分析和处理非常重要。
__u32 rx_queue_index
	描述：表示数据包进入的接收队列的索引。对于支持多队列的网卡，用于标识数据包是从哪个接收队列接收到的。
	用途：在多队列网络接口中，能够帮助确定接收数据包的具体队列，有助于实现负载均衡和流量调度。
__u32 egress_ifindex
	描述：发送接口的索引，表示数据包将要发送的网络接口。这个字段主要用于 XDP_TX 操作，即当 XDP 程序决定将数据包重新发送回同一网卡时。
	用途：用于指定发送数据包的目标网卡，这对数据包的转发和路由非常重要。
总结
struct xdp_md 提供了 XDP 程序访问和处理网络数据包所需的关键信息。通过这个结构体，XDP 程序能够有效地读取数据包内容、
确保安全访问、并利用接口和队列信息进行高效的网络流量管理。这些字段在 XDP 的高性能数据处理和网络优化中起着重要作用。

*/
