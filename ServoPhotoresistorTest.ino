#include <Servo.h>
Servo servo1;
int servopos=0;           
int photoPin= A0;        //analog input of photoresistor measurements
int light;              //voltage value of photoresistor output
int max_measurement= 0; //initial voltage value
int opt_pos=0;         //position of maximum voltage output
void setup()
{
  servo1.attach(9);
  Serial.begin(9600);
}
void Servo_LightScan() {
   for (servopos = 0; servopos <= 180; servopos += 1) //motor scans the sky
  { 
    servo1.write(servopos);
    light= analogRead(photoPin);           
    delay(100);   

    Serial.println("Current position: ");
    Serial.println(servopos);

    Serial.println("Current light: ");
    Serial.println(light);  
    Serial.println();
   if (light>max_measurement) {
    max_measurement = light;
    opt_pos = servopos;
  }   //saves the maximum voltage recorded and its corresponding position             
  }
  delay(15);
 servo1.write(opt_pos); //servo moves to optimal position
 delay(10000); //waits 10 seconds before scanning again
  
}
void loop() {
  Servo_LightScan();
  Serial.println("Optimal Pos: ");
  Serial.println(opt_pos);
   Serial.println("Optimal meas: ");
  Serial.println(max_measurement);

}

