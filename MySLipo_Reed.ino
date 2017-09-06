/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

//#define MY_DEBUG

#define MY_OTA_FIRMWARE_FEATURE
#define MY_RADIO_NRF24
#define MY_RF24_PA_LEVEL RF24_PA_LOW // RF24_PA_MAX

#include <MySensors.h>

const int BATTERY_SENSE_PIN = A7;
const int REED1_PIN = 2;
const int REED2_PIN = 3;

#define REED1_CHILD_ID 1
#define REED2_CHILD_ID 2

MyMessage msg(REED1_CHILD_ID, V_TRIPPED);
MyMessage msg2(REED2_CHILD_ID, V_TRIPPED);

void presentation(){
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("MySLipo reed", "2.0");
  present(REED1_CHILD_ID, S_DOOR);
  present(REED2_CHILD_ID, S_DOOR);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup(){
  analogReference(INTERNAL);
  pinMode(REED1_PIN, INPUT);
  pinMode(REED2_PIN, INPUT);
}

void loop(){
  uint8_t value;
  static uint8_t sentValue = 2;
  static uint8_t sentValue2 = 2;
  
  // Short delay to allow buttons to properly settle
  sleep(5);

  value = digitalRead(REED1_PIN);

  if (value != sentValue) {
    // Value has changed from last transmission, send the updated value
    send(msg.set(value==HIGH));
    sentValue = value;
  }

  value = digitalRead(REED2_PIN);

  if (value != sentValue2) {
    // Value has changed from last transmission, send the updated value
    send(msg2.set(value==HIGH));
    sentValue2 = value;
  }
  
#ifdef MY_DEBUG
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.println(" C");
#endif
  
  // get the battery Voltage
  float batteryV = analogRead(BATTERY_SENSE_PIN) * 0.00433365917; //((R1+R2)/R2)*1.1/1023
  byte batteryP = mapfloat(batteryV, 2.5, 4.2, 0, 100); // 2.5V cutoff voltage for USB charger circut, 4.2V fully charged Li-po
  batteryP = constrain(batteryP, 0, 100); // prevet overflow

#ifdef MY_DEBUG
  Serial.print("Battery voltage: ");
  Serial.print(batteryV);
  Serial.println(" V");
  Serial.print("Battery percentage: ");
  Serial.print(batteryP);
  Serial.println(" %");
#endif

  sendBatteryLevel(batteryP);

  // Sleep until something happens with the sensor
  sleep(REED1_PIN-2, CHANGE, REED2_PIN-2, CHANGE, 0);
}
