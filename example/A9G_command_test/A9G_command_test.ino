#include <stdio.h>
#include <string.h>

//Description: input AT commands via serial monitor to learn how to use A9/A9G module
//version:v1.0
//Author:Charlin
//web: http://www.makerfabs.com
//

#define DEBUG true
#define PWR_KEY 5
#define RST_KEY 6
#define DTR_PIN 9 // D9高
#define RI_PIN 8  // D8输入

bool ModuleState = false;
String from_usb = "";

void setup()
{
    pinMode(PWR_KEY, OUTPUT);
    pinMode(RST_KEY, OUTPUT);
    pinMode(DTR_PIN, OUTPUT);
    pinMode(RI_PIN, INPUT);

    digitalWrite(RST_KEY, LOW);
    digitalWrite(PWR_KEY, HIGH);
    digitalWrite(DTR_PIN, HIGH);

    Serial1.begin(115200);
    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(10000);

    SerialUSB.begin(115200);
    //while (!SerialUSB)
    {
        ; // wait for serial port to connect
    }

    ModuleState = moduleStateCheck();
    if (ModuleState == false) //if it's off, turn on it.
    {
        digitalWrite(PWR_KEY, LOW);
        delay(3000);
        digitalWrite(PWR_KEY, HIGH);
        delay(10000);
        SerialUSB.println("Now turnning the SIM7600 on.");
    }

    //sendData("AT+CCID", 3000, DEBUG);
    //sendData("AT+CREG?", 3000, DEBUG);
    //sendData("AT+CGATT=1", 1000, DEBUG);
    //sendData("AT+CGACT=1,1", 1000, DEBUG);
    //sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"", 1000, DEBUG);

    //sendData("AT+CIPSTART=\"TCP\",\"www.mirocast.com\",80", 2000, DEBUG);
    SerialUSB.println("Maduino A9/A9G Test Begin!");
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
                //SerialUSB.println(from_usb);
                sendData(from_usb, 0, DEBUG);
                from_usb = "";
            }
        }

        // Serial1.write(SerialUSB.read());
        // yield();
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
