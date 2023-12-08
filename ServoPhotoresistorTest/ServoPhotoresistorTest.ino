#include <Servo.h>
Servo servo1;
int pos=0;
int photoPin= A0;
int min_measurement= 1023;
void setup()
{
  servo1.attach(9);
}
void loop() {
  for (pos = 0; pos <= 180; pos += 1) 
  { 
    servo1.write(pos);             
    delay(15);                       
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    servo1.write(pos);              
    delay(15);                       
  }
}

