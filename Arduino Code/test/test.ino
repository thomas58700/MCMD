const int numReadings = 10;
int readIndex = 0;
int readings[numReadings];
//Analog Reference Voltage
#define aref_voltage 3.3

int tempPin = A1;

int tempReading;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
//analogReference(EXTERNAL);
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
readings[readIndex] = temperatureC;
Serial.println(readings[readIndex]);
readIndex = readIndex + 1;
if (readIndex >= numReadings) // if we're at the end of the array...
  {
    Serial.println("The 10 values in the array:");
    for(int i=0;i<readIndex;i=i+1)
    {
      Serial.println(readings[i]);
    }
    readIndex = 0;
  // Delay for sensor to stabilize
  }
  delay(2000);
  
}
