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
#include <DallasTemperature.h>
#include <OneWire.h>

const int BATTERY_SENSE_PIN = A7;
const int ONE_WIRE_PIN = 4;
const int ONE_WIRE_SUPPLY_PIN = 5;
const unsigned long SLEEP_TIME = 600000;  // sleep time between reads (seconds * 1000 milliseconds)

const byte temp_resolution = 12; // 12bit 
const unsigned int temp_wait = (750 / (1 << (12 - temp_resolution)))+30;

#define CHILD_ID 0

OneWire oneWire(ONE_WIRE_PIN); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);
MyMessage msg(CHILD_ID, V_TEMP);

void before(){
  // Startup up the OneWire library
  sensors.begin();
  sensors.setResolution(temp_resolution);
}

void setup(){
  analogReference(INTERNAL);
  sensors.setWaitForConversion(false);
}

void presentation(){
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("MySLipo", "2.0");
  present(CHILD_ID, S_TEMP);
}

void loop(){
  // supply voltage to Dallas
  pinMode(ONE_WIRE_SUPPLY_PIN, OUTPUT);
  wait(1);
  digitalWrite(ONE_WIRE_SUPPLY_PIN, HIGH);

  sensors.requestTemperatures();
  sleep(temp_wait);
  
  float temperature = sensors.getTempCByIndex(0);
  digitalWrite(ONE_WIRE_SUPPLY_PIN, LOW); // turn off supply for Dallas
  
#ifdef MY_DEBUG
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.println(" C");
#endif
  
  if(temperature != -127.00 && temperature < 85.00) { // if valid reading, send
    send(msg.set(temperature,2));
  }
  
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

  pinMode(ONE_WIRE_SUPPLY_PIN, INPUT);
  sleep(SLEEP_TIME - 1 - temp_wait); // 1 millis after PinMode, temp_wait conversion of dallas
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
