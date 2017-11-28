  //This is the very basic of reading a K type termocouple
#include "max6675.h"
                  // The pins are connected on the arduino inputs
int thermoDO = 4; // Every input could be used
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
    Serial.begin(9600);
    Serial.println("MAX6675 test");
    delay(500);
}
void loop() {
   Serial.print("C = "); 
   Serial.println(thermocouple.readCelsius());
   delay(1000);
}
