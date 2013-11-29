# ACL - ARM cryptographic library

## About

A cryptographic library optimized for the ARM7TDMI architecture.
The library includes the AES, SHA, RSA, some PRNGs and ECC (ECDSA) over all
the SECG-recommended curves. The library is written in GNU assembler and C,
the testing is done in Keil uVision3.


## Authors

### 2007

**Author** - Jaroslav Bán
<br>
Technical University of Košice
<br>
Faculty of Electrical Engineering and Informatics (FEI)
<br>
Department of Electronics and Multimedia Communications (DEMC)
<br>
**Supervisor** - Assoc. Prof. Miloš Drutarovský, Ph.D.

### 2013

**Maintainer** - Volodymyr Medvid <vmedvid@riseup.net>

## License
Unknown. Original code was pulled out from Jaroslav Bán's [master thesis][1].

Also, library code was uploaded by author to [Programmersheaven.com][2],
but [download link][3] is broken.

Some code for device support was taken from third-party software:

* Header for lpc213x is copied from [Keil uVision IDE][4] [distribution][5].
* [PL011][6] and [SP804][7] drivers for versatilepb are taken from
  [U-Boot][8] bootloader.


## Changelog

### 2007-04-24

  - Version 1.0.0

### 2007-05-15

  - PDF publication

### 2013-06-26

  - Pulled out from PDF
  - Fixed Makefile

### 2013-06-27

  - Added README
  - Uploaded to GitHub

### 2013-10-03

  - Fixed bugs
  - Build system improvements
  - New targets: versatilepb, generic


## Build instructions

Tested only in linux with CodeSourcery's and Linaro toolchains.


To build library, run:

    ./configure --cross-prefix=arm-none-eabi- --board=lpc213x --disable-debug
    make lib

To build test application, run:

    make test

To resolve build problems, enable verbose output with `V=1`:

    make V=1

You can get Linaro arm-none-eabi toolchain [here][9].

Everything was tested with version 4.7-2013-q2-update: [sources][10],
[linux binaries][11], [win32 binaries][12], [win32 installer][13].

## Test instructions

Original code was tested on Keil MCB2130 board with NXP LPC2138.
Simulation was done in Keil uVision3 simulator/debugger.

New targets are tested with [qemu][14].

There is also generic target available. It can be useful while adding
support for new target.

To run test application in qemu, you should configure it for
`arm-softmmu` target:

    ./configure --target-list=arm-softmmu --disable-blobs --disable-tools \
    --disable-sdl --disable-vnc --disable-kvm --disable-vde --disable-docs \
    --disable-curl --disable-curses --disable-bluez --disable-guest-base \
    --disable-pie --disable-linux-aio --disable-guest-agent --disable-virtfs \
    --disable-attr --disable-libusb --disable-uuid --disable-smartcard-nss \
    --disable-vhost-net --disable-vhost-scsi --disable-glx --disable-rbd \
    --disable-xfsctl --disable-libssh2 --disable-cap-ng --disable-seccomp \
    --disable-qom-cast-debug --disable-libiscsi --audio-drv-list=
    make
    make install

Run qemu as gdb server:

    qemu-system-arm -M versatilepb -m 256K  -gdb tcp:127.0.0.1:1234 -S \
    -nographic -kernel bin/test.elf

Configure ACL for Versatile Platform Baseboard:

    ./configure --board=versatilepb --enable-debug
    make test

Run debugging session:

    arm-none-eabi-gdb
    (gdb) file bin/test.elf
    (gdb) target remote localhost:1234
    (gdb) break main
    (gdb) continue

Script `.gdbinit` in project root directory automatically launches
these commands at gdb startup.

Note: main procedure loops infinitely, so you should manually stop qemu
instance after end of debugging session.
This can be done with `CTRL-A; X` shortcut.
If `CTRL-A` combination is not available (e.g. running in screen/tmux),
you should start qemu with alternate key modifier. For example,

    qemu-system-arm ... -echr 0x03

will assign `CTRL-C` as control sequence for qemu
(press `CTRL-C; X` to stop qemu instance).


[1]: http://www.kemt.fei.tuke.sk/personal/drutarovsky/students/pdfs/ban2007.pdf
[2]: http://www.programmersheaven.com/download/52999/Download.aspx
[3]: http://web.omnidrive.com/APIServer/public/Dog4VIJWMbOTFzupAMhMstsd/Cryptographic%20library%20for%20ARM7TDMI%20processors.zip
[4]: http://www.keil.com/uvision/
[5]: http://www.keil.com/dd/docs/arm/philips/lpc213x.h
[6]: http://git.denx.de/cgi-bin/gitweb.cgi?p=u-boot.git;a=blob;f=drivers/serial/serial_pl01x.c
[7]: http://git.denx.de/cgi-bin/gitweb.cgi?p=u-boot.git;a=blob;f=arch/arm/cpu/arm926ejs/versatile/timer.c
[8]: http://www.denx.de/wiki/U-Boot
[9]: https://launchpad.net/gcc-arm-embedded
[10]: https://launchpad.net/gcc-arm-embedded/4.7/4.7-2013-q2-update/+download/gcc-arm-none-eabi-4_7-2013q2-20130614-src.tar.bz2
[11]: https://launchpad.net/gcc-arm-embedded/4.7/4.7-2013-q2-update/+download/gcc-arm-none-eabi-4_7-2013q2-20130614-linux.tar.bz2
[12]: https://launchpad.net/gcc-arm-embedded/4.7/4.7-2013-q2-update/+download/gcc-arm-none-eabi-4_7-2013q2-20130614-win32.zip
[13]: https://launchpad.net/gcc-arm-embedded/4.7/4.7-2013-q2-update/+download/gcc-arm-none-eabi-4_7-2013q2-20130614-win32.exe
[14]: http://wiki.qemu.org
