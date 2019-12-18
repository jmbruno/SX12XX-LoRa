/*******************************************************************************************************
  LoRaTracker Programs for Arduino - Copyright of the author Stuart Robinson - 16/12/19

  http://www.LoRaTracker.uk

  This program is supplied as is, it is up to the user of the program to decide if the program is
  suitable for the intended purpose and free from errors.
*******************************************************************************************************/

/*******************************************************************************************************
  Program Operation - This program is a remote control receiver. When a packet is received an 8 bit byte
  (SwitchByte) is read and the three outputs (defined in Settings.h) are toggled according to the bits
  set in this byte. If the Switch1 byte has bit 0 cleared, then OUTPUT0 is toggled. If the Switch1 byte
  has bit 1 cleared, then OUTPUT1 is toggled. If the Switch1 byte has bit 2 cleared, then OUTPUT2 is toggled.

  To prevent false triggering at the receiver the packet contains also contains a 32 bit number called the
  TXIdentity which in this example is set to 1234554321. The receiver will only act on, change the state
  of the outputs, if the identity set in the receiver matches that of the transmitter. The chance of a
  false trigger is fairly remote.

  The pin definitions, LoRa frequency and LoRa modem settings are in the Settings.h file.

  Serial monitor baud rate is set at 9600.
*******************************************************************************************************/

#define programversion "V1.0"

#include <SPI.h>
#include "SX127XLT.h"
#include "Settings.h"

SX127XLT LT;

uint32_t RXpacketCount;
uint16_t errors;

uint8_t RXPacketL;               //length of received packet
int8_t  PacketRSSI;              //RSSI of received packet
int8_t  PacketSNR;               //signal to noise ratio of received packet

uint8_t SwitchByte = 0xFF;       //this is the transmitted switch values, bit 0 = Switch0 etc

void loop()
{

  RXPacketL = LT.receiveSXBuffer(0, 0, WAIT_RX);       //returns 0 if packet error of some sort, timeout set at 5000mS

  digitalWrite(LED1, HIGH);                            //something has happened

  PacketRSSI = LT.readPacketRSSI();                    //read the signal strength of the received packet
  PacketSNR = LT.readPacketSNR();                      //read the signal to noise ratio of the received packet

  if (RXPacketL == 0)
  {
    packet_is_Error();
  }
  else
  {
    packet_is_OK();
  }

  digitalWrite(LED1, LOW);
  Serial.println();
}


uint8_t packet_is_OK()
{
  //packet has been received, now read from the SX12xx Buffer using the same variable type and
  //order as the transmit side used.
  uint32_t TXIdentity;

  RXpacketCount++;
  Serial.print(RXpacketCount);
  Serial.print(F("  Packet Received"));

  LT.startReadSXBuffer(0);                //start buffer read at location 0
  TXIdentity = LT.readUint32();           //read in the identity of transmitter
  SwitchByte = LT.readUint8();            //read in the Switche values
  RXPacketL = LT.endReadSXBuffer();

  printpacketDetails();

  if (TXIdentity != RXIdentity)
  {
    Serial.print(F("  Transmitter "));
    Serial.print(TXIdentity);
    Serial.print(F(" not recognised"));
    led_Flash(5, 25);                      //short fast speed flash indicates transmitter not recognised
    return 0;
  }

  if (LT.readRXPacketL() != 5)
  {
    Serial.print(F("  Wrong Packet Length"));
    led_Flash(5, 25);                      //short fast speed flash indicates transmitter not recognised
    return 0;
  }


  Serial.print(F("SwitchByte "));
  Serial.print(SwitchByte, BIN);           //print switch values in binary, if a bit is 0, that switch is active
  readSwitches(SwitchByte);

  return RXPacketL;
}


void packet_is_Error()
{
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();

  if (IRQStatus & IRQ_RX_TIMEOUT)
  {
    Serial.print(F("RXTimeout"));
  }
  else
  {
    errors++;
    Serial.print(F("PacketError"));
    printpacketDetails();
    Serial.print(F("IRQreg,"));
    Serial.print(IRQStatus, HEX);
  }
}


void printpacketDetails()
{
  Serial.print("  RSSI,");
  Serial.print(PacketRSSI);
  Serial.print(F("dBm,SNR,"));
  Serial.print(PacketSNR);
  Serial.print(F("dB,Length,"));
  Serial.print(LT.readRXPacketL());
}


void led_Flash(uint16_t flashes, uint16_t delaymS)
{
  uint16_t index;

  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }
}


void readSwitches(uint8_t switches)
{
  //read the recreived switch byte and toggle outputs as required

  if (!bitRead(switches, 0))
  {
    //Switch Output state
    digitalWrite(OUTPUT0, !digitalRead(OUTPUT0));
  }

  if (!bitRead(switches, 1))
  {
    //Switch Output state
    digitalWrite(OUTPUT1, !digitalRead(OUTPUT1));
  }

  if (!bitRead(switches, 2))
  {
    //Switch Output state
    digitalWrite(OUTPUT2, !digitalRead(OUTPUT2));
  }

}


void setupOutputs()
{
  //configure the output pins, if a pin is set as -1, its not configured

  if (OUTPUT0  >= 0)
  {
    pinMode(OUTPUT0, OUTPUT);
  }

  if (OUTPUT1  >= 0)
  {
    pinMode(OUTPUT1, OUTPUT);
  }

  if (OUTPUT2  >= 0)
  {
    pinMode(OUTPUT1, OUTPUT);
  }
}


void setup()
{
  pinMode(LED1, OUTPUT);
  led_Flash(2, 125);

  setupOutputs();

  Serial.begin(9600);

  SPI.begin();

  if (LT.begin(NSS, NRESET, DIO0, DIO1, DIO2, LORA_DEVICE))
  {
    led_Flash(2, 125);
  }
  else
  {
    Serial.println(F("Device error"));
    while (1)
    {
      led_Flash(50, 50);                                            //long fast speed flash indicates device error
    }
  }

  LT.setupLoRa(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate, Optimisation);

  Serial.println(F("Receiver ready"));
  Serial.println();
}


