#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>

#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <StringSplitter.h>

char ssid[] = "LabElektronikaCom";
char wifiPassword[] = "MasterSya121efHiday4T";

char username[] = "866df290-1f2b-11ec-9f5b-45181495093e";
char password[] = "6f3c61dd3aa4c5f202d24b1e141ca2b8c913fe4b";
char clientID[] = "978a49c0-1f2b-11ec-ad90-75ec5e25c7a4";


//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData,Count;
String Data1,Data2,Data3,Data4,Data5,Data6;
void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA RECEIVER ");
  display.display();

  Serial.println("LoRa Receiver Test");
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.println("LoRa Initializing OK!");
  display.display();  
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  
}

void loop() {

  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    Serial.print("Received packet ");

    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      //Serial.print("*");
      //Serial.print(LoRaData);
      //Serial.print("*");


StringSplitter *splitter = new StringSplitter(LoRaData, '#', 7);
int itemCount = splitter->getItemCount();
//Serial.println("Item count: " + String(itemCount));
Serial.println();
for(int i = 0; i < itemCount; i++){
String item = splitter->getItemAtIndex(i);
Serial.println("Item @ index " + String(i) + ": " + String(item));

if(i==0){
  Data1=String(item);
}else if(i==1){
   Data2=String(item);
 
}else if(i==2){
   Data3=String(item);
 
}else if(i==3){
    Data4=String(item);

}else if(i==4){
    Data5=String(item);

}else if(i==5){
    Data6=String(item);

}else if(i==6){
  Count=String(item);
}

}
      
    }

    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    Serial.print(" with RSSI ");    
    Serial.println(rssi);

   // Dsiplay information
   display.clearDisplay();
   display.setCursor(0,0);
   display.print("LORA RECEIVER");
   display.setCursor(0,20);
   display.print("Received packet:");
   display.setCursor(0,30);
   display.print(Count);
   display.setCursor(0,40);
   display.print("RSSI:");
   display.setCursor(30,40);
   display.print(rssi);
   display.display();  
     Cayenne.loop();
 
  }
}

CAYENNE_OUT_DEFAULT()
{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  

  Cayenne.virtualWrite(1, Data5.toFloat());
  Cayenne.virtualWrite(2, Data3.toFloat());
  Cayenne.virtualWrite(3, Data1.toFloat());
  Cayenne.virtualWrite(4, Data6.toFloat());
  Cayenne.virtualWrite(5, Data4.toFloat());
  Cayenne.virtualWrite(6, Data2.toFloat());


}
