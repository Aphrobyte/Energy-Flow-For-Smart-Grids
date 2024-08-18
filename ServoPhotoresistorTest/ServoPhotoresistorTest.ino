#include <Servo.h>

/////////////////////////////////////////////
//Variables Declaration
////////////////////////////////////////////

//Input variables
//==========================================================//
//Servo Motors
Servo servo_PhotoResistor;     //Photoresistor servo motor
Servo servo_PhotovoltaicCell;  //Photovoltaic cell servo motors

//Analog Inputs
int in_int_PhotoResistor_Measured   = A0;   // Analog input of photoresistor measurements reading from pin A0

//Boolean Inputs
bool in_b_Scan_Servo_PhotoResistor; //When button 3 is pressed the PhotoResistor Servo to perform 180 degrees solar scan  
bool in_b_Goto_Optimal_Servo_Pos;   //When button 4 is pressed the Photovoltaic Servo goes to optimal position found during solar scan  

//Internal Variables
//==========================================================//

int int_optimal_Servo_pos = 0;   //position of maximum (optimal) Photoresistor voltage output
int int_PhotoResistor_Measured;   //Analog input of photoresistor measurements
int int_Max_PhotoResistor_Measured = 0; //Maximum Voltage

const int b_Scan_Servo_PhotoResistor = 3; // b_Scan_Servo_PhotoResistor is hardwared connected to digital pin 3 and is configured as digital input Read 
bool b_Scan_Servo_PhotoResistor_Active;   // When TRUE Photoresistor Servo performs 180 deg solar scanning

const int b_Goto_Optimal_Servo_Pos  = 4; //b_Goto_Optimal_Servo_Pos is hardwared connected to digital pin 4 and is configured as digital input Read 
bool b_Goto_Optimal_Servo_Pos_Active;    // When TRUE Servo goes to Optimal Position found during solar scan 

const int int_State_Idle = 10; 
const int int_State_SolarScan = 20; 
const int int_State_GoOptPos = 30; 
int int_State_Sequencer = int_State_Idle;

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long interval = 5000UL;       


void setup()
{
  servo_PhotoResistor.attach(9);     //photoresistor servo motor is connected to digital pin 9
  servo_PhotovoltaicCell.attach(10); //photovoltaic cell servo motor is connected to digital pin 10
  Serial.begin(9600);
  pinMode(3, INPUT); //b_Scan_Servo_PhotoResistor is connected to digital pin 3 and is configured as digital input Read 
  pinMode(4, INPUT); //button2 is connected to digital pin 4
}

void Servo_LightScan() {
  int_Max_PhotoResistor_Measured = 0;
  int int_servo_pos;

  for (int_servo_pos = 0; int_servo_pos <= 180; int_servo_pos += 1) //motor performs 180 degrees movement
  {    
    servo_PhotoResistor.write(int_servo_pos);
    int_PhotoResistor_Measured = analogRead(in_int_PhotoResistor_Measured);           
    delay(30);   

    Serial.println("Current position: ");
    Serial.println(int_servo_pos);

    Serial.println("Current light: ");    
    Serial.println(int_PhotoResistor_Measured);  
   
   if (int_PhotoResistor_Measured > int_Max_PhotoResistor_Measured) {
      int_Max_PhotoResistor_Measured = int_PhotoResistor_Measured;
      int_optimal_Servo_pos = int_servo_pos; //assignment of maximum light measurement and optimal position values during servo motion
      }  
  }
}

void Go_To_Opt_Pos(){
  servo_PhotovoltaicCell.write(int_optimal_Servo_pos); //photovoltaic cell servo moves to optimal position found during solar scan
 
  Serial.println("Optimal Position: ");
  Serial.println(int_optimal_Servo_pos);
  Serial.println("Maximum light measurement: ");
  Serial.println(int_Max_PhotoResistor_Measured);
 }

 

void loop() {

  in_b_Scan_Servo_PhotoResistor = digitalRead(b_Scan_Servo_PhotoResistor);
  in_b_Goto_Optimal_Servo_Pos = digitalRead(b_Goto_Optimal_Servo_Pos);

    if (millis() - previousMillis > interval) 
  {
    // save the last time you blinked the LED
    previousMillis += interval;  

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }

  }


Serial.println(millis() - previousMillis);
Serial.println(ledState);

  switch (int_State_Sequencer) {

  case int_State_Idle:

        if ((in_b_Scan_Servo_PhotoResistor) && (int_State_Sequencer == int_State_Idle)) {
          int_State_Sequencer = int_State_SolarScan; 
          }
        if ((in_b_Goto_Optimal_Servo_Pos) && (int_State_Sequencer == int_State_Idle)) {
          int_State_Sequencer = int_State_GoOptPos;
        }
        break;

  case int_State_SolarScan:
        Servo_LightScan();
        int_State_Sequencer = int_State_Idle;
        break;

  case int_State_GoOptPos:
        Go_To_Opt_Pos();
        int_State_Sequencer = int_State_Idle;
        break;

  //default:
    // if nothing else matches, do the default
    // default is optional
  }
}



