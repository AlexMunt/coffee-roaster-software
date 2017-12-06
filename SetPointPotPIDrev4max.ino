 // This is a Temperature PID(basic) control, 
// The setpoit is inserted from a Potenciometer
// The input is the Max6675 thermocuopleCelsius with a K type termocouple.
// The Output is the PWM that control a SSR (solid state rele) 

   //The PID basic
  #include <PID_v1.h>
  #define PIN_INPUT 0  //this is the potenciometer 1 / Th SET Temperature
  #define PIN_INPUT 1  //this is the potenciometer 2 (not used in this example)
  #define PIN_OUTPUT 3 //this is PWM output for SSR rele

  // The potenciometers 
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

    Input = thermocouple.readCelsius();  //initialize the variables Input
    Setpoint =analogRead(potPin1) ;                        // The set point is readed 
    myPID.SetMode(AUTOMATIC);            //turn the PID ON
    Input = thermocouple.readCelsius();
  myPID.Compute();                       // PID in action
  
  analogWrite(PIN_OUTPUT, Output);       // The result of the PID process

                                        // Printing on Monitor
  Serial.print ("Set Temp:");
  Serial.print (analogRead(potPin1));
  Serial.println ("C");
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
  lcd.print("SET="); // Prints Sensor Val: to LCD
  lcd.print(analogRead(potPin1)); // Prints value on Potpin1 to LCD
  lcd.print("C");

  lcd.setCursor(0,1); // Sets the cursor to col 1 and row 0
  lcd.print("Heat="); // Prints Sensor Val: to LCD
lcd.print(val); // Prints value on Potpin1 to LCD
lcd.print("%");
        lcd.print("T="); 
        lcd.print(thermocouple.readCelsius());
        lcd.println ("C");

 delay (1000);
}

