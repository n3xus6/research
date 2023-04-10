# PowerPC emulation
Goal: emulate hardware that uses **big-endian byte-ordering**. \
Emulator: [QEMU](https://www.qemu.org/)

## Needed files
1. Debian Image for PowerPC: [Debian for PowerPC DVD](https://cdimage.debian.org/cdimage/archive/8.11.0/powerpc/iso-dvd/debian-8.11.0-powerpc-DVD-1.iso)
2. initrd, vmlinux: mount the DVD .iso image and extract initrd.gz and vmlinux from the install\powerpc directory. Unpack initrd.gz to initrd.
3. hard disk image: `qemu-img create -f qcow2 ppc.img 20G`

## Install OS

`qemu-system-ppc.exe -L pc-bios -boot d -M mac99,via=pmu -m 1024 -hda C:\qemu_vm\ppc.img -initrd c:\qemu_vm\initrd -kernel c:\qemu_vm\vmlinux -cdrom c:\qemu_vm\debian-8.11.0-powerpc-DVD-1.iso`

When asked which packages to install only choose the basic system and remove the selection for desktop environment.

The abvove command assumes that all needed files are stored inside `C:\qemu_vm`. Use whatever you want.

## Boot OS

`qemu-system-ppc -L pc-bios -M mac99,via=pmu -m 1024 -cdrom c:\qemu_vm\debian-8.11.0-powerpc-DVD-1.iso -hda C:\qemu_vm\ppc.img -boot c -prom-env "boot-device=hd:,\yaboot" -prom-env "boot-args=conf=hd:,\yaboot.conf`

After the OS booted and you loged in you can install additional software packages from the .iso image. For example to install gcc and it's dependencies you can use `sudo apt install gcc`.

## Big-endian and little-endian differences

The following code example shows how to read integer values independent from the byte-ordering of the machine.

```
unsigned char mem_be_511[4] = { 0x00, 0x00, 0x01, 0xFF }; // value 511 stored in big-endian byte-order
unsigned char mem_le_511[4] = { 0xFF, 0x01, 0x00, 0x00 }; // value 511 stored in little-endian byte-order
unsigned int val = 0;
int i = 0;

// Issue: wrong value on little-endian x86 architecture, correct value on big-endian PowerPC architecture
val = *((unsigned int*)mem_be_511);
printf("%u\n", val);

// Solution: correct value on both architectures, x86 and PowerPC.
for (i = 0, val = 0; i < 4; i++) val |= mem_be_511[i] << (24 - i * 8);
printf("%u\n", val);

// Issue: wrong value on big-endian PowerPC architecture, correct value on little-endian x86 architecture
val = *((unsigned int*)mem_le_511);
printf("%u\n", val);

// Solution: correct value on both architectures, x86 and PowerPC.
for (i = 0, val = 0; i < 4; i++) val |= mem_le_511[i] << (i * 8);
printf("%u\n", val);
```

All links and steps tested on 10.04.2023.
