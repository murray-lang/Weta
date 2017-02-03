#include "Arduino.h"
#include <weta_platform.h>
extern "C" {
#include "../../src/weta.h"
}

Weta weta;
extern Hardware hardware;

#define LED_PIN 13

void setup() {
    //pinMode(LED_PIN, OUTPUT);
    //digitalWrite(LED_PIN, LOW);

    //Serial.begin(9600);
    hw_init();
    weta_stack_init();
    weta_init(&weta, &hardware, 0);
}

void loop() {
    /*
    Serial.println("Example arduino project in CLion");

    for (int i = 0; i < 2; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
    }

    delay(1000);
     */
    weta_loop_body(&weta);
}
