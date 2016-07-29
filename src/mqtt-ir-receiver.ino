/**
 * Receiving IR Commands and send the Command to a MQTT Topic
 */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include <IRremote.h>
#include <ArduinoJson.h>
#include "MqttDeviceHealthCheck.h"

#if !defined(ARRAY_SIZE)
    #define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif


// Network config
// MAC Adresse des Ethernet Shields
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEE };

//IPAddress server(192, 168, 178, 64);
IPAddress ip(192, 168, 178, 74);

// IP des MQTT Servers
char server[]            = "maxchen";

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient apiClient;
PubSubClient mqttClient(server, 1883, callback, apiClient);

int RECV_PIN = 8;

IRrecv irrecv(RECV_PIN);
decode_results results;

char DEVICE_ID[] = "ir-receiver-livingroom";
char MQTT_IRCOMMANDS_PATH[] = "/iot/ircommands";
// constants won't change :
const long interval = 30000;  
unsigned long previousMillis = 0;

void setup()
{
  Serial.begin(9600);

  Ethernet.begin(mac, ip);

  // Allow the hardware to sort itself out
  delay(2500);
  if (mqttClient.connect(DEVICE_ID))
  {
    Serial.println("Connected");
  } else {
    Serial.println("Not Connected");
  }
 

  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Start IR MQTT Hub");
}

void loop() {
  unsigned long currentMillis = millis();

  if (irrecv.decode(&results)) {
    //if (results.value != zero) {
    Serial.println("===============Start============================");
    Serial.println(results.value, HEX);
    Serial.println(results.decode_type, HEX);
    Serial.println("===========================================");
    String hexCommand =  String(results.value, HEX);
    String hexType =  String(results.decode_type, HEX);
    // build the JSON Publishing message

    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["value"] = hexCommand;
    root["decode_type"] = hexType;
    root["irReceiverId"] = DEVICE_ID;
    String jsonString = "";
    root.printTo(jsonString);

    //      json.toCharArray(message_buff, json.length() + 1);
    //publish the info to mqtt

    Serial.println("===========================================");
    Serial.println(String(MQTT_IRCOMMANDS_PATH));
    Serial.println(jsonString);


    Serial.println("===========================================");
    String publishTarget = String(MQTT_IRCOMMANDS_PATH) + "/" + DEVICE_ID + "/" + results.decode_type;
    Serial.println(publishTarget);
    Serial.println(hexCommand);


    if (mqttClient.connected())
    {
      Serial.println("connected publishing IR command");
      String hexCommandJson = String(jsonString);
      int commandLenJson = hexCommandJson.length()  + 1;
      char message_buffCommandJson[commandLenJson];
      hexCommandJson.toCharArray(message_buffCommandJson, commandLenJson);
      boolean response = mqttClient.publish(MQTT_IRCOMMANDS_PATH, message_buffCommandJson);
      Serial.println(response);

      String hexCommand = String(results.value, HEX);
      int commandLen = hexCommand.length()  + 1;
      char message_buffCommand[commandLen];
      hexCommand.toCharArray(message_buffCommand, commandLen);

      String publishTargetFull = String(publishTarget);
      int singleTargetLen = publishTargetFull.length() + 1;
      char singleTarget_buffer[singleTargetLen];
      publishTargetFull.toCharArray(singleTarget_buffer, singleTargetLen);
      boolean response2 = mqttClient.publish(singleTarget_buffer, message_buffCommand);
      Serial.println(response2);
    } else {
      Serial.println("not conntected");
      if (mqttClient.connect(DEVICE_ID))
      {
        Serial.println("Connected");
      } else {
        Serial.println("Not Connected");
      }
    }
    //}

    Serial.println("===============ENDE============================");
    irrecv.resume(); // Receive the next value
  }
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (mqttClient.connected())
    {
    doHealthCheckMessage(String(DEVICE_ID),ip , mqttClient);
    }
   }

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("receiving");
}



