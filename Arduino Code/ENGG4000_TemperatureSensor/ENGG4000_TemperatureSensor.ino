
//Analog Reference Voltage
#define aref_voltage 3.3

int tempPin = 1;

int tempReading;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
analogReference(EXTERNAL);
}

void loop() {
  // Read voltage value from sensor
  tempReading = analogRead(tempPin);
  //Print to serial monitor
  Serial.print("RAW Input = ");
  Serial.print(tempReading);

  // Convert to voltage
  float voltage = tempReading * aref_voltage;
  voltage /= 1024.0;

  //Print voltage to serial monitor
  Serial.print(" - ");
  Serial.print(voltage);
  Serial.print(" volts ");

  // Print the temperature
  // Convert from 10 mv per degree with 500 mv offset to degree celsius
  // (voltage - 500mv) * 10)
  float temperatureC = (voltage - 0.5) * 100;
  Serial.print(" --> Temperature ");
  Serial.print(temperatureC);
  Serial.println(" degrees C");

  // Delay for sensor to stabilize
  delay(1000);
  
}
