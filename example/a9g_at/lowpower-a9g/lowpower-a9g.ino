#include <stdio.h>
#include <string.h>
#include "LowPower.h"

#define DEBUG true

#define A9G_PWR_KEY 9
#define A9G_LOW_PWR 5
#define A9G_RST_KEY 6

#define SLEEP_BUTTON 4

long int runtime = 0;

void setup()
{
    pinMode(A9G_PWR_KEY, OUTPUT);
    pinMode(A9G_RST_KEY, OUTPUT);
    pinMode(A9G_LOW_PWR, OUTPUT);

    digitalWrite(A9G_RST_KEY, LOW);
    digitalWrite(A9G_LOW_PWR, LOW);
    digitalWrite(A9G_PWR_KEY, LOW);

    SerialUSB.begin(115200);
    Serial1.begin(115200);

    delay(1100);
    digitalWrite(A9G_PWR_KEY, HIGH);
    SerialUSB.println("Sports Tracker Test Start!");

    delay(10000);

    sendData("AT+GPS=0", 1000, DEBUG);

    sendData("AT", 1000, DEBUG);

    {
        sendData("AT+sleep=1", 1000, DEBUG);

        delay(1000);

        digitalWrite(A9G_LOW_PWR, HIGH);
        SerialUSB.println("Sleep MODE");

        SerialUSB.println("CPU Sleep");
        delay(1000);

        while (1)
        {
            // Detach USB interface
            USBDevice.detach();
            // Enter standby mode
            LowPower.standby();
            // Attach USB interface
        }
    }
}

void loop()
{
    while (Serial1.available() > 0)
    {
        SerialUSB.write(Serial1.read());
        yield();
    }
    while (SerialUSB.available() > 0)
    {
        Serial1.write(SerialUSB.read());
        yield();
    }

    if (millis() - runtime > 3000)
    {
        if (digitalRead(SLEEP_BUTTON) == LOW)
        {
            digitalWrite(A9G_LOW_PWR, HIGH);
            SerialUSB.println("Sleep MODE");
        }
        else
        {
            digitalWrite(A9G_LOW_PWR, LOW);
            SerialUSB.println("Action MODE");
        }
        runtime = millis();
        SerialUSB.print("Runtime:");
        SerialUSB.println(runtime / 1000);
    }
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    Serial1.println(command);
    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (Serial1.available())
        {
            char c = Serial1.read();
            response += c;
        }
    }
    if (debug)
    {
        SerialUSB.print(response);
    }
    return response;
}
