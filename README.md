# my-esp32-board

A self-contained PlatformIO / pioarduino project template for defining a **custom ESP32 board variant** without modifying or submitting a pull request to any upstream repository.

## Why this exists

PlatformIO's Espressif Arduino build system (`pioarduino-build.py`) resolves pin definitions through a *variant* directory mechanism. The canonical path is inside the framework package:

```
~/.platformio/packages/framework-arduinoespressif32/variants/<variant>/pins_arduino.h
```

This template redirects that lookup to a local `variants/` folder inside your project, so your board definition lives entirely in your own repo.

**Source reference** — the relevant lines in the upstream build script:

```python
# espressif/arduino-esp32 · tools/pioarduino-build.py
variants_dir = join(FRAMEWORK_DIR, "variants")

if "build.variants_dir" in board_config:
    variants_dir = join("$PROJECT_DIR", board_config.get("build.variants_dir"))
```

## Repository layout

```
my-esp32-board/
├── platformio.ini                   PlatformIO project and build config
├── boards/
│   └── my_board.json                Board manifest (MCU, flash, upload, variant name)
├── variants/
│   └── my_board/
│       └── pins_arduino.h           Pin definitions — the only file you need to edit
├── src/
│   └── main.cpp                     Minimal firmware entry point
└── README.md
```

## Files explained

### `boards/my_board.json`

The board manifest consumed by PlatformIO. The two fields that make the variant redirect work:

```json
"variant":      "my_board",
"variants_dir": "variants"
```

`variants_dir` is relative to `$PROJECT_DIR`. PlatformIO merges this with any `board_build.*` overrides from `platformio.ini`.

### `platformio.ini`

```ini
board_dir             = ${PROJECT_DIR}/boards
board_build.variants_dir = variants
board_build.variant      = my_board
```

`board_dir` tells PlatformIO where to find `my_board.json`.
`board_build.variants_dir` and `board_build.variant` override the board JSON at build time (useful when you want to share one JSON across multiple environments with different pin layouts).

### `variants/my_board/pins_arduino.h`

The only file that describes your hardware. Use `static const uint8_t` for `A*` and `T*` aliases rather than `#define` — avoids ODR issues when the symbol appears in multiple translation units. See [Known gotchas](#known-gotchas) for other rules derived from real build failures.

## Customising for your board

1. **Rename** every occurrence of `my_board` (directory names, JSON fields, `platformio.ini` values) to your board's identifier. Use only lowercase letters, digits, and underscores — no hyphens.
1. **Edit `variants/my_board/pins_arduino.h`** — set the correct GPIO numbers for your schematic:

   - `LED_BUILTIN`
   - SPI (`PIN_SPI_SS`, `PIN_SPI_MOSI`, `PIN_SPI_MISO`, `PIN_SPI_SCK`)
   - I²C (`PIN_WIRE_SDA`, `PIN_WIRE_SCL`)
   - UART (`PIN_SERIAL_RX`, `PIN_SERIAL_TX`)
   - ADC aliases (`A0`–`A19`) — remove any that don't exist on your board
   - Touch aliases (`T0`–`T9`) — remove any that don't exist on your board
   - Note: ADC2 pins are unavailable while Wi-Fi is active
1. **Edit `boards/my_board.json`** — set:

   - `"mcu"` — e.g. `esp32`, `esp32s3`, `esp32c3`
   - `"f_cpu"` — CPU frequency in Hz as a string (`"240000000L"`)
   - `"flash_size"`, `"maximum_size"`, `"maximum_ram_size"` — match your module
   - `"upload.speed"` — baud rate for flashing
1. **Build and verify:**

```sh
pio run -v
```

The verbose flag shows the exact `-I` paths the compiler uses. Confirm your `variants/my_board/` folder appears *before* the framework's own `variants/` folder in the include search order.

5. **Sanity-check at runtime** — `main.cpp` prints the resolved variant name on boot:

```cpp
Serial.printf("Variant: %s\n", ARDUINO_VARIANT);
// Should print: Variant: my_board
```

## Platform

Uses [pioarduino](https://github.com/pioarduino/platform-espressif32) — a community-maintained fork of the Espressif PlatformIO platform with current Arduino 3.x / ESP-IDF 5.x support.

```ini
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
```

Pin to a specific release tag instead of `stable` for reproducible builds.

## Known gotchas

| Symptom | Cause | Fix |
| --- | --- | --- |
| `"NUM_DIGITAL_PINS" redefined` warning on every TU | Defined in `pins_arduino.h` | Remove the define; `Arduino.h` sets it after including this header, via `SOC_GPIO_PIN_COUNT` / `SOC_ADC_CHANNEL_NUM()` |
| `unknown type name 'uint8_t'` in `.c` files | Missing `<stdint.h>` — header is pulled into C TUs like `esp32-hal-gpio.c` | Add `#include <stdint.h>` at top of header |
| Wrong variant loaded (`ARDUINO_VARIANT` prints `esp32`) | `board_build.variants_dir` path wrong or missing | Check path is relative to `$PROJECT_DIR`, not absolute |
| Build uses upstream pins instead of yours | `board_build.variant` name doesn't match folder name | They must be identical, case-sensitive |
