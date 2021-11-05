#include <stdio.h>
#include <string.h>
#include <DHT.h>

//Description: input AT commands via serial monitor to learn how to use A9/A9G module
//version:v1.0
//Author:Charlin
//web: http://www.makerfabs.com
//

//Change the API key to yours
String Apikey = "2ZOQP7ZGJ9OVGU6X";

#define DEBUG true

#define LTE_RESET_PIN 6

#define LTE_PWRKEY_PIN 5

#define LTE_FLIGHT_PIN 7


#define Sensor_PIN 3  //D3-DHT11

DHT  dht(Sensor_PIN,DHT11);



void setup()
{
    SerialUSB.begin(115200);
    //while (!SerialUSB)
    {
      ; // wait for Arduino serial Monitor port to connect
    }
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
    digitalWrite(LTE_FLIGHT_PIN, LOW);//Normal Mode
    
    delay(5000);

    
    /*ModuleState = moduleStateCheck();
    if (ModuleState == false) //if it's off, turn on it.
    {
        digitalWrite(PWR_KEY, LOW);
        delay(3000);
        digitalWrite(PWR_KEY, HIGH);
        delay(10000);
        SerialUSB.println("Now turnning the SIM7600 on.");
    }*/
    

    //sendData("AT+CCID", 3000, DEBUG);
    //sendData("AT+CREG?", 3000, DEBUG);
    //sendData("AT+CGATT=1", 1000, DEBUG);
    //sendData("AT+CGACT=1,1", 1000, DEBUG);
    //sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"", 1000, DEBUG);

    //sendData("AT+CIPSTART=\"TCP\",\"www.mirocast.com\",80", 2000, DEBUG);
    SerialUSB.println("4G HTTP Test Begin!");

    dht.begin();
    delay(1000);
}

void loop()
{
    //--------Get temperature and humidity-------------
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    //-----------HTTP---------------------
    String http_str = "AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=" + Apikey + "&field1=" + (String)t + "&field2=" + (String)h + "\"\r\n";
    SerialUSB.println(http_str);

    sendData("AT+HTTPINIT\r\n", 2000, DEBUG);
    sendData(http_str, 2000, DEBUG);
    sendData("AT+HTTPACTION=0\r\n", 3000, DEBUG);
    sendData("AT+HTTPTERM\r\n", 3000, DEBUG);

    delay(5000);   
    
    /*
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
    */
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
  Serial1.println(command);
  
  long int time = millis();
  while ( (time + timeout) > millis())
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
