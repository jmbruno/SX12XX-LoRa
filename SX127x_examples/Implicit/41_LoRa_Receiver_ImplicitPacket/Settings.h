/*******************************************************************************************************
  lora Programs for Arduino - Copyright of the author Stuart Robinson - 16/12/19

  This program is supplied as is, it is up to the user of the program to decide if the program is
  suitable for the intended purpose and free from errors.
*******************************************************************************************************/

//*******  Setup hardware pin definitions here ! ***************

//These are the pin definitions for one of my own boards, the Easy Mikrobus Pro Mini, 
//be sure to change the definitions to match your own setup. Some pins such as DIO1,
//DIO2, BUZZER may not be in used by this sketch so they do not need to be
//connected and should be included and be set to -1.

#define NSS 10                                  //select pin on LoRa device
#define NRESET 9                                //reset pin on LoRa device
#define LED1 8                                  //on board LED, high for on
#define DIO0 3                                  //DIO0 pin on LoRa device, used for RX and TX done 
#define DIO1 -1                                 //DIO1 pin on LoRa device, normally not used so set to -1 
#define DIO2 -1                                 //DIO2 pin on LoRa device, normally not used so set to -1
#define BUZZER 4                                //pin for buzzer, on when logic high

#define LORA_DEVICE DEVICE_SX1278               //we need to define the device we are using


//*******  Setup LoRa Parameters Here ! ***************

//LoRa Modem Parameters
const uint32_t Frequency = 434000000;           //frequency of transmissions in hertz
const uint32_t Offset = 0;                      //offset frequency for calibration purposes

const uint8_t Bandwidth = LORA_BW_125;          //LoRa bandwidth
const uint8_t SpreadingFactor = LORA_SF6;       //LoRa spreading factor
const uint8_t CodeRate = LORA_CR_4_5;           //LoRa coding rate
const uint8_t Optimisation = LDRO_AUTO;         //low data rate optimisation setting, normally set to auto
const uint8_t PacketLength = 19;                //packet length is fixed  

const int8_t TXpower = 2;                       //LoRa transmit power in dBm

const uint16_t packet_delay = 1000;             //mS delay between packets

#define RXBUFFER_SIZE 32                        //RX buffer size  



