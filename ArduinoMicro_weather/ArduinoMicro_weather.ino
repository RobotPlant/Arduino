/***************************************************************************************************************
 * IoT Weather Station using NodeMCU ESP-12 Develop Kit V1.0
 *  DHT connected to NodeMCU pin D3
 *  Soil Moister Sensor connected to A0
 *  Sensor Data on local OLED Display
 *  Sensor data sent to Thingspeak Channel: https://thingspeak.com/channels/135305 
 *  Sensor data sent to Blynk app
 *  Downloads, docs, tutorials: http://www.blynk.cc
 *  Blynk library is licensed under MIT license
 *  
 *                  Version Blynk&TS - Developed by MJRovai 13 October 2017
 ********************************************************************************************************************************/

/* DHT11*/
#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
float hum = 0;
float temp = 0;

/* TIMER */
#include <SimpleTimer.h>
SimpleTimer timer;

/* Soil Moister */
#define soilMoisterPin A0
#define soilMoisterVcc 4
float soilMoister = 0;

int mostra = 0;

struct Dados {
  float sHum;
  float sTemp;
  float sSoil;
};

void setup()
{
  pinMode(soilMoisterVcc, OUTPUT);
  pinMode(soilMoisterPin, INPUT);
  Serial.begin(9600);
  delay(10000);
  dht.begin();
  timer.setInterval(7000L, getDhtData);
  timer.setInterval(7000L, getSoilMoisterData);
 // timer.setInterval(5000L, sendUptime);
  digitalWrite (soilMoisterVcc, LOW);
}

void loop()
{

  Dados dados[1] = {hum, temp, soilMoister };
  
  timer.run(); // Initiates SimpleTimer

  if(mostra == 1){
for(int i = 0; i< 1; i++) {
    Serial.println(dados[i].sHum);
    Serial.println(dados[i].sTemp);
    Serial.println(dados[i].sSoil);
    dados[i] = {0, 0, 0};
    Serial.println("Dados limpos");
  }
}  
}

/***************************************************
 * Get DHT data
 **************************************************/
void getDhtData(void)
{
  
  float tempIni = temp;
  float humIni = hum;

  temp = dht.readTemperature();

  hum = dht.readHumidity();
  
  if(temp != tempIni || temp == 0) {
      
    Serial.print("Temperatura: ");
    //Serial.print(tempIni);
    Serial.println(temp);
    //Serial.write(temp);
    tempIni = temp;
  }
  if(hum != humIni || hum == 0) {
    
    Serial.print("Humidade do ar: ");
    //Serial.print(humIni);
    Serial.println(hum);
 // Serial.write(hum);  
    humIni = hum;
  }
  
  if (isnan(hum) || isnan(temp))   // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    temp = tempIni;
    hum = humIni;
    return;
  }
}

/***************************************************
 * Get Soil Moister Sensor data
 **************************************************/
void getSoilMoisterData(void)
{
  float soilMoisterInit = soilMoister;
  digitalWrite (soilMoisterVcc, HIGH);
  delay (500);
  int N = 3;
  
  for(int i = 0; i < N; i++) // read sensor "N" times and get the average
  {
    soilMoister += analogRead(soilMoisterPin);   
    delay(150);
  }
  digitalWrite (soilMoisterVcc, LOW);
  soilMoister = soilMoister/N; 
 // Serial.println(soilMoister);
 
 if((soilMoister != soilMoisterInit) || (soilMoister ==0)){
  Serial.print("Humidade solo: ");
  Serial.println(soilMoister);
//  Serial.write(soilMoister);
  soilMoister = map(soilMoister, 380, 0, 0, 100);
  soilMoisterInit = soilMoister;
 }

}

/***************************************************
 * Send DHT data to Blynk
 **************************************************/
/*void sendUptime()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(10, temp); //virtual pin V10
  Blynk.virtualWrite(11, hum); // virtual pin V11
  Blynk.virtualWrite(12, soilMoister); // virtual pin V12
}
*/

/***************************************************
 * Start OLED
 **************************************************/
/*void oledStart(void)
{
  Wire.begin();  
  oled.init();                      // Initialze SSD1306 OLED display
  clearOledDisplay();
  //oled.setFont(font5x7);            // Set font type (default 8x8)
  oled.clearDisplay();              // Clear screen
  oled.setTextXY(0,0);              
  oled.putString("MJRoBot Blynk WS");
  oled.setTextXY(1,0);              
  oled.putString(String(SW_VERSION));
}
*/
/***************************************************
 * Display data at Serial Monitora & OLED Display
 **************************************************/
/*void displayData(void)
{ 
  oled.setTextXY(3,0);              // Set cursor position, start of line 2
  oled.putString("Temp: " + String(temp) + " oC");
  oled.setTextXY(4,0);              // Set cursor position, start of line 2
  oled.putString("Hum:  " + String(hum) + " %");
  oled.setTextXY(5,0);              // Set cursor position, start of line 2
  oled.putString("Soil: " + String(soilMoister) + " %");
}
*/
/***************************************************
 * Clear OLED Display
 **************************************************/
/*void clearOledDisplay()
{
  oled.setFont(font8x8);
  oled.setTextXY(0,0); oled.putString("                ");
  oled.setTextXY(1,0); oled.putString("                ");
  oled.setTextXY(2,0); oled.putString("                ");
  oled.setTextXY(3,0); oled.putString("                ");
  oled.setTextXY(4,0); oled.putString("                ");
  oled.setTextXY(5,0); oled.putString("                ");
  oled.setTextXY(6,0); oled.putString("                ");
  oled.setTextXY(7,0); oled.putString("                ");
  oled.setTextXY(0,0); oled.putString("                ");              
}
*/
/***************************************************
 * Sending Data to Thinkspeak Channel
 **************************************************/
/*void sendDataTS(void)
{
   if (client.connect(TS_SERVER, 80)) 
   { 
     String postStr = TS_API_KEY;
     postStr += "&field1=";
     postStr += String(temp);
     postStr += "&field2=";
     postStr += String(hum);
     postStr += "&field3=";
     postStr += String(soilMoister);
     postStr += "\r\n\r\n";
   
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: " + TS_API_KEY + "\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
     delay(1000); 
   }
   client.stop();
}
*/
