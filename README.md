
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

# RTL8139 Protocol Handler Registration and Organization

![protocol_handler_organization](https://user-images.githubusercontent.com/24198081/118883512-39b73500-b8ed-11eb-8366-3c41f9b1ff56.png)



# RafaOS Network Stack:
TODO
The idea is to create whole network stack based on Linux kernel structure.

The following networking diagram was created based on my interpretation of reading Kernel source code version 3.19. It doesn't cover all functions, only the most important ones for IP and XFRM.

![kernel_xfrm_fwd (1)](https://user-images.githubusercontent.com/24198081/118876712-797a1e80-b8e5-11eb-8bda-85937a8be9f8.png)

# RafaOS Routing Subsystem

Routing engine implemented will based on Linux kernel 3.6+ LPC-trie. Diagram below should ilustrate the structures and relatiotionship between each other based on my understanding of Linux kernel:

![routing_trie](https://user-images.githubusercontent.com/24198081/119225986-def52780-bafe-11eb-9666-70b070d8ad66.png)


References: 
https://elixir.bootlin.com/linux/v4.0/source/net/ipv4/fib_trie.c
https://vincent.bernat.ch/en/blog/2017-ipv4-route-lookup-linux


