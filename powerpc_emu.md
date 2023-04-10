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
