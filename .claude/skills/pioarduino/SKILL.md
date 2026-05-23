---
name: pioarduino
description: "PlatformIO for Arduino projects on ESP32 (via pioarduino fork). Use when user mentions PlatformIO, pio, platformio.ini, Arduino/ESP32 builds, pio run/upload, custom board variants, pins_arduino.h, or embedded Arduino development with PlatformIO."
---

# Pioarduino

## Overview

Guidance for PlatformIO-based Arduino development on ESP32, using the **pioarduino** community fork of `platform-espressif32` (current Arduino 3.x / ESP-IDF 5.x). Covers project setup, build/upload, and — most importantly — defining **custom board variants in-repo without forking `framework-arduinoespressif32`**.

## When to Use

- User runs / wants to run `pio run`, `pio run -t upload`, `pio device monitor`.
- User edits `platformio.ini`, `boards/*.json`, or `variants/*/pins_arduino.h`.
- User wants a custom ESP32 board variant kept inside their project repo.
- Build errors mentioning `NUM_DIGITAL_PINS` redefined, `uint8_t` unknown in `.c` files, or wrong `ARDUINO_VARIANT` at runtime.

## Platform

Use the pioarduino fork, not the stock Espressif platform:

```ini
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
framework = arduino
```

Pin to a release tag instead of `stable` for reproducible builds.

## Core commands

```sh
pio project init --board <board>     # bootstrap
pio run                              # build
pio run -v                           # verbose — shows -I include order
pio run -t upload                    # flash
pio run -t clean
pio device monitor -b 115200         # serial
pio lib install <name>               # or edit lib_deps in [env]
```

## Project conventions

- Prefer `src/main.cpp` over `.ino` for non-trivial projects.
- Standard `platformio.ini` keys: `framework = arduino`, `board`, `upload_speed`, `monitor_speed`.
- Debug build errors with `pio run -v` before guessing.

## Custom board variants in-repo

The pioarduino build script (`tools/pioarduino-build.py`) honors `build.variants_dir` to redirect variant lookup away from the framework package:

```python
variants_dir = join(FRAMEWORK_DIR, "variants")
if "build.variants_dir" in board_config:
    variants_dir = join("$PROJECT_DIR", board_config.get("build.variants_dir"))
```

Three pieces must agree — changing any without the others breaks the redirect:

1. **`platformio.ini`**

   ```ini
   board     = my_board
   board_dir = ${PROJECT_DIR}/boards
   board_build.variants_dir = variants
   board_build.variant      = my_board
   ```

2. **`boards/my_board.json`** — `build.variants_dir: "variants"` (relative to `$PROJECT_DIR`) and `build.variant: "my_board"`.

3. **`variants/my_board/pins_arduino.h`** — folder name must match `build.variant` **exactly, case-sensitive**.

Verify at runtime by printing `ARDUINO_VARIANT` from `setup()`. If it prints `esp32` instead of your variant name, the redirect failed.

### `pins_arduino.h` rules from real build failures

- **Do not define `NUM_DIGITAL_PINS` or `NUM_ANALOG_INPUTS`.** `Arduino.h` sets them *after* including this file from `SOC_GPIO_PIN_COUNT` / `SOC_ADC_CHANNEL_NUM()`. Redefining causes `-Wmacro-redefined` on every TU.
- **`#include <stdint.h>` explicitly.** Header is pulled into C TUs (e.g. `esp32-hal-gpio.c`) where `uint8_t` is not implicit.
- **`A*` / `T*` aliases**: use `static const uint8_t`, not `#define` — avoids ODR issues across TUs.
- **Board name**: lowercase letters, digits, underscores only. No hyphens (breaks `ARDUINO_<NAME>` macro).
- **ADC2 pins unusable while Wi-Fi active** — don't expose as defaults if Wi-Fi is on.

### Gotcha quick-table

| Symptom | Cause | Fix |
| --- | --- | --- |
| `"NUM_DIGITAL_PINS" redefined` warning on every TU | Defined in `pins_arduino.h` | Remove; `Arduino.h` sets it via `SOC_GPIO_PIN_COUNT` |
| `unknown type name 'uint8_t'` in `.c` files | Missing `<stdint.h>` (C TU pulls header) | Add `#include <stdint.h>` |
| `ARDUINO_VARIANT` prints `esp32` at runtime | `variants_dir` wrong / absolute | Path must be relative to `$PROJECT_DIR` |
| Build uses upstream pins instead of yours | `board_build.variant` ≠ folder name | Match exactly, case-sensitive |
| Variant override seems ignored | `board_dir` not set, JSON not loaded | Set `board_dir = ${PROJECT_DIR}/boards` |

See `references/custom-variant-template.md` for a full minimal template (files + content).

## Reference files

- `references/custom-variant-template.md` — full file-by-file template for a project-local ESP32 board variant.
