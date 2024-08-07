#include <Adafruit_VCNL4040.h>

Adafruit_VCNL4040 vcnl4040 = Adafruit_VCNL4040();

void setup() {
  Serial.begin(115200);
  // Wait until serial port is opened
  while (!Serial) { delay(1); }

  Serial.println("Adafruit VCNL4040 Config demo");

  if (!vcnl4040.begin()) {
    Serial.println("Couldn't find VCNL4040 chip");
    while (1);
  }
  Serial.println("Found VCNL4040 chip");

  setupVCNL4040();
}

void loop() {
  // Read proximity value
  uint16_t proximity = vcnl4040.getProximity();
  Serial.println(proximity);
  delay(10);
}


//-----------------------------------------------------------
void setupVCNL4040(){
  vcnl4040.enableAmbientLight(false);  // disable ambient light sensor
  vcnl4040.enableAmbientLightInterrupts(false); // disable ambient light interrupt
  vcnl4040.enableWhiteLight(false); // disable white light sensor
  vcnl4040.enableProximityInterrupts(VCNL4040_PROXIMITY_INT_DISABLE); // disable proximity sensor interrupt
  
  vcnl4040.setProximityHighResolution(true); // set to 16bit resolution (vs. 12bit)
  // Setting VCNL4040_LED_DUTY_1_40 gives shortest proximity measurement time (about 4.85ms)
  vcnl4040.setProximityLEDDutyCycle(VCNL4040_LED_DUTY_1_40); // 1_40, 1_80,1_160,1_320
  vcnl4040.setProximityLEDCurrent(VCNL4040_LED_CURRENT_200MA); // 50,75,100,120,140,160,180,200
  // Setting VCNL4040_PROXIMITY_INTEGRATION_TIME_1T gives the shortest pulse (lowest LED output)
  // in combination with the LED_CURRENT setting above. A longer integration time like
  // VCNL4040_PROXIMITY_INTEGRATION_TIME_8T raises the pulse length (higher LED output) in
  // combination with the LED_CURRENT setting.
  vcnl4040.setProximityIntegrationTime(VCNL4040_PROXIMITY_INTEGRATION_TIME_8T); // 1T,1_5T,2T,2_5T,3T,3_5T,4T,8T
  vcnl4040.enableProximity(true); // turn on IR sensor for right now
}