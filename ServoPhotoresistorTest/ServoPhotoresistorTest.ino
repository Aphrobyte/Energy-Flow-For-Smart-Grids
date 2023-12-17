#include <Servo.h>
Servo servo1;
int servo_pos=0;           
int photoPin= A0;        //analog input of photoresistor measurements
int light;              //voltage value of photoresistor output
int max_measurement= 0; //initial voltage value
int opt_pos=0;         //position of maximum voltage output

bool scan_button;
const int button1=10;     //instructs the circuit to perform solar scan    
bool scan_set;

bool pos_button;
const int button2=11;     //instructs the circuit to perform solar scan

void setup()
{
  servo1.attach(9);
  Serial.begin(9600);
  pinMode(10, INPUT); //button is connected to digital pin 10
  pinMode(11, INPUT);
}
void Servo_LightScan() {
   for (servo_pos = 0; servo_pos <= 180; servo_pos += 1) //motor scans the sky
  { 
    servo1.write(servo_pos);
    light= analogRead(photoPin);           
    delay(100);   

    Serial.println("Current position: ");
    Serial.println(servo_pos);

    Serial.println("Current light: ");
    Serial.println(light);  
    Serial.println();
   if (light>max_measurement) {
    max_measurement = light;
    opt_pos = servo_pos;
  }  

  
  delay(15);}
}

void Go_To_Opt_Pos() {
 servo1.write(opt_pos); //servo moves to optimal position
}



void loop() {
  scan_button=digitalRead(button1);
  pos_button=digitalRead(button2);


  if (scan_button == true) {
    scan_set = true;
  }

 if (scan_set == true) {
  Servo_LightScan();
  Serial.println("Optimal Position: ");
  Serial.println(opt_pos);
  Serial.println("Maximum light measurement: ");
  Serial.println(max_measurement);
  scan_set = false;
  scan_button=false;
  }

 if (pos_button == true) {
    scan_set = false;
    Go_To_Opt_Pos();
  }

 Serial.println(scan_set);
}

