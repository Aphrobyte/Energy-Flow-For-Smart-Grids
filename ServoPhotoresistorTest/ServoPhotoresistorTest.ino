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

int int_State_Sequencer;
const int int_State_Idle = 0; 
const int int_State_SolarScan = 1; 
const int int_State_GoOptPos = 2; 

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
    //delay(15);   

    Serial.println("Current position: ");
    Serial.println(int_servo_pos);

    Serial.println("Current light: ");    
    Serial.println(int_PhotoResistor_Measured);  
  
   if (int_PhotoResistor_Measured > int_Max_PhotoResistor_Measured) {
    int_Max_PhotoResistor_Measured = int_PhotoResistor_Measured;
    int_optimal_Servo_pos = int_servo_pos; //assignment of maximum light measurement and optimal position values during servo motion
   }  
  
  delay(15);
  }
}

void Go_To_Opt_Pos(){
 servo_PhotovoltaicCell.write(int_optimal_Servo_pos); //photovoltaic cell servo moves to optimal position found during solar scan
 }



void loop() {

  in_b_Scan_Servo_PhotoResistor = digitalRead(b_Scan_Servo_PhotoResistor);
  in_b_Goto_Optimal_Servo_Pos =digitalRead(b_Goto_Optimal_Servo_Pos);

  // if ((in_b_Scan_Servo_PhotoResistor == true) && (b_Scan_Servo_PhotoResistor_Active == false)) {
  //   b_Scan_Servo_PhotoResistor_Active = true;
  //   b_Goto_Optimal_Servo_Pos_Active = false;
   
  // } //pushing the button triggers a set of actions

  // if ((in_b_Goto_Optimal_Servo_Pos == true) && (b_Goto_Optimal_Servo_Pos_Active == false)){
  //   b_Scan_Servo_PhotoResistor_Active = false;
  //   b_Goto_Optimal_Servo_Pos_Active = true;
  // } 



  switch (int_State_Sequencer) {

  case int_State_Idle:

      if ((in_b_Scan_Servo_PhotoResistor == true) && (b_Scan_Servo_PhotoResistor_Active == false)) {
        b_Scan_Servo_PhotoResistor_Active = true;
        b_Goto_Optimal_Servo_Pos_Active = false;
   
        } //pushing the button triggers a set of actions

      if ((in_b_Goto_Optimal_Servo_Pos == true) && (b_Goto_Optimal_Servo_Pos_Active == false)){
         b_Scan_Servo_PhotoResistor_Active = false;
         b_Goto_Optimal_Servo_Pos_Active = true;
        } 

  case int_State_SolarScan:
    Servo_LightScan();
    b_Scan_Servo_PhotoResistor_Active = false;
    break;
  case int_State_GoOptPos:
    // do something when var equals 2
    break;
  default:
    // if nothing else matches, do the default
    // default is optional
    break;
}


 if (b_Scan_Servo_PhotoResistor_Active == true) {
   Servo_LightScan();
   b_Scan_Servo_PhotoResistor_Active = false; //only allows the Servo_LightScan function to run once
  }

 if (b_Goto_Optimal_Servo_Pos_Active == true) {
    Go_To_Opt_Pos();
    Serial.println("Optimal Position: ");
    Serial.println(int_optimal_Servo_pos);
    Serial.println("Maximum light measurement: ");
    Serial.println(int_Max_PhotoResistor_Measured);
  
   b_Goto_Optimal_Servo_Pos_Active = false;
  }
 
}



