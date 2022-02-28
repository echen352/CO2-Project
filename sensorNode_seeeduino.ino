/********************************************************************/
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include "SparkFunCCS811.h" //Click here to get the library: http://librarymanager/All#SparkFun_CCS811
//#include <XBee.h>
#include <SoftwareSerial.h>
/********************************************************************/
#define RxD 0
#define TxD 1
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2
/********************************************************************/
#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 
CCS811 mySensor(CCS811_ADDR);
/********************************************************************/ 
//SoftwareSerial nss(3,4);
SoftwareSerial XBee(RxD,TxD);
/********************************************************************/ 
//XBee xbee = XBee();
 char temp_string[10], co2_string[10];
 float temp_data, co2_data;
/********************************************************************/ 
void setup() {
 Serial.begin(9600); 
 Serial.print("Initializing DS18B20 Temperature Sensor"); 
 // Start up the library 
 sensors.begin();
 Serial.println(" [Done]");
 delay(2000);
/********************************************************************/
 Serial.print("Initializing CCS811 CO2 Sensor");
 Wire.begin(); //Inialize I2C Hardware
 if (mySensor.begin() == false)
 {
   Serial.println();
   Serial.println("CCS811 error. Please check wiring. Freezing...");
   while (1);
 } else Serial.println(" [Done]");
 Serial.println("Initializing XBee");
 XBee.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
 Serial.println();
 /********************************************************************/
// xbee.setSerial(Serial);
// nss.begin(9600);
 delay(5000);
}

void loop() {
 Serial.println("DS18B20: Requesting temperatures...");
 delay(100);
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.print("[DONE]"); 
/********************************************************************/
 Serial.print(" Temperature is: ");
temp_data = sensors.getTempCByIndex(0);
 dtostrf(temp_data,3,3,temp_string);
 Serial.println(temp_data); // Why "byIndex"?  
 // You can have more than one DS18B20 on the same bus.  
 // 0 refers to the first IC on the wire 
 Serial.println();
/********************************************************************/
 //Serial.println("Attempting to send TEMPERATURE data via XBee...");
 //XBee.write("Temperature Data Received: ");
 //XBee.write(temp_string);
 //XBee.write("\n");
 //if (XBee.available()) {Serial.write(XBee.read());}
 //else {Serial.println("No Incoming Data Detected\n");}
/********************************************************************/
 Serial.println("CCS811: Requesting CO2 data..."); 
 //Check to see if data is ready with .dataAvailable()
 if (mySensor.dataAvailable())
 {
    //If so, have the sensor read and calculate the results.
    //Else get them later
    delay(100);
    mySensor.readAlgorithmResults();
/********************************************************************/
    Serial.print("[DONE] ");
    Serial.print("CO2[");
    //Returns calculated CO2 reading
  co2_data = mySensor.getCO2();
    dtostrf(co2_data,3,3,co2_string);
    //Serial.plot(co2_data);
    Serial.print(co2_data);
    Serial.print("] tVOC[");
    //Returns calculated TVOC reading
    Serial.print(mySensor.getTVOC());
    Serial.print("] millis[");
    //Display the time since program start
    Serial.print(millis());
    Serial.print("]");
    Serial.println();

    //Serial.println("Attempting to send CO2 data via XBee...");
    //XBee.write("CO2 Data Received: ");
    //XBee.write(co2_string);
    //XBee.write("\n");
 }
 else {Serial.println("CO2 data not ready yet");}
 Serial.println();
 delay(1000);
}
