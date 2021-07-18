#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>

//Fill these fields in
#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"
#define INFLUXDB_URL "influxdb-url"
#define INFLUXDB_DB_NAME "database"
#define INFLUXDB_USER "username"
#define INFLUXDB_PASSWORD "password"

#define R1  100
#define R2  200

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_DB_NAME);


// Data point
Point sensor("wifi_status");

void setup() {
  Serial.begin(115200);

  // Connect WiFi
  Serial.println("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // Set InfluxDB 1 authentication params
  client.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_USER, INFLUXDB_PASSWORD);

  // Add constant tags - only once
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop() {
  
  //Poll AC Voltage Data
  int voltageBuffer[100];
  for(int i= 0; i < 100; i ++) {
    voltageBuffer[i] = (analogRead(32) / R2) * R1;
    delay(1);
  }

  //Find the max voltage point
  int peakVoltage = 0;
  for(int i = 0; i < 100; i ++) {
    if(voltageBuffer[i] > peakVoltage) {
      peakVoltage = voltageBuffer[i];
    }
  }
  //peakVoltage = peakVoltage / sqrt(2); //Uncomment this if you want RMS rather than raw

  
  //Poll Current Data
  int currentBuffer[100];
  for(int i = 0; i < 100; i ++) {
    currentBuffer[i]= analogRead(34);
    delay(1);
  }
  int current = 0; 
  for(int i = 0; i < 100; i ++) {
    current += currentBuffer[i];
  }
  current = current / 100;
 
  

  
  // Store measured value into point
  sensor.clearFields();
  sensor.addField("Voltage", peakVoltage);
  sensor.addField("Current", current);

  
  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensor));
  // If no Wifi signal, try to reconnect it
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }
  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  //Wait 10s
  Serial.println("Wait 10s");
  delay(10000);
}
