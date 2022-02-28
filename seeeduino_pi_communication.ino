/********************************************************************/
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include "SparkFunCCS811.h"
#include <SoftwareSerial.h>
/********************************************************************/
#define RxD 0
#define TxD 1
#define ONE_WIRE_BUS 2
#define CCS811_ADDR 0x5B
/********************************************************************/
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
CCS811 mySensor(CCS811_ADDR);
SoftwareSerial XBee(RxD,TxD);
/********************************************************************/ 
 char temp_string[10], co2_string[10];
 float temp_data, co2_data;
/********************************************************************/ 
void setup() {
 Serial.begin(9600);  
 sensors.begin();
 delay(2000);
/********************************************************************/
 Wire.begin(); //Inialize I2C Hardware
 if (mySensor.begin() == false)
 {
   Serial.println();
   Serial.println("CCS811 error. Please check wiring. Freezing...");
   while (1);
 }
 XBee.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
 /********************************************************************/
 delay(5000);
}

void loop() {
 delay(100);
 sensors.requestTemperatures();
 Serial.print(sensors.getTempCByIndex(0));
 Serial.print("\t");
/********************************************************************/
 if (mySensor.dataAvailable())
 {
    delay(100);
    mySensor.readAlgorithmResults();
    Serial.print(mySensor.getCO2());
 }
 else {Serial.println("CO2 data not ready yet");}
 delay(1000);
}
