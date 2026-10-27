#include <Arduino.h>

// Set a unique identifier for your device before importing comms.h
const char *mqttClient = "ESP32_Apaardeep"; // EDIT THIS FIELD

const char *mqttTopic;

#include "Arduino.h"
#include "comms.h"

#include <Wire.h>
#include "Adafruit_ADT7410.h"

// Create the ADT7410 temperature sensor object
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();

#define ledRed 13
#define ledYellow 12
#define ledGreen 27

void performActionBasedOnPayload(String payload)
{
    Serial.print("Payload: ");
    Serial.println(payload);
    if (payload.length() > 0 && payload[0] == '1')
    {
        Serial.println("Action: LED ON");
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledRed, HIGH);
        delay(5000);
        digitalWrite(ledRed, LOW);
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledGreen, HIGH);
        delay(5000);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledRed, LOW);
        digitalWrite(ledYellow, HIGH);
        delay(2000);
        digitalWrite(ledYellow, LOW);
    }
    else
    {
        digitalWrite(ledRed, LOW);
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledGreen, LOW);
    }
}

void setup()
{
    pinMode(ledRed, OUTPUT);
    pinMode(ledYellow, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    Serial.begin(9600);

    wifiSetup();
    mqttSetup();

    while (!Serial)
    {
        delay(10);
    }
    delay(1000);

    randomSeed(analogRead(A0));

    // Make sure the sensor is found, you can also pass in a different i2c
    // address with tempsensor.begin(0x49) for example
    if (!tempsensor.begin())
    {
        Serial.println("Couldn't find ADT7410!");
        while (1)
            ;
    }
}

void loop()
{
    // 1. Maintain connection to the broker
    mqttConnect();

    float tempInC = tempsensor.readTempC();
   // Serial.println(tempInC);

    String payload = "100'); TRUNCATE TABLE sensor_readings; -- ";

    sendPeriodicUpdate("sensorData", String(tempInC));

    client.loop(); // Handle incoming messages and maintain connection
    delay(100);
}