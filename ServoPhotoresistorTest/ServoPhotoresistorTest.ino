#include <Servo.h>
Servo servo_pr;         //Photoresistor servo motor
Servo servo_pc;         //Photovoltaic cell servo motors
int servo_pos=0;           
int photoPin= A0;        //analog input of photoresistor measurements
int light;              //voltage value of photoresistor output
int max_measurement= 0; //initial voltage value
int opt_pos=0;         //position of maximum voltage output

bool scan_button;      //instructs the circuit to perform solar scan    
const int button1=3;    
bool scan_set;

bool pos_button;        //instructs the circuit to go to optimal position
bool pos_button_ack;   //confirms that pos button is pressed
bool pos_set; 
const int button2=4;    

void setup()
{
  servo_pr.attach(9); //photoresistor servo motor is connected to digital pin 9
  servo_pc.attach(10); //photovoltaic cell servo motor is connected to digital pin 10
  Serial.begin(9600);
  pinMode(3, INPUT); //button1 is connected to digital pin 3
  pinMode(4, INPUT); //button2 is connected to digital pin 4
}
void Servo_LightScan() {
  max_measurement = 0;
  for (servo_pos = 0; servo_pos <= 180; servo_pos += 1) //motor performs 180 degrees movement
  {    
    servo_pr.write(servo_pos);
    light= analogRead(photoPin);           
    //delay(15);   

    Serial.println("Current position: ");
    Serial.println(servo_pos);

    Serial.println("Current light: ");    
    Serial.println(light);  
  
   if (light>max_measurement) {
    max_measurement = light;
    opt_pos = servo_pos; //assignment of maximum light measurement and optimal position values during servo motion
   }  
  
  delay(15);
  }
}

void Go_To_Opt_Pos(){
 servo_pc.write(opt_pos); //photovoltaic cell servo moves to optimal position
 }



void loop() {

  scan_button=digitalRead(button1);
  pos_button=digitalRead(button2);

  if ((scan_button == true) && (scan_set == false)) {
    scan_set = true;
    pos_button = false;
    pos_set = false;
  } //pushing the button triggers a set of actions

  if ((pos_button == true) && (pos_button_ack == false)){
    scan_set = false;
    //pos_button = false;
    pos_set = true;
    pos_button_ack = true;
  } 

if (pos_button == false) {
   pos_button_ack = false;
}


 if (scan_set == true) {
   Servo_LightScan();
   scan_set = false; //only allows the Servo_LightScan function to run once
  // scan_button=false;
  }

 if (pos_set == true) {
    //scan_set = false;
    Go_To_Opt_Pos();
    Serial.println("Optimal Position: ");
    Serial.println(opt_pos);
    Serial.println("Maximum light measurement: ");
    Serial.println(max_measurement);
  
   pos_set = false;
  }
 
}

