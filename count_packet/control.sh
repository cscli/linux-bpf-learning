#!/bin/bash

INTERFACE=ens38
PROGRAM=xdp_stats.o
SECTION=xdp_ss

if [ "$1" == "load" ]; then
    echo "Loading XDP program..."
    sudo ip link set dev $INTERFACE xdp obj $PROGRAM sec $SECTION
elif [ "$1" == "unload" ]; then
    echo "Unloading XDP program..."
    sudo ip link set dev $INTERFACE xdp off
else
    echo "Usage: $0 {load|unload}"
fi




