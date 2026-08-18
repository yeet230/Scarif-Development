// Set a unique identifier for your device before importing comms.h
const char *mqttClient = "ESP32_Lachlan"; // EDIT THIS FIELD

const char *mqttTopic;

#include <Arduino.h>
#include <Wire.h>

#include "comms.h"
#include "Adafruit_ADT7410.h"

Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();

void sensor_setup()
{
    Serial.println("ADT7410 demo");

    // Make sure the sensor is found, you can also pass in a different i2c
    // address with tempsensor.begin(0x49) for example
    if (!tempsensor.begin())
    {
        Serial.println("Couldn't find ADT7410!");
        while (1);
    }

    // sensor takes 250 ms to get first readings
    delay(250);
}

void performActionBasedOnPayload(String payload)
{
    Serial.print("Payload received: ");
    Serial.println(payload);

    // Turn built-in LED ON if payload starts with '1', else OFF
    if (payload.length() > 0 && payload[0] == '1')
    {
        Serial.println("Action: LED ON");
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        Serial.println("Action: LED OFF");
        digitalWrite(LED_BUILTIN, LOW);
    }
}

float tempSensorUpdate()
{
    // Read and print out the temperature, then convert to *F
    float tempInC = tempsensor.readTempC();
   
    Serial.println(tempInC);

    //delay(1000);
    return tempInC;
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    delay(2000);

    wifiSetup();
    mqttSetup();
    sensor_setup();

    while (!Serial)
    {
        delay(10);
    }

    delay(1000);
}

void loop()
{
    // 1. Maintain connection to the broker
    mqttConnect();

    // 2. Transmit periodic telemetry (if required by design specification)
    unsigned long now = millis();
    if (now - lastUpdate > updateInterval)
    {
        lastUpdate = now;
        Serial.println("Update data now");
        // TODO: Insert customized sendDataToServer() calls here.
        int randomNum = random(0, 100);
        float temp = tempSensorUpdate();

        String dataToUpload = String(temp) + "°C";

        sendDataToServer("sensorData", dataToUpload);
    }

    // 3. Yield execution time for PubSubClient processing
    client.loop();
    delay(100);
}

// mqttClient is the device name eg ESP32_Lachlan
// MQTT Topics
// EventLog/mqttClient       - For Device events eg startup, error   | Uploads
// sensorData/mqttClient     - For Telementry Data                   | Uploads
// devicePayload/mqttClient  - For Giving Device Data To Act On      | Recives
