// --------------------------------------
// i2c_scanner
// from: https://playground.arduino.cc/Main/I2cScanner/

// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.


#include <Wire.h>
uint16_t counter;

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("I2C Scanner starting");
  Serial.println();
}


void loop()
{
  uint8_t error, address;
  int16_t nDevices;

  counter++;
  Serial.print(counter);
  Serial.println(" Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
  {
    Serial.println("No I2C devices found");
  }
  else
  {
    Serial.println();
    Serial.println("Done");
    Serial.println();
  }

  delay(5000);           // wait 5 seconds for next scan
}