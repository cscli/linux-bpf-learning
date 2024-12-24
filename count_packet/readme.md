

# 1

## 编译
    直接make 命令编译 xdp_stats.c
    make -f makefile_r 编译 read_stats.c 
    control.sh 脚本加载和卸载

## 启动逻辑

  ```bash
./control.sh  load  加载xdp程序
bpftool map show 获取映射音系
bpftool map pin id 41 /sys/fs/bpf/stats_map   id映射到文件 /sys/fs/bpf/stats_map
./read_stats 执行,可以看到数据包个数统计和字节统计
  ```


## bpftool map show
    41: array  name stats_map  flags 0x0
            key 4B  value 16B  max_entries 1  memlock 264B
            btf_id 231

    逐个字段解释
    41:
    说明：这是映射的唯一标识符（ID）。在这个例子中，映射的ID是41。你可以使用这个ID来引用和操作该映射。
    array
    说明：这是映射的类型。array 表示这是一个数组类型的映射。其他常见的映射类型包括 hash、perf_event_array 等等。数组映射的特点是键是一个索引，值按照索引顺序存储。
    name stats_map
    说明：这是映射的名称。在这个例子中，映射的名称是 stats_map。名称用于在代码或命令行工具中引用映射。
    flags 0x0
    说明：这些是映射的标志位。0x0 表示没有设置任何特殊标志。可能的标志包括：
    0x1 (MAP_CREATE: 已弃用)
    0x2 (MAP_EXCL_GLOBAL)
    0x4 (MAP_PIN_ANY)
    0x8 (MAP_PIN_GLOBAL)
    0x10 (MAP_PERF_ARRAY_READ)
    0x20 (MAP_PREALLOC)
    0x40 (MAP_UNSAFE)
    0x80 (MAP_FROZEN)
    在这个例子中，没有任何特殊标志被设置。
    key 4B
    说明：这是映射键的大小（以字节为单位）。在这个例子中，键的大小是4字节（4B），意味着每个键是一个32位整数。
    value 16B
    说明：这是映射值的大小（以字节为单位）。在这个例子中，值的大小是16字节（16B），意味着每个值占用16个字节的内存空间。
    max_entries 1
    说明：这是映射的最大条目数。在这个例子中，映射最多可以包含1个条目。对于数组映射，这通常表示数组的长度。
    memlock 264B
    说明：这是映射所需的内存量（以字节为单位）。在这个例子中，映射需要264字节的内存。这包括了映射本身的开销以及所有条目的总和。
    btf_id 231
    说明：这是与映射关联的 BTF（BPF Type Format）ID。BTF 是一种元数据格式，用于描述BPF程序和映射的结构信息。ID 231 表示这个映射与ID为231的BTF记录相关联。这有助于调试和分析工具更好地理解映射的内容。
    总结
    这条命令的输出详细描述了一个名为 stats_map 的数组映射，其ID为41。它指定了键和值的大小、最大条目数、所需内存大小，并关联了一个BTF ID。这些信息对于管理和调试BPF程序非常重要，特别是当你需要确保映射配置正确且符合预期时。




# 常用命令
  ```bash
bpftool prog show        查看所有加载的BPF程序   

bpftool map show         bpftool来列出所有加载的BPF程序及其关联的映射
 
使用 bpftool 将映射挂载到一个指定的路径下, 挂载之后才能通过路径获取信息
sudo bpftool map pin id 11 /sys/fs/bpf/stats_map


sudo dmesg | tail  查看日志

sudo bpftool prog show name xdp_drop_icmp pinned /sys/fs/bpf/xdp_drop_icmp

sudo bpftool map dump pinned /sys/fs/bpf/tc/globals/icmp_count
 
ip link set dev ens38 xdp off  卸载xdp程序      用于禁用指定网络接口上的 eXpress Data Path (XDP) 程序


sudo bpftool prog list：
显示所有已经被load到系统里的eBPF程序的信息列表，除了显示功能外，还支持dump等功能，可以通过man bpftool prog来查看具体支持的功能。

bpftool net list
显示内核网络子系统里的eBPF程序，除了显示功能外，还支持其它功能，可以通过man bpftool net来查看具体支持的功能。

bpftool link list
显示所有激活的链接，除了显示功能外，还支持其它功能，可以通过man bpftool link来查看具体支持的功能。

bpftool perf list
显示系统里所有raw_tracepoint, tracepoint, kprobe attachments ，除了显示功能外，还支持其它功能，可以通过man bpftool perf来查看具体支持的功能。

bpftool btf list
显示所有BPF Type Format (BTF)数据 ，除了显示功能外，还支持其它功能，可以通过man bpftool btf来查看具体支持的功能。

bpftool map list
显示系统内已经载入的所有bpf map数据，除了显示功能外，还支持其它功能，可以通过man bpftool map来查看具体支持的功能。

bpftool feature probe dev eth0
查看eth0支持的eBPF特性 。
```