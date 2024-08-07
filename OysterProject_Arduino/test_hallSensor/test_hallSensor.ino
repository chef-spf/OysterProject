const unsigned long overflowMicros = 4294967295UL;

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  
  // Accounting for delay in initialization
  unsigned long currentTime = micros();
  unsigned long delayTime = 1000000 - (currentTime % 1000000);
  delayMicroseconds(delayTime);
}

void loop() {
  //Code for runtime, accounting for micros() reset
  unsigned long currentMicros = micros();
  unsigned long lastMicros = 0;
  unsigned long runtime = 0;

  // Check if micros() has overflowed
  if (currentMicros < lastMicros) {
    runtime += (overflowMicros - lastMicros + currentMicros);
  } else {
    runtime += (currentMicros - lastMicros);
  }
  lastMicros = currentMicros;

  int analogA1 = analogRead(A1); // Read the sensor value
  float voltageA1 = analogA1 * (5 / 4095.0); // Converting analog reading to voltage

  // Print variables
  Serial.print(runtime);
  Serial.print(", ");
  Serial.print(analogA1);
  Serial.print(", ");
  Serial.println(voltageA1);

  delay(500);
}
