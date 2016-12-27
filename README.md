# MySensors Li-po/USB powered wirelles node
Run by atmega328p and 18500 Li-po battery with integrated USB charger and battery protection IC. Supports ATSHA204A CryptoAuth chip and DS18B20 temperature IC.


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


