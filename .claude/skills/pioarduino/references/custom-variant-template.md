# Custom ESP32 board variant — minimal template

Drop-in files for a project-local board variant under pioarduino. Replace `my_board` everywhere with your board id (lowercase + digits + underscore, no hyphens).

## Layout

```
project/
├── platformio.ini
├── boards/
│   └── my_board.json
├── variants/
│   └── my_board/
│       └── pins_arduino.h
└── src/
    └── main.cpp
```

## `platformio.ini`

```ini
[env:my_board]
platform  = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
framework = arduino

board     = my_board
board_dir = ${PROJECT_DIR}/boards

board_build.variants_dir = variants
board_build.variant      = my_board

monitor_speed = 115200
upload_speed  = 921600
```

## `boards/my_board.json`

```json
{
  "build": {
    "arduino":     { "ldscript": "esp32_out.ld" },
    "core":        "esp32",
    "extra_flags": "-DARDUINO_MY_BOARD -DARDUINO_ARCH_ESP32",
    "f_cpu":       "240000000L",
    "f_flash":     "80000000L",
    "flash_mode":  "dio",
    "mcu":         "esp32",
    "variant":     "my_board",
    "variants_dir": "variants"
  },
  "connectivity": ["wifi", "bluetooth"],
  "debug": { "openocd_target": "esp32.cfg" },
  "frameworks": ["arduino"],
  "name": "My Custom ESP32 Board",
  "upload": {
    "flash_size":          "4MB",
    "maximum_ram_size":    327680,
    "maximum_size":        4194304,
    "require_upload_port": true,
    "speed":               921600
  },
  "url":    "https://github.com/yourname/my-esp32-board",
  "vendor": "YourName"
}
```

For other SOCs, set `mcu` (`esp32s3`, `esp32c3`, …) and adjust `maximum_ram_size` / `maximum_size` per module datasheet.

## `variants/my_board/pins_arduino.h` — skeleton

```c
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>     // required: header is pulled into C TUs

// Do NOT define NUM_DIGITAL_PINS or NUM_ANALOG_INPUTS.
// Arduino.h sets them after including this file via SOC_GPIO_PIN_COUNT / SOC_ADC_CHANNEL_NUM().

#define LED_BUILTIN  2

// SPI
#define PIN_SPI_SS    5
#define PIN_SPI_MOSI 23
#define PIN_SPI_MISO 19
#define PIN_SPI_SCK  18

// I2C
#define PIN_WIRE_SDA 21
#define PIN_WIRE_SCL 22

// UART
#define PIN_SERIAL_RX 3
#define PIN_SERIAL_TX 1

// ADC aliases — use static const uint8_t to avoid ODR issues.
// Drop any pin not present on your SOC. ADC2 pins fail while Wi-Fi is active.
static const uint8_t A0 = 36;
static const uint8_t A3 = 39;
static const uint8_t A4 = 32;
static const uint8_t A5 = 33;
static const uint8_t A6 = 34;
static const uint8_t A7 = 35;

// Touch aliases
static const uint8_t T0 = 4;
static const uint8_t T1 = 0;
static const uint8_t T2 = 2;
static const uint8_t T3 = 15;

#endif
```

## `src/main.cpp` — sanity check

```cpp
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.printf("Variant: %s\n", ARDUINO_VARIANT);  // must print "my_board"
}

void loop() {}
```

## Verification

1. `pio run -v` — confirm `variants/my_board/` appears in `-I` flags **before** the framework `variants/` path.
2. Flash, open monitor, check `ARDUINO_VARIANT` prints your board id, not `esp32`.

## Renaming checklist

When renaming `my_board` → `your_board`:

- `variants/my_board/` → `variants/your_board/`
- `boards/my_board.json` → `boards/your_board.json`
- Inside JSON: `build.variant`, `build.variants_dir`-relative target
- `platformio.ini`: `[env:...]`, `board`, `board_build.variant`
- `extra_flags`: `-DARDUINO_MY_BOARD` → `-DARDUINO_YOUR_BOARD`

Identifier constraint: `[a-z0-9_]+`. No hyphens — they break the `ARDUINO_<NAME>` define.
