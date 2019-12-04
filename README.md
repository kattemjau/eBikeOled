# eBikeOled
Controll Pannel for eBike project

Uses SSD1306 128X64 Oled screen 2.42"
https://forum.arduino.cc/index.php?topic=442255.0

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 9, /* data=*/ 8, /* cs=*/ 11, /* dc=*/ 10, /* reset=*/ 12);

GND      GND
VCC       5v
SCL       9 (SCK)
SDA       8 (MOSI)
RES        12
DC         10
CS         11 (SS)

Driven by an ARDUINO MKR NB 1500
Uno had to little ram

And 3 DS18B20 sensors. To use new sensors, sensor adress needs to be changed.
Sensor pin is Pin2. Pullup to VCC is needed.
