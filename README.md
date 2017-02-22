# MySensors Li-po/USB powered wirelles node
PCB size as 18500 Li-po battery board with ATmega328p with integrated USB charger and battery protection IC. Supports ATSHA204A CryptoAuth chip and DS18B20 temperature IC.


Supported radios:
* NRF24L01 SMD
* NRF24L01+LNA+PA SMD
* RFM69W

Pins available:
* D3
* D4 (used for 1-Wire with R2 pull-up)
* D5 (1-Wire supply with R2 pull-up)
* SDA (I2C with R3 pull-up)
* SCL (I2C with R4 pull-up)
* A0 (optional R7 pull-down)
* A1

Pins used:
* D2 = IRQ respectively DIO0
* D9 = CE pin
* D10 = CSN respectively NSS
* SPI = radios
* A2 = ATSHA204A
* A8 = battery monitor

## PCB top view
![PCB top view](https://s3.amazonaws.com/maker.works.boards/svgs/a8b670172ba2305c976bdec733c1f8e19b4596c7f034bc854a0c1bcfbb4d4a11/top.svg.s.png?updated=1482859644)
## PCB bottom view
![PCB bottom view](https://s3.amazonaws.com/maker.works.boards/svgs/a8b670172ba2305c976bdec733c1f8e19b4596c7f034bc854a0c1bcfbb4d4a11/bottom.svg.s.png?updated=1482859644)


[Order now at PCBs.io](https://PCBs.io/share/4qbyd)

### ATmega328p fuses
- **HIGH:** DA
- **LOW:** E2
- **EXTENDED:** 06

**NOTE:**
HEX files already contain *Sensebender Micro* bootloader - fully ready for MySensors network and [OTA updates](https://www.mysensors.org/about/ota), can be flashed with [Arduino as ISP programmer](https://www.arduino.cc/en/Tutorial/ArduinoISP) or any other AVR-ISP

*In the PCBs.io repo, DS18B20 is fliped by 180°, when you soldering don't forget to flip it. I'm working on the newer version which will correct that*