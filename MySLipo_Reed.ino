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
const int LED_PIN = 7;

#define REED1_CHILD_ID 1
#define REED2_CHILD_ID 2

MyMessage msg(REED1_CHILD_ID, V_TRIPPED);
MyMessage msg2(REED2_CHILD_ID, V_TRIPPED);

void presentation(){
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("MySLipo reed", "2.0");
  present(REED1_CHILD_ID, S_DOOR);
  sleep(5);
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
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  // Short delay to allow buttons to properly settle
  sleep(5);

  value = digitalRead(REED1_PIN);

  if (value != sentValue) {
    // Value has changed from last transmission, send the updated value
    debug("Reed switch 1 changed\n");
    send(msg.set(value==HIGH));
    sentValue = value;
  }

  value = digitalRead(REED2_PIN);

  if (value != sentValue2) {
    // Value has changed from last transmission, send the updated value
    debug("Reed switch 2 changed\n");
    send(msg2.set(value==HIGH));
    sentValue2 = value;
  }
  
  // get the battery Voltage
  float batteryV = analogRead(BATTERY_SENSE_PIN) * 0.00433365917; //((R1+R2)/R2)*1.1/1023
  byte batteryP = mapfloat(batteryV, 2.5, 4.2, 0, 100); // 2.5V cutoff voltage for USB charger circut, 4.2V fully charged Li-po
  batteryP = constrain(batteryP, 0, 100); // prevet overflow

  char output_buffer[80];
  char *output_pointer;
  sprintf (output_buffer, "Battery voltage: %.2fV\nBattery percentage: %i%%\n", batteryV, batteryP);
  memcpy(output_pointer, output_buffer, 80);

  debug(output_pointer);

  sendBatteryLevel(batteryP);

  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, INPUT); // uses less power when pin set to input

  // Sleep until something happens with the sensor
  sleep(REED1_PIN-2, CHANGE, REED2_PIN-2, CHANGE, 0);
}
