/********************************************************************/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "SparkFunCCS811.h"
#include <SPI.h>
#include <SD.h>

#define ONE_WIRE_BUS 2
#define CCS811_ADDR 0x5B

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
CCS811 mySensor(CCS811_ADDR);
File myFile;
/********************************************************************/
void setup() {
  Serial.begin(9600);
  sensors.begin();
  delay(2000);
  /********************************************************************/
  Wire.begin();
  if (mySensor.begin() == false)
  {
    Serial.println("CCS811 error. Please check wiring. Freezing...");
    while (1);
  }
  delay(5000);

  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

void loop() {
  myFile = SD.open("data.txt", FILE_WRITE);
  if (myFile) {
    sensors.requestTemperatures();
    myFile.print(sensors.getTempCByIndex(0));
    myFile.print(",");
    /********************************************************************/
    if (mySensor.dataAvailable())
    {
      delay(100);
      mySensor.readAlgorithmResults();
      myFile.print(mySensor.getCO2());
      myFile.print(",");
      myFile.print(mySensor.getTVOC());
      myFile.print(",");
      myFile.println(millis());
    }
    myFile.close();
    delay(30000);
  } else {
    Serial.println("error opening data.txt");
  }
}
