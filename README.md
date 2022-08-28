# PS3 Pad MacOS
Simple program to allow USB-connected PS3 controllers to work on M1 / M2 macs. (Works as of August 2022)

I didn't write the source code, but I've added comments to hopefully make what is happening a bit clearer. I've also chosen not to include a pre-compiled binary, because it's safer as a user to compile the source directly.

## Compiling
Compile the C code with `clang` or `gcc`

> `-o` configures the output file, so feel free to change this to whatever works

```bash
clang -framework CoreFoundation -framework IOKit ps3pad.c -o ps3pad
```

```bash
gcc -framework CoreFoundation -framework IOKit ps3pad.c -o ps3pad
```

## Usage
Directly execute the binary

```bash
./ps3pad
```

## Acknowledgements
I found the original source code in [a bug raised against SDL](https://github.com/libsdl-org/SDL/issues/4923#issuecomment-966722634) - I just removed the annoying rumble when activating the controller.

As such, I've left the original copyright notice in the source code.

## Caveats
I've only used this to allow my PS3 controller to work with [dolphin](https://github.com/dolphin-emu/dolphin), and it works well, however caveats I've noticed:
- No rumble
- No pressure-sensitive triggers

However this probably varies depending on the software, e.g. I think [RPCS3](https://github.com/RPCS3/rpcs3) might have their own custom implementation which doesn't need this activation.

And [SDL](https://github.com/libsdl-org/SDL) is supposed to support PS3 pads now too, so any software bundling that likely works too.
