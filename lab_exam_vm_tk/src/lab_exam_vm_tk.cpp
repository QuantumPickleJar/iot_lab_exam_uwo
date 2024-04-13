/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "tmp36_utils.h"
#include "btn_utils.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

/// These variables are for convenience and add a bit of overhead
double p2_humidity, p2_temperature;
bool awaiting_first_press;
// static double 

// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
  initButtonPin();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  if(awaiting_first_press) { 
  // Handle first-time press, setting pilot_push_set appropriately
  //     set initial heat index
  
  // else after first-time-run:
  } else if(isReceivingPureInput()) {
    checkTemp();  // instruct the TMP36 to perform a new read
    p2_temperature = getTemperatureInUnit(Celsius);
    p2_humidity =  /*getHumiditySHT40*/;
    // temp_utils -> write and call a getHeatIndex function
  }
}