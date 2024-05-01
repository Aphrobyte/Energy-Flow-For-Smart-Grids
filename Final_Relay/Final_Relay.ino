const int relayPin1 = 2;
const int relayPin2 = 3;
const int battery1 = A0;
const int battery2 = A1;

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
Serial.println(bVoltage1);

Serial.print("Voltage of battery 2= ");
Serial.println(bVoltage2);

delay(1000);
}
