// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9

#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>


U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 9, /* data=*/ 8, /* cs=*/ 11, /* dc=*/ 10, /* reset=*/ 12);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);



// Assign address manually. The addresses below will beed to be changed
// to valid device addresses on your bus. Device address can be retrieved
// by using either oneWire.search(deviceAddress) or individually via
// sensors.getAddress(deviceAddress, index)
//28 5E A7 B3 13 19 01 F6
//28 91 6B D2 13 19 01 51
//28 AA 9D 48 4A 14 1 F8

DeviceAddress bat1 = { 0x28, 0x5E, 0xA7, 0xB3, 0x13, 0x19, 0x1, 0xF6 };
DeviceAddress bat2 = { 0x28, 0x91, 0x6B, 0xD2, 0x13, 0x19, 0x1, 0x51 };
DeviceAddress outside = {0x28, 0xAA, 0x9D, 0x48, 0x4A, 0x14, 0x1, 0xF8 };

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Search for devices on the bus and assign based on an index. Ideally,
  // you would do this to initially discover addresses on the bus and then
  // use those addresses and manually assign them (see above) once you know
  // the devices on your bus (and assuming they don't change).
  //
  // method 1: by index
  //if (!sensors.getAddress(bat1, 0)) Serial.println("Unable to find address for Device 0");
  //if (!sensors.getAddress(bat2, 1)) Serial.println("Unable to find address for Device 1");

  // method 2: search()
  // search() looks for the next device. Returns 1 if a new address has been
  // returned. A zero might mean that the bus is shorted, there are no devices,
  // or you have already retrieved all of them. It might be a good idea to
  // check the CRC to make sure you didn't get garbage. The order is
  // deterministic. You will always get the same devices in the same order
  //
  // Must be called before search()
  //oneWire.reset_search();
  // assigns the first address found to insideThermometer
  //if (!oneWire.search(insideThermometer)) Serial.println("Unable to find address for insideThermometer");
  // assigns the seconds address found to outsideThermometer
  //if (!oneWire.search(outsideThermometer)) Serial.println("Unable to find address for outsideThermometer");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(bat1);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(bat2);
  Serial.println();
  
    Serial.print("Device 1 Address: ");
  printAddress(outside);
  Serial.println();

  // set the resolution to 9 bit per device
  sensors.setResolution(bat1, TEMPERATURE_PRECISION);
  sensors.setResolution(bat2, TEMPERATURE_PRECISION);
sensors.setResolution(outside, TEMPERATURE_PRECISION);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(bat1), DEC);
  Serial.println();

  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(bat2), DEC);
  Serial.println();
    Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(outside), DEC);
  Serial.println();

  u8g2.begin();  
  u8g2.setFont (u8g2_font_u8glib_4_hr);

}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);

}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

/*
   Main function, calls the temperatures in a loop.
*/
void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures();

  int temp1 = sensors.getTempC(bat1);
  int temp2 = sensors.getTempC(bat2);
  int temp3 = sensors.getTempC(outside);
  
  u8g2.firstPage();
  do {
    //u8g2.drawStr (20, 22, "Progress Bar"); // write the text at the X and Y coordinates
    u8g2.drawFrame (3,7,12,50); // create an empty rectangle (frame) with the values ​​(x, y, width, height)
    u8g2.drawFrame (3+40,7,12,50); // create an empty rectangle (frame) with the values ​​(x, y, width, height)
    u8g2.drawFrame (3+80,7,12,50); // create an empty rectangle (frame) with the values ​​(x, y, width, height)
    draw(temp1, temp2, temp3); // we are going to draw
    u8g2.setFont (u8g2_font_u8glib_4_hr);

    u8g2.drawUTF8(18, 20, "Bat1");  
    u8g2.drawUTF8(18+40, 20, "Bat2");  
    u8g2.drawUTF8(18+80, 20, "outside");  

    u8g2.setFont (u8g2_font_5x8_tf );

    String yeet = "°C";
    yeet=temp1 + yeet;
    //Serial.println(yeet);
    char buf[10];
    strcpy(buf, yeet.c_str());
    u8g2.drawUTF8(18, 30, buf);  

    yeet = "°C";
    yeet=temp2 + yeet;
    //Serial.println(yeet);
    //char buf[10];
    strcpy(buf, yeet.c_str());
    u8g2.drawUTF8(18+40, 30, buf);  

    yeet = "°C";
    yeet=temp3 + yeet;
    //Serial.println(yeet);
    //char buf[10];
    strcpy(buf, yeet.c_str());
    u8g2.drawUTF8(18+80, 30, buf);  
    
  } while ( u8g2.nextPage() );

  


  // print the device information
  Serial.print("Bat1: ");
  Serial.println(temp1);
  Serial.print("Bat2: ");
  Serial.println(temp2);
  Serial.print("outside: ");
  Serial.println(temp3);

  delay(300);
  
}void draw(int pos, int pos2, int pos3){
  int maxtemp = 30;
  int mintemp = 22;
  //temp from 50C-10C
  //44 pixels high
  pos = map(pos, mintemp, maxtemp, 42,10);
  int bottom1= 44-(pos-10);
  u8g2.drawBox(5,pos, 8,bottom1);
  
  pos2 = map(pos2,mintemp, maxtemp, 42,10);
  int bottom2= 44-(pos2-10);
  u8g2.drawBox(5+40,pos2, 8,bottom2);
  
  pos3 = map(pos3, mintemp, maxtemp, 42,10);
  int bottom3= 44-(pos3-10);
  u8g2.drawBox(5+80,pos3, 8,bottom3);
  
}
