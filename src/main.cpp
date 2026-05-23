#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    // ARDUINO_VARIANT is defined by the build system from boards/my_board.json "variant" field.
    // Should print "my_board" -- confirms the custom variant was picked up correctly.
    Serial.printf("Variant: %s\n", ARDUINO_VARIANT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
