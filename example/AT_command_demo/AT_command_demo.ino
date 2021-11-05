
// Description: input AT commands via serial monitor to learn how to use 4G module
// version:v1.0
// Author:Vincent
// web: http://www.makerfabs.com

// This version is modify for SIM7600
// If define MODE_1A
// When "1A" or "1A" is entered, 0x1A is sent to the module.
// When sending SNS information, you need to send 0x1A to end the input.

#include <stdio.h>
#include <string.h>

#define DEBUG true
#define MODE_1A

#define DTR_PIN 9
#define RI_PIN 8

#define LTE_PWRKEY_PIN 5
#define LTE_RESET_PIN 6
#define LTE_FLIGHT_PIN 7

String from_usb = "";

void setup()
{
    SerialUSB.begin(115200);
    //while (!SerialUSB)
    {
        ; // wait for Arduino serial Monitor port to connect
    }

    delay(100);

    Serial1.begin(115200);

    //Serial1.begin(UART_BAUD, SERIAL_8N1, MODEM_RXD, MODEM_TXD);

    pinMode(LTE_RESET_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);

    pinMode(LTE_PWRKEY_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LTE_PWRKEY_PIN, HIGH);
    delay(2000);
    digitalWrite(LTE_PWRKEY_PIN, LOW);

    pinMode(LTE_FLIGHT_PIN, OUTPUT);
    digitalWrite(LTE_FLIGHT_PIN, LOW); //Normal Mode
    // digitalWrite(LTE_FLIGHT_PIN, HIGH);//Flight Mode

    SerialUSB.println("Maduino Zero 4G Test Start!");

    sendData("AT+CGMM", 3000, DEBUG);
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
#ifdef MODE_1A
        int c = -1;
        c = SerialUSB.read();
        if (c != '\n' && c != '\r')
        {
            from_usb += (char)c;
        }
        else
        {
            if (!from_usb.equals(""))
            {
                sendData(from_usb, 0, DEBUG);
                from_usb = "";
            }
        }
#else
        Serial1.write(SerialUSB.read());
        yield();
#endif
    }
}

bool moduleStateCheck()
{
    int i = 0;
    bool moduleState = false;
    for (i = 0; i < 5; i++)
    {
        String msg = String("");
        msg = sendData("AT", 1000, DEBUG);
        if (msg.indexOf("OK") >= 0)
        {
            SerialUSB.println("SIM7600 Module had turned on.");
            moduleState = true;
            return moduleState;
        }
        delay(1000);
    }
    return moduleState;
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    if (command.equals("1A") || command.equals("1a"))
    {
        SerialUSB.println();
        SerialUSB.println("Get a 1A, input a 0x1A");

        //Serial1.write(0x1A);
        Serial1.write(26);
        Serial1.println();
        return "";
    }
    else
    {
        Serial1.println(command);
    }

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
