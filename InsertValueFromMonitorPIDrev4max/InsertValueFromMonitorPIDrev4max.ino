// This is a Temperature PID(basic) control, 
// The setpoit is inserted from serial monitor (from keypad)
// The input, iniocialy was the potenciometer, for testing.
// The input is the Max6675 thermocuopleCelsius with a K type termocouple.
// The Output is the PWM that control a SSR (solid state rele) 

   //The PID basic
  #include <PID_v1.h>
  #define PIN_INPUT 0  //this is a potenciometer
  #define PIN_OUTPUT 3 //this is PWM output for SSR rele

  // The potenciometer for testing
  int potPin1 = A0;
  int potPin2 = A1;
  
  // The  LiquidCrystal 
#include <LiquidCrystal.h>
  LiquidCrystal lcd(7,8,9,10,11,12);
  
  //The Max6675
#include <max6675.h>
       int thermoDO = 4;
       int thermoCS = 5;
       int thermoCLK = 6;
   MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


   //Defining PID 
double Setpoint, Input, Output;   //Define Variables we'll be connecting to
double Kp=10, Ki=5, Kd=1;          //The  initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

//
char rx_byte = 0;
String rx_str = "";
boolean not_number = false;
int TheSetTemp;
int x;


void setup() {
  Serial.begin(9600);
  // A basic description shown on the serail monitor 
  Serial.println("Temperature  control of the Roaster");
  //Wait for Max chip to stabiliza
  Serial.println("MAX6675 test");
  // Time to wait for MAX chip to stabilize
  delay(500); 
  
lcd.begin(16, 2);
lcd.clear();
}

void loop() {
   // Insert the Setpoint from the keypad
  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
    
    if ((rx_byte >= '0') && (rx_byte <= '9')) {
      rx_str += rx_byte;
    }
    else if (rx_byte == '\n') {   // end of string
      if (not_number) {
        Serial.println("Not a number");
      }
      else {
        TheSetTemp = rx_str.toInt();      // print the Set Temperature
        Serial.print(" SetTemp = ");  // Serial.print(rx_str);
        Serial.print(TheSetTemp);
        Serial.println("C");      //Serial.println("Set the temperature of the Roaster");
      }
      not_number = false;         // reset flag
      rx_str = "";                // clear the string for reuse
    }
    else {                    // non-number character received
      not_number = true;      // flag a non-number
    }
  } // end: if (Serial.available() > 0)
   x= TheSetTemp;

   
    Input = thermocouple.readCelsius();  //initialize the variables Input
    Setpoint =x ;                        // The set point is readed 
    myPID.SetMode(AUTOMATIC);            //turn the PID ON
    Input = thermocouple.readCelsius();
  myPID.Compute();                       // PID in action
  
  analogWrite(PIN_OUTPUT, Output);       // The result of the PID process

                                        // Printing on Monitor
  Serial.print ("Set Temp:");
  Serial.print (TheSetTemp);
  Serial.println ("C");
  Serial.print ("Potenciometer Temp:");
  Serial.print (Input);
  Serial.print ("C    ");
  Serial.print ("Power Heat:");
       int val = Output;                // Mapping to percentage of the Power Heat 
       val = map(val, 0, 255, 0, 100);
       analogWrite(9, val);
       Serial.print (val);
       Serial.println ("%");
       
        Serial.print("Roaster Temp = "); 
        Serial.print(thermocouple.readCelsius());
        Serial.println ("C");

                                           //Printing on LCD 2x16
  lcd.setCursor(0,0); // Sets the cursor to col 0 and row 0
  lcd.print("POT:"); // Prints Sensor Val: to LCD
  lcd.print(analogRead(potPin1)); // Prints value on Potpin1 to LCD
  lcd.print("C");
  lcd.print("/SET:");
  lcd.print(TheSetTemp);
  lcd.print("C");
  lcd.setCursor(0,1); // Sets the cursor to col 1 and row 0
  lcd.print("Heat:"); // Prints Sensor Val: to LCD
lcd.print(val); // Prints value on Potpin1 to LCD
lcd.print("%");
        lcd.print("R = "); 
        lcd.print(thermocouple.readCelsius());
        lcd.println ("C");

 delay (1000);
}

