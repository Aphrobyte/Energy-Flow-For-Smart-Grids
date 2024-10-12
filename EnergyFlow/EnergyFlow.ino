#include <Servo.h>

/////////////////////////////////////////////>^.^<
//Variables Declaration
////////////////////////////////////////////

//Input variables
//==========================================================//
//Servo Motors
Servo servo_PhotoResistor;     //Photoresistor servo motor
Servo servo_PhotovoltaicCell;  //Photovoltaic cell servo motor

//Analog Inputs
const int in_int_PhotoResistor_Measured   = A0;   // Analog input of photoresistor measurements reading from pin A0
const int in_int_Photovoltaic_Voltage     = A1;   // Analog input of Photovoltaic voltage
const int in_int_Battery_A_Voltage        = A2;   // Analog input of Battery A
const int in_int_Battery_B_Voltage        = A4;   // Analog input of Battery B

//Boolean Inputs
const int in_int_Manual_Mode = 2;               // b_Auto_mode is hardware connected to Arduino digital pin 2 and is configured as digital input Read
const int in_int_Scan_Servo_PhotoResistor = 3;  // b_Scan_Servo_PhotoResistor is hardware connected to Arduino digital pin 3 and is configured as digital input Read 
const int in_int_Goto_Optimal_Servo_Pos  = 4;   // b_Goto_Optimal_Servo_Pos is hardware connected to Arduino digital pin 4 and is configured as digital input Read 
const int in_int_RelayPin_A  = 5;               // Relay A Output
const int in_int_RelayPin_B  = 6;               // Relay B Output



//Internal Variables
//==========================================================//


int int_optimal_Servo_pos = 0;   //position of maximum (optimal) Photoresistor voltage output
int int_PhotoResistor_Measured;   //Analog input of photoresistor measurements
int int_Max_PhotoResistor_Measured = 0; //Maximum Voltage

//Boolean Inputs
bool b_Scan_Servo_PhotoResistor; //When button 3 is pressed the PhotoResistor Servo to perform 180 degrees solar scan  
bool b_Goto_Optimal_Servo_Pos;   //When button 4 is pressed the Photovoltaic Servo goes to optimal position found during solar scan  
bool b_Auto_Mode;                 //When switch is set to TRUE (AutoMode) Sequence runs in autom i.e. SolarScanning, Photovoltaic going to optimal position 


float r_Battery_A_Voltage;         
float r_Battery_B_Voltage;
float r_Photovoltaic_Voltage;



const int int_State_Idle = 10; 
const int int_State_SolarScan = 20; 
const int int_State_GoOptPos = 30; 
int int_State_Sequencer = int_State_Idle; // Solar Tracking Sequencer; the sequencer has three states idle -> SolarScan -> GoToOptPos


unsigned long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long t_TimerInterval = 60000UL;     //Every minute  

bool Timer_Trigger_Auto_Sequence; // When mode is in AUTO and timer period is elapsed the solar scanning sequence is triggered 

bool b_State_SolarScan_Auto; //When TRUE and Mode is AUTO Servo_LightScan() is being executed 
bool b_State_GoOptPos_Auto; //When TRUE and Mode is AUTO Go_To_Opt_Pos() is being executed
bool b_State_Idle_Auto;     //When TRUE and Mode is AUTO State_Sequencer goes to State_Idle state


void setup()
{
  servo_PhotoResistor.attach(9);     //photoresistor servo motor is connected to digital pin 9
  servo_PhotovoltaicCell.attach(10); //photovoltaic cell servo motor is connected to digital pin 10
  Serial.begin(9600);

  pinMode(2, INPUT); //Solar Scan Sequence Mode (AUTO if HIGH, MANUAL if LOW)
  pinMode(3, INPUT); //When Pressed and Solar Scan Sequence is MANUAL it perform SolarScanning
  pinMode(4, INPUT); //When Pressed and Solar Scan Sequence is MANUAL Servo goes to Optimal Position (found during SolarScanning)
  pinMode(5, OUTPUT); //Digital Output to Relay A
  pinMode(6, OUTPUT); //Digital Output to Relay B
}

void Servo_LightScan() {
  int_Max_PhotoResistor_Measured = 0;
  int int_servo_pos;

  for (int_servo_pos = 30; int_servo_pos <= 150; int_servo_pos += 1) //motor performs 30 deg - 150 deg scanning
  {    
    servo_PhotoResistor.write(int_servo_pos); 
    int_PhotoResistor_Measured = analogRead(in_int_PhotoResistor_Measured);    //During solar scan we read the Photoresistor        
    delay(100);   //Delay of 100ms to allow servo setle at desired position

   if (int_PhotoResistor_Measured > int_Max_PhotoResistor_Measured) {
      int_Max_PhotoResistor_Measured = int_PhotoResistor_Measured; //During Solar scan we record the maximum Photoresistor value 
      int_optimal_Servo_pos = int_servo_pos; //During Solar scan we record the servo pos max Photoresistor val occurs 
      }  
  }
}

void Go_To_Opt_Pos(){
 // int_optimal_Servo_pos = 90;
  servo_PhotovoltaicCell.write(int_optimal_Servo_pos); //photovoltaic cell servo moves to optimal position found during solar scan
 }

 void Relay() {

  r_Battery_A_Voltage = analogToVoltage(analogRead(in_int_Battery_A_Voltage));
  r_Battery_B_Voltage = analogToVoltage(analogRead(in_int_Battery_B_Voltage));

  r_Photovoltaic_Voltage = analogToVoltage(analogRead(in_int_Photovoltaic_Voltage));
  

  if (r_Battery_A_Voltage > r_Battery_B_Voltage +0.1)
    {  
        digitalWrite(in_int_RelayPin_A, LOW);
        digitalWrite(in_int_RelayPin_B, HIGH);
    }
  
  else if (r_Battery_B_Voltage > r_Battery_A_Voltage +0.1)
    {
        digitalWrite(in_int_RelayPin_A, HIGH);
        digitalWrite(in_int_RelayPin_B, LOW);
    }
 }

float analogToVoltage(int analogValue) {
  const float referenceVoltage = 3.7; // Assuming 3.7V battery  voltage
  const int resolution = 1023; // 10-bit ADC gives a value from 0 to 1023
  return (analogValue / float(resolution)) * referenceVoltage + 0.3;
}


void loop() {

  Relay();


  b_Scan_Servo_PhotoResistor = digitalRead(in_int_Scan_Servo_PhotoResistor);
  b_Goto_Optimal_Servo_Pos = digitalRead(in_int_Goto_Optimal_Servo_Pos);
  b_Auto_Mode = !digitalRead(in_int_Manual_Mode);

  

  if (!b_Auto_Mode) {
    Timer_Trigger_Auto_Sequence = LOW;
  }
    if (millis() - previousMillis > t_TimerInterval) 
  {

    Timer_Trigger_Auto_Sequence = b_Auto_Mode;
    // save the last time you blinked the LED
    previousMillis += t_TimerInterval;  

  }




int_PhotoResistor_Measured = analogRead(in_int_PhotoResistor_Measured);     
Serial.println("Sens Val: "  + String(int_PhotoResistor_Measured) + ", Max Sens Val: " + String(int_Max_PhotoResistor_Measured) + ", Opt Pos: " + String(int_optimal_Servo_pos) + ", Battery A Volt:" + String(r_Battery_A_Voltage) +  ", Battery B Volt:" + String(r_Battery_B_Voltage) + ", PhotoVolt Volt: "  +  String(r_Photovoltaic_Voltage));

  switch (int_State_Sequencer) {

  case int_State_Idle:

        if (((b_Scan_Servo_PhotoResistor) && (int_State_Sequencer == int_State_Idle) && (!b_Auto_Mode) ) ||   ( b_Auto_Mode && Timer_Trigger_Auto_Sequence   ) ){
          int_State_Sequencer = int_State_SolarScan; 
          }
        if ((b_Goto_Optimal_Servo_Pos) && (int_State_Sequencer == int_State_Idle) && (!b_Auto_Mode)) {
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

  }
}



