/*
MIT License
Copyright (c) 2018 Pavel Slama
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include <MySensors.h>

const int battery_pin = A7;
const int reed1_pin = 2;
const int reed2_pin = 3;
const int led_pin = 7;

#define REED1_CHILD_ID 1
#define REED2_CHILD_ID 2

bool prev_value[2]  = {false, false};

MyMessage msg(REED1_CHILD_ID, V_TRIPPED);
MyMessage msg2(REED2_CHILD_ID, V_TRIPPED);

void presentation() {
    sendSketchInfo("MySLipoReed", SKETCH_VER);
    present(REED1_CHILD_ID, S_DOOR);
    // sleep(5);
    present(REED2_CHILD_ID, S_DOOR);
}

void setup() {
    analogReference(INTERNAL);
    pinMode(reed1_pin, INPUT);
    pinMode(reed2_pin, INPUT);
}

void loop() {
    bool value;

    pinMode(led_pin, OUTPUT);
    wait(1);
    digitalWrite(led_pin, HIGH);

    value = digitalRead(reed1_pin);

    if (value != prev_value[0]) {
#ifdef MY_DEBUG
        Serial.println("Reed switch 1 changed\n");
#endif

        send(msg.set(value == HIGH));
        prev_value[0] = value;
    }

    value = digitalRead(reed2_pin);

    if (value != prev_value[1]) {
#ifdef MY_DEBUG
        Serial.println("Reed switch 2 changed\n");
#endif

        send(msg2.set(value == HIGH));
        prev_value[1] = value;
    }

    // get the battery Voltage
    uint32_t battery_v = analogRead(battery_pin) * 1000000;
    battery_v /= 2363076;  // (0,1*1024/4,3333333)*100000; adc to voltage conversion
    uint8_t battery_p = map(battery_v, 300, 420, 0, 100);  // voltage to percentage
    battery_p = constrain(battery_p, 0, 100);  // prevet overflow

#ifdef MY_DEBUG
    Serial.print("Battery voltage: ");
    Serial.print(battery_v);
    Serial.println(" V(*100)");
    Serial.print("Battery percentage: ");
    Serial.print(battery_p);
    Serial.println(" %");
#endif

    sendBatteryLevel(battery_p);

    digitalWrite(led_pin, LOW);
    pinMode(led_pin, INPUT);

    // Sleep until something happens with the sensor
    sleep(digitalPinToInterrupt(reed1_pin), CHANGE, digitalPinToInterrupt(reed2_pin), CHANGE, 0);
}
