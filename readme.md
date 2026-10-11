# VoidFetch

A simple, fastfetch inspired, system info tool written in C

## Features

- **username and hostname**: shows username and host name on top
- **SHELL**: Shows login SHELL
- **OS & Kernel detection**: tells Linux distro and kernel, using ``<sys/utsname>``, os detection comes from ``/etc/os-release``
- **CPU info**: tells CPU name model, clock speed and threads, using stdio file read tools (fopen, fgets, fclose, ...)
- **GPU detection**: Actual GPU detection that was a HELL to make, detect multiple GPUs by reading ``/sys/bus/pci/devices``, doesnt use lspci anymore no need for extra dependencies
- **RAM info**: Total Ram available for the system and usage
- **Motherboard detection**: tells you the model
- **Total disk space and usage**: just like fastfetch ``disk`` shows used space and total
- **Uptime**: Just like the name suggests it shows the time the system has been on
- **ASCII logos**: What a surprise another neofetch/fastfetch like tool has logos in ASCII
- **Lightweight**: instead of being thousands of lines of code like fastfetch this one is just a few hundreds, enjoy your saving of 2 bytes of ram usage to run it
- **Fast execution**: reads everything from `/proc`, `/sys` and `/etc`, with no external commands. A run takes a few milliseconds (measured about 2 ms on a Ryzen 5 7600X).


## Why VoidFetch

I don't have any idea, i mean it's just fastfetch or neofetch but worse.

The only thing i can think of is liking the cringy VoidFetch name

## AI usage

To learn file handling and like 90% of header files I used to make it(for example: string.h)

Used claude help to write multi GPU detection, not a single line was pasted, the ones that were copied were pure hand copy and altered by me

## How to run

Linux only for now. Only tested on Ubuntu 26.04 LTS.

You need `gcc` and `make`. Nothing else is required at runtime.

First clone the repo and cd into it:

```bash
git clone "https://github.com/otzpt/VoidFetch" && cd VoidFetch
```

### Option 1: build and run from the folder

```bash
make
./voidfetch
```

`make run` builds (if needed) and runs in one step. `make clean` removes the build files.

### Option 2: install for your user

```bash
chmod +x install.sh && ./install.sh
```

This compiles the program and copies it to `~/.local/bin/voidfetch`. Then open a terminal and run:

```bash
voidfetch
```

If the command is not found, make sure `~/.local/bin` is in your `PATH`.


## Limitations

Only tested on Ubuntu LTS 26.04

Only works on linux

If file ``pci.ids`` doesnt exist GPU detection doesnt work

ONLY nvidia, amd and intel

shows login shell

some functions still print errors if file doesnt open
 
To change settings and logos you need to know at least basic/intermediate C or just ask AI

was made in 2 days and 10 hours so it's not polished

BTW these limitations apart from the test one are supposed to disappear so pls hang on tight

## Responsibility

I'm not responsible for anything that happens to your system

## License

## License

[GPLv3](LICENSE)
