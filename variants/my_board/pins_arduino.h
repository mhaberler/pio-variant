#pragma once

#include <stdint.h>   /* uint8_t visible in both C and C++ translation units */

/* Do NOT define NUM_DIGITAL_PINS or NUM_ANALOG_INPUTS here.
   Arduino.h defines them after including this file, using SOC_GPIO_PIN_COUNT
   and SOC_ADC_CHANNEL_NUM() — let it do so. Redefining them here causes
   redefinition warnings across every compiled TU. */

// -- Built-in LED ------------------------------------------------------
#define LED_BUILTIN             2
#define BUILTIN_LED             LED_BUILTIN

// -- SPI ---------------------------------------------------------------
#define PIN_SPI_SS              5
#define PIN_SPI_MOSI            23
#define PIN_SPI_MISO            19
#define PIN_SPI_SCK             18

// -- I2C ---------------------------------------------------------------
#define PIN_WIRE_SDA            21
#define PIN_WIRE_SCL            22

// -- UART0 -------------------------------------------------------------
#define PIN_SERIAL_RX           3
#define PIN_SERIAL_TX           1

// -- DAC ---------------------------------------------------------------
#define DAC1                    25
#define DAC2                    26

// -- ADC ---------------------------------------------------------------
static const uint8_t A0  = 36;   // ADC1_CH0
static const uint8_t A1  = 37;   // ADC1_CH1
static const uint8_t A2  = 38;   // ADC1_CH2
static const uint8_t A3  = 39;   // ADC1_CH3
static const uint8_t A4  = 32;   // ADC1_CH4
static const uint8_t A5  = 33;   // ADC1_CH5
static const uint8_t A6  = 34;   // ADC1_CH6  (input-only)
static const uint8_t A7  = 35;   // ADC1_CH7  (input-only)
// ADC2 pins -- unavailable when Wi-Fi is active
static const uint8_t A10 = 4;    // ADC2_CH0
static const uint8_t A11 = 0;    // ADC2_CH1  (boot strapping pin)
static const uint8_t A12 = 2;    // ADC2_CH2
static const uint8_t A13 = 15;   // ADC2_CH3
static const uint8_t A14 = 13;   // ADC2_CH4
static const uint8_t A15 = 12;   // ADC2_CH5  (boot strapping pin)
static const uint8_t A16 = 14;   // ADC2_CH6
static const uint8_t A17 = 27;   // ADC2_CH7
static const uint8_t A18 = 25;   // ADC2_CH8  (also DAC1)
static const uint8_t A19 = 26;   // ADC2_CH9  (also DAC2)

// -- Touch -------------------------------------------------------------
static const uint8_t T0 = 4;
static const uint8_t T1 = 0;
static const uint8_t T2 = 2;
static const uint8_t T3 = 15;
static const uint8_t T4 = 13;
static const uint8_t T5 = 12;
static const uint8_t T6 = 14;
static const uint8_t T7 = 27;
static const uint8_t T8 = 33;
static const uint8_t T9 = 32;