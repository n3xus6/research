# Linux kernel, initramfs

Protocol of the kernel build process and related tasks.

## Compiling a tiny Linux kernel

Compile a Linux kernel with only the required features to load a simple hello world programm. Uses the linux-5.3.12 source tree.

1. `linux-5.3.12$ make O=/path/build tinyconfig`

    Creates a base config with only the minimum options enabled

1. `linux-5.3.12$ make O=/path/build menuconfig`

    To run a binary at startup and have a console to print something a few additional options must be selected from the kernel config menu:
    * Initial RAM filesystem and RAM disk (initramfs/initrd) support
    * 64-bit kernel
    * Enable TTY
    * Kernel support for ELF binaries
1. `linux-5.3.12$ make O=/path/build`

        Kernel: arch/x86/boot/bzImage is ready

All Makefile targets are listed in _Documentation/admin-guide/README.rst_.

The "flex" parser is needed to compile the kernel. Besides that it worked right out of the box.

## Creating a dummy initramfs
To check if the compiled kernel is loading and working, a message should be printed to the console using the following simple c program.

`$ cat hello.c`

    #include <stdio.h>
    int main(void) {
	    printf("Hello!\n");
	    return 0;
    }

Must be compiled with _static_ since there is no libc library implementation at the time the program is running. The binary must be named _init_ in order to get executed by the kernel. In actuality, init is the root process, all other processes are child processes of init. For testing, init just prints a greeting and nothing more.

`$ gcc -static hello.c -o init`

As next, a "initramfs" archive has to be build. Therefore the "_init_" binary is copied to a cpio-format archive:

`$ echo init | cpio -o -H newc --quiet -F initramfs-test.cpio`

The kernel extracts this archive into a block of ram and starts the _init_ process from there. It uses the "ramfs" filesystem to read the data (the old way was using a "ram disk" but an initramfs image requires less filesystem overhead). The initramfs image  (also called early userspace image) should contain a root filesystem (rootfs) with the programs required to bring up the system (for example using the busybox toolset), mounting the actual root file system and allowing the user to login to the system.

## Running the kernel

Using qemu to quickly test if the tiny custom kernel can run the init program.

`$ qemu-system-x86_64 -kernel ./build/arch/x86/boot/bzImage -initrd initramfs-test.cpio`

    SeaBIOS (version 1.10.2-1ubuntu1)

    iPXE (http://ipxe.org) ...

    Booting from ROM...
    Hello!

The "Hello!" message comes from the init (hello.c) program.

## A real initramfs image

The initramfs image from xubuntu, for example, is located here:

    /boot/initrd.img-5.0.0-37-generic

But only one file is extracted (early load microcode):

`$ mkdir extract && cpio -i --quiet -D extract -F initrd.img-5.0.0-37-generic`

    $ tree extract/
    extract/
    └── kernel
        └── x86
            └── microcode
                └── AuthenticAMD.bin

It shows that the image file is not just a simple cpio-format archive as created before. However when installed the "initramfs-tools" package a tool is at hand to restore the rootfs from this image.

`$ unmkinitramfs initrd.img-5.0.0-37-generic extract`

    $ tree -L 2
    .
    ├── early
    │   └── kernel
    └── main
        ├── bin
        ├── conf
        ├── etc
        ├── init
        ├── lib
        ├── lib64
        ├── run
        ├── sbin
        ├── scripts
        ├── usr
        └── var

## Compiling against uClibc

* uClibc stands for micro (u) controller (C) C library (libc)
* C library for embedded Linux
* the goal is providing as much functionality as possible in a small amount of space
* some space savings cost performance and features
* it is many times smaller than glibc


### uClibc cross-compiler

Download "cross-compiler-x86_64.tar.bz2" from the uClibc homepage and unpack it. Add the bin folder to the PATH variable.

`$ PATH=${PATH}:/path/cross-compiler-x86_64/bin`

`$ x86_64-gcc --version`

    uClibc x86_64-rawgcc (GCC) 4.1.2

Compile and statically link the simple hello world program with uClibc:

`$ x86_64-gcc -static hello.c -o ./hello`

`$ ls -s hello`

    20 hello

The binary has a size of 20 kilobytes. Now lets check with glibc:

`$ gcc -static hello.c -o ./hello`

`$ ls -s hello`

    828 hello

The result is a binary of size 800 kilobytes which is 40x times larger.

The binaries for the early rootfs could also be compiled with uClibc to shrink it's size.

# Busybox

`./busybox`

	[...]
    BusyBox is a multi-call binary that combines many common Unix
	utilities into a single executable.  Most people will create a
	link to busybox for each function they wish to use and BusyBox
	will act like whatever it was invoked as.
    [...]

The easy way of building busybox with uClibc is to use a pre-build toolchain.

A fitting toolchain for a x86-64 PC which uses uClibc-ng can be found on this site:

[toolchains.bootlin.com](https://toolchains.bootlin.com/)

File: _x86-64-core-i7--uclibc--stable-2018.11-1.tar.bz2_

Next, adjust the busybox config to this toolchain:

`busybox-1_31_1$ make O=/path/output defconfig`

`busybox-1_31_1$ make O=/path/output menuconfig`

* Cross compiler prefix (**CONFIG_CROSS_COMPILER_PREFIX**)

    _x86_64-linux-_

* Sysroot path (**CONFIG_SYSROOT**)

    _/path/x86-64-core-i7--uclibc--stable-2018.11-1/x86_64-buildroot-linux-uclibc/sysroot_

* Build static binary (no shared libs) (**CONFIG_STATIC**)

    _yes_

The _bin_ folder of the toolchain must be added to PATH.

`PATH=${PATH}:/path/x86-64-core-i7--uclibc--stable-2018.11-1/bin`

Then start the build:

`busybox-1_31_1$ make O=/path/output`

The resulting static linked busybox binary has a size of **1,2M**.

`output$ ./busybox`

    BusyBox v1.31.1 (2020-01-05 12:39:55 CET) multi-call binary.

_**Using glibc shows a binary size of 2,9M. So linking against uClibc-ng results in a binary below half of the size.**_

# References

[Linux kernel sources](https://www.kernel.org/)

[The Linux Kernel documentation](https://www.kernel.org/doc/html/latest/)

Documentation folder inside the kernel source tree:
* Documentation/admin-guide/README.rst
* Documentation/admin-guide/initrd.rst
* Documentation/filesystems/ramfs-rootfs-initramfs.txt
* Documentation/driver-api/early-userspace/early_userspace_support.rst

[linux from scratch](http://www.linuxfromscratch.org/)

[The Linux Bootdisk HOWTO](http://www.tldp.org/HOWTO/Bootdisk-HOWTO/)

[From Power Up To Bash Prompt](http://www.tldp.org/HOWTO/From-PowerUp-To-Bash-Prompt-HOWTO.html)

[Busybox](https://busybox.net/)

[QEMU](https://www.qemu.org/)

[uClibc](https://www.uclibc.org/)

[uClibc-ng - Embedded C library](https://uclibc-ng.org/)

[toolchains.bootlin.com](https://toolchains.bootlin.com/)
