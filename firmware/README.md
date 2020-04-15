# Embedded Firmware Readme

STM32F4 firmware for the zaphod-controller PCB. Manages the delta's servos and handles movement processing.

Uses the ST LL (LowLayer) hal as a base, with my own HAL on top of (or replacing) as applicable. Written for the STM32F429VET6 target microcontroller.

General system runs with event driven HSM design patterns, while background tasks use somewhat simple statemachines to handle operation.

## Building

Uses the `arm-gcc-eabi-none` toolchain, should build with fairly standard flags.

Requires a copy of [a copy of `electricui-embedded`](https://github.com/electricui/electricui-embedded)  cloned into `vendor/electricui`, as this isn't included in the repo.

It might be required to remove or exclude the `electricui-embedded` sub-directories `test` and `examples`  from the build.

CLion workspace files are in the repo, so building from the CMakeLists should be reasonably straightforward.

`clang-format` config file is under git, used to maintain some semblance of style consistency.

## Flashing and Debugging

I use CLion as my IDE alongside a JLink.

Depending on the version, either run a remote GDB server, or use CLion's integrated support.

### Manually running GDB server

Run the JLinkGDBServer as the remote GDB server instance with 

`./JLinkGDBServer -if SWD -device STM32F429VG`

then in CLion, connect with a remote GDB debugger with target-remote args of `tcp:localhost:2331`.

I had to edit my `~/.gdbinit` to include flashing and reset commands, which looks something like this:

```
# Reset the chip to get to a known state.
define target hookpost-remote
load
monitor reset
monitor halt
b main

# Set auto JTAG speed
monitor speed auto
```