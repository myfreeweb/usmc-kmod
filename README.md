# usmc-kmod

FreeBSD Kernel module for doing ARMv8 Secure Monitor Calls from userspace.

For obvious reasons, don't load on Production™ systems, don't use on shared systems, etc.

## Usage

```shell
make -j4
kldload ./usmc.ko
cc -o smctool smctool.c

# PSCI: version
./smctool 0x84000000 0x0 0x0 0x0
10001
0
0
0

# Marvell Armada8k: MV_SIP_DRAM_SIZE
./smctool 0x82000010 0x0 0x0 0x0
60000000
0
0
0
```

[source for Armada8k](https://github.com/MarvellEmbeddedProcessors/atf-marvell/blob/1f8ca7e01d4ac7023aea0eeb4c8a4b98dcf05760/plat/marvell/common/mrvl_sip_svc.c)
