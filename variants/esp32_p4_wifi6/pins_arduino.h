#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

// ESP32-P4 board with onboard ESP32-C6 (Wi-Fi 6) co-processor.
// Header pinout — only GPIOs broken out on the board edge are declared here.
// Do NOT define NUM_DIGITAL_PINS / NUM_ANALOG_INPUTS — Arduino.h sets them
// from SOC_GPIO_PIN_COUNT / SOC_ADC_CHANNEL_NUM().

// --- I2C: explicitly labeled on the silkscreen ---
#define PIN_WIRE_SDA  7   // SDA / GPIO7
#define PIN_WIRE_SCL  8   // SCL / GPIO8

// --- USB 2.0 OTG (P4 native HS PHY pads exposed on header) ---
#define PIN_USB_DM   24   // DM / GPIO24
#define PIN_USB_DP   25   // DP / GPIO25

// --- SDMMC on-chip LDO power channel ---
// Required by esp32-hal-spi.c setLDOPower() on P4 (guarded by SOC_SDMMC_IO_POWER_EXTERNAL).
// Channel 4 is the canonical P4 internal LDO for SDMMC IO power; matches all stock
// P4 variants (esp32p4, dfrobot_firebeetle2_esp32p4, esp32p4_4ds_mipi*).
#define BOARD_SDMMC_POWER_CHANNEL 4

// SPI / UART / LED_BUILTIN intentionally left undefined.
// The header exposes general-purpose GPIOs only; the user must pick pins
// for their SPI bus / serial / status LED and define them in their sketch.

// --- ADC / Touch aliases ---
// ESP32-P4 has SAR-ADC1 on GPIO16-23 and SAR-ADC2 on GPIO24-31.
// Of the header pins, the following land on ADC channels:
static const uint8_t A0 = 20;  // ADC1
static const uint8_t A1 = 21;  // ADC1
static const uint8_t A2 = 22;  // ADC1
static const uint8_t A3 = 23;  // ADC1
static const uint8_t A4 = 26;  // ADC2  (P4 has no Wi-Fi radio of its own,
static const uint8_t A5 = 27;  // ADC2   so ADC2 contention does not apply
static const uint8_t A6 = 28;  // ADC2   — Wi-Fi runs on the C6 over SDIO)
static const uint8_t A7 = 29;  // ADC2
static const uint8_t A8 = 30;  // ADC2
static const uint8_t A9 = 31;  // ADC2

// ESP32-P4 has no touch peripheral — no T* aliases.

// --- Pin count sanity ---
// Header total: 32 IO pins (20 GPIO numbers per column, minus GND/power on each).
// Full SoC pin count is set by SOC_GPIO_PIN_COUNT (55 for P4).

#endif  // Pins_Arduino_h
