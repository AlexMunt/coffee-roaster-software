/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <PID_v1.h>  // This is the library

#define PIN_INPUT 0  // The Input, a potenciometer for example, 
#define PIN_OUTPUT 3 // The PWM Output,a  LED for example 

double Setpoint, Input, Output; //Define Variables of the PID
double Kp=2, Ki=5, Kd=1;        //The tuning parameters of the PID  
                                // The PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  Input = analogRead(PIN_INPUT); //initialize the variables we're linked to
  Setpoint = 100;                // The setpoint value, the reference of the PID
  myPID.SetMode(AUTOMATIC);      // START ON, turn the PID on
}

void loop()
{
  Input = analogRead(PIN_INPUT);
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
}


