# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Purpose

Self-contained PlatformIO / pioarduino template for defining a **custom ESP32 board variant** entirely inside the project repo — no fork of `framework-arduinoespressif32` required. Uses pioarduino (community fork of Espressif's PlatformIO platform, Arduino 3.x / ESP-IDF 5.x).

## Commands

```sh
pio run                 # build
pio run -v              # verbose — shows -I order; verify project variants/ comes before framework variants/
pio run -t upload       # flash (requires upload port)
pio device monitor      # serial @ 115200; main.cpp prints ARDUINO_VARIANT on boot for sanity-check
pio run -t clean
```

Single env defined: `my_board`. Add `-e <env>` if more envs are added later.

## Architecture — how the variant redirect works

The whole project exists to exploit two lines in `tools/pioarduino-build.py` (upstream `espressif/arduino-esp32`):

```python
variants_dir = join(FRAMEWORK_DIR, "variants")
if "build.variants_dir" in board_config:
    variants_dir = join("$PROJECT_DIR", board_config.get("build.variants_dir"))
```

Three pieces wire together; changing any one without the others breaks the redirect:

1. **`platformio.ini`** sets `board_dir = ${PROJECT_DIR}/boards` (so `board = my_board` resolves to `boards/my_board.json` instead of a built-in) and overrides `board_build.variants_dir` + `board_build.variant`.
2. **`boards/my_board.json`** declares `build.variants_dir: "variants"` (relative to `$PROJECT_DIR`) and `build.variant: "my_board"`. Folder name under `variants/` must match `build.variant` **exactly, case-sensitive**.
3. **`variants/my_board/pins_arduino.h`** is the only hardware description file. Included by `Arduino.h` *before* `Arduino.h` defines pin-count macros.

The variant folder is consumed by both C and C++ translation units inside the framework, which dictates the header rules below.

## Editing `pins_arduino.h` — rules from real build failures

- **Do not define `NUM_DIGITAL_PINS` / `NUM_ANALOG_INPUTS`.** `Arduino.h` sets them from `SOC_GPIO_PIN_COUNT` / `SOC_ADC_CHANNEL_NUM()` after including this header. Redefining produces `-Wmacro-redefined` on every TU.
- **`#include <stdint.h>` explicitly.** Header is pulled into `.c` files (e.g. `esp32-hal-gpio.c`) where `uint8_t` is not implicit.
- **`A*` / `T*` aliases**: use `static const uint8_t`, not `#define` — avoids ODR issues across TUs.
- ADC2 pins are unusable while Wi-Fi is active — don't expose them as default ADCs if Wi-Fi is in use.

## Renaming the board

Rename `my_board` everywhere — directory `variants/my_board/`, `boards/my_board.json` filename, `variant`/`variants_dir` fields inside the JSON, `board` + `board_build.variant` in `platformio.ini`, and `-DARDUINO_MY_BOARD` in `extra_flags`. Use lowercase letters, digits, underscores only — **no hyphens** (breaks the `ARDUINO_<NAME>` macro).

## Gotcha table (from README)

| Symptom | Cause | Fix |
|---|---|---|
| `NUM_DIGITAL_PINS` redefined on every TU | Defined in `pins_arduino.h` | Remove; let `Arduino.h` set it |
| `unknown type name 'uint8_t'` in `.c` files | Missing `<stdint.h>` | Add include |
| `ARDUINO_VARIANT` prints `esp32` at runtime | `variants_dir` path wrong/absolute | Must be relative to `$PROJECT_DIR` |
| Build uses upstream pins | `board_build.variant` ≠ folder name | Match exactly, case-sensitive |

## Reproducibility

`platform = .../stable/platform-espressif32.zip` tracks moving `stable`. Pin to a release tag for reproducible builds.
