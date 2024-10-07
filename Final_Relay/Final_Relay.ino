const int relayPin1 = 5;
const int relayPin2 = 6;
const int battery1 = A1;
const int battery2 = A2;

float analogToVoltage(int analogValue) {
  const float referenceVoltage = 5.0; // Assuming 5V reference for the analog input
  const int resolution = 1023; // 10-bit ADC gives a value from 0 to 1023
  return (analogValue / float(resolution)) * referenceVoltage;
}


void setup() {
 Serial.begin(9600);
 pinMode(relayPin1, OUTPUT);
 pinMode(relayPin2, OUTPUT);
}

void loop() {
 int bVoltage1=analogRead(battery1);
 int bVoltage2=analogRead(battery2);

 if (bVoltage1 >= bVoltage2)
 {digitalWrite(relayPin1, LOW);
 digitalWrite(relayPin2, HIGH);
 
 }

 else
 {digitalWrite(relayPin1, HIGH);
 digitalWrite(relayPin2, LOW);
}

Serial.print("Voltage of battery 1= ");
Serial.println(analogToVoltage(bVoltage1));

Serial.print("Voltage of battery 2= ");
Serial.println(analogToVoltage(bVoltage2));

delay(1000);
}
