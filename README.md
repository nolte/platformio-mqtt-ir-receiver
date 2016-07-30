Ir Receiver
----------------


 [![Build Status](https://travis-ci.org/nolte/platformio-mqtt-ir-receiver.svg?branch=master)](https://travis-ci.org/nolte/platformio-mqtt-ir-receiver)

 Receiving IR Commands, the received Command are produce to two MQTT Topics.

# MQTT Topics
 
## Main Topic

 **Topic:** ``/iot/ircommands``
 
 **Payload:**
 
```json
{"value":"a40","decode_type":"4","irReceiverId":"ir-receiver-livingroom"}
```


## Command Topic

 **Topic:** ``/iot/ircommands/{irReceiverId}/{decode_type}``  
 **Example:** ``/iot/ircommands/ir-receiver-livingroom/4``
 
 **Payload:**
 
```
 a40
```

# Build

 Using [platformio.org](http://platformio.org/) as build system.  
 
 **used libs:**  
* [IRremote (4)](http://platformio.org/lib/show/4/IRremote)
* [Json (64)](http://platformio.org/lib/show/64/Json)
* [PubSubClient (89)](http://platformio.org/lib/show/89/PubSubClient) 

## run a build

```bash
 platformio run
```


## Upload to Arduino

```bash
 platformio run -target upload
```

# Open Tasks

 * using [platformio build_flags](http://docs.platformio.org/en/latest/projectconf.html#build-flags)
 * Refectore MqttDeviceHealthCheck