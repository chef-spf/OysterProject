#include "RTClib.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

RTC_PCF8523 rtc;
String filename;

const int sensorPins[] = {A0, A1, A2, A3, A6, A7}; // Array of sensor pins
const int numSensors = sizeof(sensorPins) / sizeof(sensorPins[0]); // Calculate the number of sensors
const int chipSelect = 21; // CS pin for SD card module

// __________________________________________________________________________________________________________________________
// __________________________________________________________________________________________________________________________

void setup() {
  Serial.begin(57600); // Start serial communication
  while (!Serial) {
    ;
  }
  Serial.println("Serial connection established");
  delay(1000);

//----------------------------------------------------------------------------------
  // Checks if RTC is found, if not, enter infinite loop 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  
  // Checks if RTC is not initialized or has lost power, if so, sets date and time to when sketch was compiled
  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  // Clears the STOP bit required for the RTC to restart after being stopped
  rtc.start();
  
  // Calibration function: calibrateRTC(drift, period)
  calibrateRTC(0, 7);

//----------------------------------------------------------------------------------
  // Initialize SD card 
  if (!SD.begin(chipSelect)) {
    Serial.println("Card Mount Failed");
    return;
  }

  // Check if SD card is attached
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  // Read card type
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  // Read card size
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  
  // Generate a new filename based on date and time
  filename = generateFilename();

  // Create new file with new filename and CSV header
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.println("Date,Time,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5,Sensor6");
    dataFile.close();
  } else {
    Serial.println("Failed to create file");
  }
}

// __________________________________________________________________________________________________________________________
// __________________________________________________________________________________________________________________________

void loop() {
  // Create array of hall sensor values, read and convert to voltage
  float hallVoltages[numSensors];
  for (int i = 0; i < numSensors; i++) {
    int hallAnalog = analogRead(sensorPins[i]);
    hallVoltages[i] = hallAnalog * (5.0 / 4095.0);
  }

  // Get RTC time
  DateTime now = rtc.now();

  // Function printing RTC and sensor data
  printData(now, hallVoltages, numSensors);

  // Function to write data to SD card
  writeDataToSD(now, hallVoltages, numSensors);

  delay(1000);
}

// __________________________________________________________________________________________________________________________
// __________________________________________________________________________________________________________________________

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

//----------------------------------------------------------------------------------
// Function to generate a filename based on the current date and time
String generateFilename() {
  DateTime now = rtc.now();
  char buf[25];
  sprintf(buf, "/data_%04d%02d%02d_%02d%02d%02d.csv", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  return String(buf);
}

//----------------------------------------------------------------------------------
// Function to print sensor data in serial monitor
void printData(DateTime now, float hallVoltages[], int numSensors) {
  Serial.print("Date: (");  
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(") Time: [");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print("]");
  Serial.println();
  
  for (int i = 0; i < numSensors; i++) {
    Serial.print(hallVoltages[i]);
    if (i < numSensors - 1) {
      Serial.print(", ");
    }
  }
  Serial.println(); Serial.println();
}

//----------------------------------------------------------------------------------
// Function to write sensor data to SD card
void writeDataToSD(DateTime now, float hallVoltages[], int numSensors) {
  // Open the file for writing
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    // Write the timestamp
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
    dataFile.print(',');
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.print(',');

    // Write the sensor data
    for (int i = 0; i < numSensors; i++) {
      dataFile.print(hallVoltages[i]);
      if (i < numSensors - 1) {
        dataFile.print(',');
      }
    }
    dataFile.println();

    // Close the file
    dataFile.close();
  } else {
    Serial.println("Failed to open file for writing");
  }
}
