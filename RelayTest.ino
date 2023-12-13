const int relayPin = 8; // Pin connected to the relay
const int greenled = 9;
const int redled = 10;
const int batteryPin = A1;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT); // Set the relay pin as an output
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
}

void loop() {
   int sensorValue = analogRead(batteryPin); // Read analog value from battery pin
  float voltage = sensorValue * (5.0 / 1023.0) * (3.7 / 5.0); // Convert analog value to voltage (adjust 3.7 to your battery's full charge voltage)

  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.println("V");

  delay(1000); // Delay for stability
 if (voltage >= 3.0)
 {digitalWrite(relayPin, HIGH); // Turn the relay ON (close the circuit)
  digitalWrite(greenled, HIGH);
  digitalWrite(redled, LOW);
  delay(1000); }// Wait for 1 second
  else 
 {digitalWrite(relayPin, LOW);// Turn the relay OFF (open the circuit)
  digitalWrite(greenled, LOW);
  digitalWrite(redled, HIGH);
  delay(1000);} // Wait for 1 second
}