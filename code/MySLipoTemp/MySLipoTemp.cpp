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
#include <DallasTemperature.h>
#include <OneWire.h>

const int battery_pin = A7;
const int one_wire_pin = 4;
const int one_wire_supply_pin = 5;
const int led_pin = 7;

const uint16_t temp_wait = (750 / (1 << (12 - TEMP_RESOLUTION))) + 15;

OneWire oneWire(one_wire_pin);
DallasTemperature sensors(&oneWire);
MyMessage msg(0, V_TEMP);

void presentation() {
    sendSketchInfo("MySLipoTemp", SKETCH_VER);
    present(0, S_TEMP);
}

void setup() {
    analogReference(INTERNAL);
    sensors.setWaitForConversion(false);
}

void loop() {
    // supply voltage to Dallas
    pinMode(one_wire_supply_pin, OUTPUT);
    pinMode(led_pin, OUTPUT);
    wait(1);
    digitalWrite(one_wire_supply_pin, HIGH);
    digitalWrite(led_pin, HIGH);

    wait(2);

    sensors.begin();
    sensors.setResolution(TEMP_RESOLUTION);

#ifdef MY_DEBUG
    Serial.print("1-Wire devices found: ");
    Serial.println(sensors.getDeviceCount());
#endif

    sensors.requestTemperatures();

    sleep(temp_wait);

    float temperature = sensors.getTempCByIndex(0);
    digitalWrite(one_wire_supply_pin, LOW);  // turn off supply for Dallas

#ifdef MY_DEBUG
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.println(" C");
#endif

    if (temperature != -127.00 && temperature < 85.00) {  // if valid reading, send
        send(msg.set(temperature, 2));
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

    pinMode(one_wire_supply_pin, INPUT);
    digitalWrite(led_pin, LOW);
    pinMode(led_pin, INPUT);

    sleep(SLEEP_TIME - 1 - 2 - temp_wait);  // 1 millis after PinMode, 2ms after dallas power-up, temp_wait conversion of dallas
}
