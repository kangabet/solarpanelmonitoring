#include <SPI.h>
#include <LoRa.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1015.h>


#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define PZEM_SERIAL Serial2

PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26


#define BAND 866E6

//packet counter
int counter = 0;


Adafruit_ADS1115 ads(0x48);
float Voltage1 = 0.0,Voltage2 = 0.0;

float vIN1 = 0.0,vIN2 = 0.0;
float iIN1 = 0.0,iIN2 = 0.0;

float R1 = 30000.0;
float R2 = 7500.0;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  ads.begin();
 
  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");

  delay(2000);
}

char voltAc[10]="0",ampereAc[10]="0";



int value = 0;
void loop() {
   
  Serial.print("Sending packet: ");
  Serial.println(counter);


    float voltage = pzem.voltage();
    float current = pzem.current();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("*");
    } else if (isnan(current)) {
        Serial.println("*");
    }  else {

        // Print the values to the Serial console
        Serial.print("Voltage AC: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current AC: ");      Serial.print(current);      Serial.println("A");

         dtostrf(voltage,7, 2, voltAc);
         dtostrf(current,7, 2, ampereAc);

    }

    Serial.println();

int16_t adc1,adc2,adc3,adc4;

adc1 = ads.readADC_SingleEnded(0);
adc2 = ads.readADC_SingleEnded(1);
adc3 = ads.readADC_SingleEnded(2);
adc4 = ads.readADC_SingleEnded(3);

Voltage1 = (adc1 * 0.1875)/1000;
Voltage2 = (adc2 * 0.1875)/1000;

iIN1=adc3;
iIN2=adc4;

vIN1 = Voltage1 / (R2/(R1+R2));
vIN2 = Voltage2 / (R2/(R1+R2));

if(vIN1<0) vIN1=0;
if(vIN2<0) vIN2=0;


  String DATA  = String(trimString(voltAc));
         DATA +="#";
         DATA += String(trimString(ampereAc));
         DATA +="#";
         DATA += String(String(vIN1));
         DATA +="#";
         DATA += String(String(iIN1));
         DATA +="#";
         DATA += String(String(vIN2));  
         DATA +="#";
         DATA += String(String(iIN2));  
         DATA +="#";
      
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  //LoRa.print("12.34#56.78#90.21#43.64#65.87#87.98#");
  LoRa.print(DATA);
  LoRa.print(counter);
  
  LoRa.endPacket();
  
  counter++;
  
  delay(10000);
}

char *trimString(char *str)
{
    char *end;

    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}
