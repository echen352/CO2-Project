#include <OneWire.h>
#include <DallasTemperature.h>
#include <sSense-CCS811.h>
#include <SoftwareSerial.h>

#define RxD 0
#define TxD 1
#define ONE_WIRE_BUS 2
#define NODE_ID 1
#define DELAY_TIME NODE_ID

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);
CCS811 ssenseCCS811;

String temp_str, co2_str, msg = "", packet = "", packLength = "";
unsigned int strLength = packet.length();

void setup() {
  Serial.begin(9600); // initialize serial baudrate to 9600

  tempSensors.begin(); // initialize DS811B20 Temperature Sensor

  if (!ssenseCCS811.begin(uint8_t(I2C_CCS811_ADDRESS), uint8_t(CCS811_WAKE_PIN), driveMode_1sec))
  {
    DebugPort.println("Initialization failed.");
    while(1);
  }
}

void loop() {
  // wait based on NODE_ID ensure only one node will read at a time
  delay(1000 * DELAY_TIME);
  transmit();
}

/* function does not currently work
void receive() {
  while (Serial.available() <= 0) {
    msg = Serial.read();
    if (msg == ("Interrupt NODE: " + String(NODE_ID))) {
      msg = "";
      break;
    }
    delay(1000);
  }
}
*/

void transmit() {
  //if (co2Sensors.dataAvailable())
  if (ssenseCCS811.checkDataAndUpdate())
  {
    // Write NODE ID to packet
    packet += String(NODE_ID);

    packet += "\t";

    // Read temp data and write to packet
    tempSensors.requestTemperatures();
    temp_str = String(tempSensors.getTempCByIndex(0));
    packet += temp_str;

    packet += "\t";

    // Read CO2 data and write to packet
    co2_str = String(ssenseCCS811.getCO2());
    packet += co2_str;

    packet += "\t";

    // Compute checksum and write to packet
    packLength = String(packet.length());
    strLength = packet.length() + packLength.length();
    packet += strLength;

    // Send packet
    Serial.println(packet);
    packet = "";
  }
  else {
    Serial.println("CO2 data not ready yet");
  }
}
