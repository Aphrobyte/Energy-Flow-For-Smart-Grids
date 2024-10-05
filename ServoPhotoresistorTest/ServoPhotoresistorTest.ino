#include <Servo.h>

/////////////////////////////////////////////>^.^<
//Variables Declaration
////////////////////////////////////////////

//Input variables
//==========================================================//
//Servo Motors
Servo servo_PhotoResistor;     //Photoresistor servo motor
Servo servo_PhotovoltaicCell;  //Photovoltaic cell servo motors

//Analog Inputs
int in_int_PhotoResistor_Measured   = A0;   // Analog input of photoresistor measurements reading from pin A0
int in_int_Battery_A_Voltage        = A1;   // Analog input of Battery A
int in_int_Battery_B_Voltage        = A2;   // Analog input of Battery B

//Boolean Inputs
bool in_b_Scan_Servo_PhotoResistor; //When button 3 is pressed the PhotoResistor Servo to perform 180 degrees solar scan  
bool in_b_Goto_Optimal_Servo_Pos;   //When button 4 is pressed the Photovoltaic Servo goes to optimal position found during solar scan  
bool in_b_Auto_Mode;                 //When switch is set to TRUE (AutoMode) Sequence runs in autom i.e. SolarScanning, Photovoltaic going to optimal position 

//Internal Variables
//==========================================================//

int int_optimal_Servo_pos = 0;   //position of maximum (optimal) Photoresistor voltage output
int int_PhotoResistor_Measured;   //Analog input of photoresistor measurements
int int_Max_PhotoResistor_Measured = 0; //Maximum Voltage

int int_Battery_A_Voltage;
int int_Battery_B_Voltage;

const int b_Scan_Servo_PhotoResistor = 3; // b_Scan_Servo_PhotoResistor is hardware connected to Arduino digital pin 3 and is configured as digital input Read 
bool b_Scan_Servo_PhotoResistor_Active;   // When TRUE Photoresistor Servo performs 180 deg solar scanning

const int b_Goto_Optimal_Servo_Pos  = 4; //b_Goto_Optimal_Servo_Pos is hardware connected to Arduino digital pin 4 and is configured as digital input Read 
bool b_Goto_Optimal_Servo_Pos_Active;    // When TRUE Servo goes to Optimal Position found during solar scan 

const int b_Manual_Mode = 2;                //b_Auto_mode is hardware connected to Arduino digital pin 2 and is configured as digital input Read
const int b_RelayPin_A  = 5;      //Relay A Output
const int b_RelayPin_B  = 6;      //Relay B Output


const int int_State_Idle = 10; 
const int int_State_SolarScan = 20; 
const int int_State_GoOptPos = 30; 
int int_State_Sequencer = int_State_Idle;

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long interval = 30000UL;     //Every minute  

bool Timer_Trigger_Auto_Sequence; // When mode is in AUTO and timer period is elapsed the solar scanning sequence is triggered 

bool b_State_SolarScan_Auto; //When TRUE and Mode is AUTO Servo_LightScan() is being executed 
bool b_State_GoOptPos_Auto; //When TRUE and Mode is AUTO Go_To_Opt_Pos() is being executed
bool b_State_Idle_Auto;     //When TRUE and Mode is AUTO State_Sequencer goes to State_Idle state


void setup()
{
  servo_PhotoResistor.attach(9);     //photoresistor servo motor is connected to digital pin 9
  servo_PhotovoltaicCell.attach(10); //photovoltaic cell servo motor is connected to digital pin 10
  Serial.begin(9600);

  pinMode(2, INPUT); //b_Scan_Servo_PhotoResistor is connected to digital pin 3 and is configured as digital input Read 
  pinMode(3, INPUT); //b_Scan_Servo_PhotoResistor is connected to digital pin 3 and is configured as digital input Read 
  pinMode(4, INPUT); //button2 is connected to digital pin 4
  pinMode(5, OUTPUT); //Digital Output to Relay A
  pinMode(6, OUTPUT); //Digital Output to Relay B
}

void Servo_LightScan() {
  int_Max_PhotoResistor_Measured = 0;
  int int_servo_pos;

  for (int_servo_pos = 30; int_servo_pos <= 150; int_servo_pos += 1) //motor performs 180 degrees movement
  {    
    servo_PhotoResistor.write(int_servo_pos);
    int_PhotoResistor_Measured = analogRead(in_int_PhotoResistor_Measured);           
    delay(100);   

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
 // int_optimal_Servo_pos = 90;
  servo_PhotovoltaicCell.write(int_optimal_Servo_pos); //photovoltaic cell servo moves to optimal position found during solar scan
 
  Serial.println("Optimal Position: ");
  Serial.println(int_optimal_Servo_pos);
  Serial.println("Maximum light measurement: ");
  Serial.println(int_Max_PhotoResistor_Measured);
 }

 void Relay() {
  int_Battery_A_Voltage = analogRead(in_int_Battery_A_Voltage);
  int_Battery_B_Voltage = analogRead(in_int_Battery_B_Voltage);

  if (int_Battery_A_Voltage > int_Battery_B_Voltage +0.1)
    {  
        digitalWrite(b_RelayPin_A, LOW);
        digitalWrite(b_RelayPin_B, HIGH);
    }
  
  else if (int_Battery_B_Voltage > int_Battery_A_Voltage +0.1)
    {
        digitalWrite(b_RelayPin_A, HIGH);
        digitalWrite(b_RelayPin_B, LOW);
    }
 }

void loop() {

  Relay();


  in_b_Scan_Servo_PhotoResistor = digitalRead(b_Scan_Servo_PhotoResistor);
  in_b_Goto_Optimal_Servo_Pos = digitalRead(b_Goto_Optimal_Servo_Pos);
  in_b_Auto_Mode = !digitalRead(b_Manual_Mode);

  

  if (!in_b_Auto_Mode) {
    Timer_Trigger_Auto_Sequence = LOW;
  }
    if (millis() - previousMillis > interval) 
  {

     
    Timer_Trigger_Auto_Sequence = in_b_Auto_Mode;
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




int_PhotoResistor_Measured = analogRead(in_int_PhotoResistor_Measured);     
Serial.println("Sensor Val: "  + String(int_PhotoResistor_Measured) + ", Max Sensor Val: " + String(int_Max_PhotoResistor_Measured) + ", Opt Pos: " + String(int_optimal_Servo_pos) + ", Auto Mode:"+ String(in_b_Auto_Mode) +  ", State: " + String(Timer_Trigger_Auto_Sequence) + ", Battery A Voltage:" + String(int_Battery_A_Voltage) +  ", Battery B Voltage:" + String(int_Battery_B_Voltage));


  switch (int_State_Sequencer) {

  case int_State_Idle:

        if (((in_b_Scan_Servo_PhotoResistor) && (int_State_Sequencer == int_State_Idle) && (!in_b_Auto_Mode) ) ||   ( in_b_Auto_Mode && Timer_Trigger_Auto_Sequence   ) ){
          int_State_Sequencer = int_State_SolarScan; 
          }
        if ((in_b_Goto_Optimal_Servo_Pos) && (int_State_Sequencer == int_State_Idle) && (!in_b_Auto_Mode)) {
          int_State_Sequencer = int_State_GoOptPos;
        }
        break;

  case int_State_SolarScan:
        Servo_LightScan();

        if (Timer_Trigger_Auto_Sequence) { 
           int_State_Sequencer = int_State_GoOptPos;
        }
        else {
          int_State_Sequencer = int_State_Idle;}
        break;

  case int_State_GoOptPos:
        Go_To_Opt_Pos();
        Timer_Trigger_Auto_Sequence = LOW;
        int_State_Sequencer = int_State_Idle;
        break;

  //default:
    // if nothing else matches, do the default
    // default is optional
  }
}



