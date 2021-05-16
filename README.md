
# RafaOS

Requirement: Cross-compiler and liker i386, nasm 
Coded and tested on qemu-system-i386

Goal: Create full network stack based on Linux kernel for study purposes.


TODO:
- Paging
- Enhanced Memory Allocator
- CPU Scheduling
- File System
# Networking: RTL8139 Registers and Memory Layout:

TODO:
- ARP 
- TCP/IP
- XFRM(IPSec)
- Firewall
  - Netfilter hooks
  - static NAT
  - Conntrack

![RTL8139_REGISTER_MEM (1)](https://user-images.githubusercontent.com/24198081/117540149-9ff6aa80-b005-11eb-9ba1-32a46ea31ab5.png)

# RTL8139 Receive Packet Handler Registers: receive_packet()

![rtl8139_rcv_packet (1)](https://user-images.githubusercontent.com/24198081/117544822-0df99c80-b01b-11eb-9c7b-559763840a96.png)

# RafaOS Network Stack:

The idea is to create whole network stack based on Linux kernel structure.

Below networking traffic flow was created based on reading Kernel source code version 3.19

![kernel_xfrm_fwd](https://user-images.githubusercontent.com/24198081/118400732-cd250780-b65a-11eb-96e6-b0d1533029c0.png)
