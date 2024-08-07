#include "RTClib.h"

RTC_PCF8523 rtc;

const int sensorPins[] = {A0, A1, A2, A3, A6, A7}; // Array of sensor pins
const int numSensors = sizeof(sensorPins) / sizeof(sensorPins[0]); // Calculate the number of sensors

void setup() {
  Serial.begin(57600); // Start serial communication
  while (!Serial) {
    delay(10); // Wait for serial connection
  }
  Serial.println("Serial connection established");
  delay(1000);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.start();
  calibrateRTC(0, 7);
}

void loop() {
  // Create array of hall sensor values, read and convert to voltage
  float hallVoltages[numSensors];
  for (int i = 0; i < numSensors; i++) {
    int hallAnalog = analogRead(sensorPins[i]);
    hallVoltages[i] = hallAnalog * (5.0 / 4095.0);
  }

  // Get RTC time
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  for (int i = 0; i < numSensors; i++) {
    Serial.print(',');
    Serial.print(hallVoltages[i]);
  }
  Serial.println();
  delay(60000);
}

// Function to calibrate RTC in setup
void calibrateRTC(float drift, int observationDays) {
  float period_sec = observationDays * 86400; // total observation period in seconds
  float deviation_ppm = (drift / period_sec * 1000000); // calculates deviation (ppm) based on drift and period
  float drift_unit = 4.34; // sets drift unit based on the calibration mode (every two hours)
  int offset = round(deviation_ppm / drift_unit); // calculates offset required for calibration

  //rtc.calibrate(PCF8523_TwoHours, offset);  // calibrate RTC
  //rtc.calibrate(PCF8523_TwoHours, 0);  // uncomment this line to cancel previous calibration

  Serial.print("Offset is "); Serial.println(offset);
}
