# VoidFetch

A simple, fastfetch inspired, system info tool written in C

## Features

- **OS & Kernel detection**: tells Linux distro and kernel, using ``<sys/utsname>``
- **CPU info**: tells CPU name model, clock speed and threads, using stdio file read tools (fopen, fgets, fclose, ...)
- **GPU detection**: **FOR NOW A PLACE HOLDER THAT NEEDS EXTRA PACKAGES**, but works well, pkgs are ``lspci``, ``grep`` and ``sed``
- **RAM info**: Total Ram available for the system, doesn't show usage yet
- **Motherboard detection**: tells you the model
- **Total disk space and usage**: just like fastfetch ``disk`` shows used space and total
- **Uptime**: Just like the name suggests it shows the time the system has been on
- **ASCII logos**: What a surprise another neofetch/fastfetch like tool has logos in ASCII
- **Lightweight**: instead of being thousands of lines of code like fastfetch this one is just a few hundreds, enjoy your saving of 2 bytes of ram usage to run it
- **Fast execution**: just like any cli tool written in C that gets system info, its insta run (though the way it gets GPU model rn makes the program take an extra 400ms to display everything)

## Why VoidFetch

I don't have any idea, i mean it's just fastfetch or neofetch but worse.

The only thing i can think of is liking the cringy VoidFetch name

## AI usage

To learn file handling and like 90% of header files I used to make it(for example: string.h)

## How to run

So you have 2 options, both build it from scratch but one install it in your system the other is just generates a runnable file

### Notice

Linux only for now; **ONLY TESTED ON UBUNTU LTS 26.04**;

#### Back to how to run

first clone the repo and cd to it
```bash
git clone "https://github.com/otzpt/VoidFetch" && cd VoidFetch
```
then you run the install script with the necessary permissions
```bash
chmod +x install.sh && ./install.sh
```

or do this to run it without installing

```bash
gcc main.c -o voidfetch
```
```bash
./voidfetch
```

then you open another terminal or on the same
```bash
voidfetch
```

## Limitations

Only tested on Ubuntu LTS 26.04

To change settings and logos you need to know at least basic/intermediate C or just ask AI

Needs dependencies(lspci, grep, sed)

BTW these limitations apart from the test one are supposed to disappear so pls hang on tight

## Responsibility

I'm not responsible for anything that happens to your system

## License

GPLv3
