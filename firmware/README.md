# Embedded Firmware Readme

STM32F4 firmware for the zaphod-controller PCB. Responsible for managing the delta's servos, handles movement processing, etc.

Written for the STM32F429VET6 target microcontroller.

FreeRTOS is used, and ST's LL (LowLayer) HAL backs my own 'HAL layer'.

## Building

Uses the `arm-gcc-eabi-none` toolchain with CMake & `stm32-cmake` helpers.

External libraries/deps `freeRTOS`, `electricui-embedded`, `electricui-interval-sender`, etc use git submodules.

`.clang-format` config file is under git.

GitHub Actions workflows attempt to build firmware on commits against master, and when tagged. 

## Flashing and Debugging

I use CLion as my IDE alongside a JLink. Opening the `/firmware` folder with CLion and reloading the CMake project should get everything setup and ready to run.

Depending on the version, either run a remote GDB server, or use CLion's integrated support.

### Optional CMake args

I provide the following CMake options manually:

```
-DSTM32_TOOLCHAIN_PATH=/usr/bin/
-DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/arm-none-eabi-g++
-DCMAKE_C_COMPILER:FILEPATH=/usr/bin/arm-none-eabi-gcc
```

### CLion Integration

- Create an "Embedded GDB Server" run configuration,
- Name it, find the built binary,
- 'target remote' args: `tcp:localhost:2331`
- GDB server: `/opt/SEGGER/JLink/JLinkGDBServer`
- args: `-if SWD -device STM32F429VG`

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