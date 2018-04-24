#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include <SoftwareSerial.h>    

#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
SoftwareSerial DebugSerial(0, 1); // RX, TX

int pulsePin = 0;
int blinkPin = 13;
volatile int BPM;
volatile int Signal;
volatile int IBI = 600;
volatile boolean Pulse = false;
volatile boolean QS = false;
float Celcius=0;
char menuOption = 0;
long time0;
const int timeout = 10000;


void setup(void)
{
  interruptSetup();
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Start");
  menuOption = menu();
}

void loop(void)
{        
if(menuOption == '1'){
                   
              sensors.requestTemperatures(); 
              Celcius=sensors.getTempCByIndex(0);
              if (Celcius >= 38){
                Serial.print("The temperature is high you have to call 937: ");
                Serial.print(Celcius);
                Serial.print(" C  ");
                }
                if (Celcius >= 37.5) {
                  Serial.print("The temperature is high (Take Shower): ");
                  Serial.print(Celcius);
                  Serial.print(" C  ");
                  }
                if (Celcius <= 37) {
                  Serial.print("The temperature is Normal: ");
                  Serial.print(Celcius);
                  Serial.print(" C  ");
                  }
}
else if (menuOption == '2'){
  if (QS == true){
          Serial.print("Heart rate: ");            
          Serial.println(BPM);  
      QS= false;    
}
 if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
  delay(1000);
}
}

//////////////////////////
char menu()
{
    Serial.println(F("\nWhat you want to measure?"));
    Serial.println(F("(1) Measure body temperature."));
    Serial.println(F("(2) Measure heart rate"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {
            if(c == '1') 
          Serial.println(F("Now Measure body temperature."));
        else if(c == '2') 
          Serial.println(F("Now Measure heart rate"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
            }
        }
    }
