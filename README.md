# ESP32 Game Boy Emulator

A compact Game Boy emulator written in C++ for ESP32-based hardware.

## Project Overview

This project emulates Nintendo Game Boy ROMs and runs them on an ESP32 board.
The emulator is designed for hobbyist hardware and prototype use, not as a finished product.

## Key Features

- Game Boy CPU and instruction execution
- Memory bank controller support for ROM banks
- Display output via an SPI-driven screen
- Joypad input support

## Limitations
- No audio emulation is included.
- Some Game Boy hardware features and mappers are not implemented.
- Save file handling is not included.

## Schematic

A hardware schematic is included in the `docs/` folder:

- `[docs/schematic.pn](https://github.com/tuliomoreira77/esp32-gb-emulator/blob/main/docs/schematic.png)`

Use the schematic as a reference for wiring the ESP32 board, display, and input connections.

