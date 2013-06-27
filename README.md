# ACL - ARM crypto library

## About

A cryptographic library optimized for the ARM7TDMI architecture. The library includes the AES, SHA, RSA, some PRNGs and ECC (ECDSA) over all the SECG-recommended curves. The library is written in GNU assembler and C, the testing is done in Keil uVision3.

## Authors

**Author** - Jaroslav Bán
<br>
Technical University of Košice
<br>
Faculty of Electrical Engineering and Informatics (FEI)
<br>
Department of Electronics and Multimedia Communications (DEMC)
<br>
**Supervisor** - Assoc. Prof. Miloš Drutarovský, Ph.D.

## License
Unknown. Original code was pulled out from Jaroslav Bán's [master thesis](http://www.kemt.fei.tuke.sk/personal/drutarovsky/students/pdfs/ban2007.pdf).
Also, library code was uploaded by author to [Programmersheaven.com](http://www.programmersheaven.com/download/52999/Download.aspx), but [download link](http://web.omnidrive.com/APIServer/public/Dog4VIJWMbOTFzupAMhMstsd/Cryptographic%20library%20for%20ARM7TDMI%20processors.zip) is broken. So, lets assume it is public domain.

## Changelog

### 2007-04-24

Version 1.0.0

### 2007-05-15

Date of PDF publication

### 2013-06-26

Pull out from PDF, fix Makefile

### 2013-06-27

Add README, GitHub upload


## Build instructions

Tested only in linux with CodeSourcery's and Linaro toolchains
Original code was tested on Keil MCB2130 board with NXP LPC2138.
Simulation was done in Keil uVision3 simulator/debugger.

I have not tested it on real hardware, but everything should work.

To build library, run:

    make CROSS_COMPILE=arm-linux-gnueabihf-

To build test application, run:

    make CROSS_COMPILE=arm-linux-gnueabihf- test

You should specify correct cross compilation prefix.
Default is `arm-linux-gnueabihf-`.

You can get Linaro GCC toolchain here:
[source](https://launchpad.net/gcc-linaro),
[binaries](https://launchpad.net/linaro-toolchain-binaries).

Everything was tested with version 2013.05:
[source](https://launchpad.net/gcc-linaro/+milestone/4.8-2013.05),
[binaries](https://launchpad.net/linaro-toolchain-binaries/+milestone/2013.05).

Direct link to prebuilt toolchain:
[gcc-linaro-arm-linux-gnueabihf-4.8-2013.05_linux.tar.xz](https://launchpad.net/linaro-toolchain-binaries/trunk/2013.05/+download/gcc-linaro-arm-linux-gnueabihf-4.8-2013.05_linux.tar.xz).

## TODO

* Add more functionality
* Add more tests
* Support different test environments
