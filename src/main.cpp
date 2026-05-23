#include <Arduino.h>

void setup() {
    Serial.begin(115200);
#ifdef LED_BUILTIN
    pinMode(LED_BUILTIN, OUTPUT);
#endif
    // ARDUINO_VARIANT is defined by the build system from the board JSON's
    // "variant" field. Confirms the custom variant was picked up correctly.
    Serial.printf("Variant: %s\n", ARDUINO_VARIANT);
}

void loop() {
#ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
#else
    delay(1000);
#endif
}
