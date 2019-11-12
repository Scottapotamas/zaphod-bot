# Embedded Firmware Readme

STM32F4 firmware for the zaphod-controller PCB. Manages the delta's servos and handles movement processing.

Uses the CubeMX HAL as the starting base, with my own HAL on top of (or replacing) as applicable. Written for the STM32F429VET6 target microcontroller.

General system runs with event driven HSM design patterns, while background tasks use somewhat simple statemachines to handle operation.

## Building

Uses the `arm-gcc-eabi-none` toolchain, should build with fairly standard flags.



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