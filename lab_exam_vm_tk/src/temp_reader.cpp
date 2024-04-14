/*
 * Project myProject
 * Author: Your Name
 * Date:
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "temp_utils.h"
#include "btn_utils.h"
// #include "Adafruit_SHT4x.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

// extern Adafruit_SHT4x sht4;  // defined in temp_utils

/// These variables are for convenience and add a bit of overhead
const int buzzer = A2;
const int SERIAL_DELAY_MS = 5000;

/// @brief timer that allows the cycle to continue without being blocked by a delay call
uint32_t msIdleTime;    


double p2_humidity, p2_temperature, initialHeatIndex, heatIndex;
extern bool awaiting_first_press;

/// @brief plays an ascending tone of chirps on the buzzer
void playAsc() {

  tone(buzzer, 523);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 587);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 659);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 698);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 784);
  delay(50);
  noTone(buzzer);
  delay(50);
}

/// @brief plays a descending tone of chirps on the buzzer
void playDesc() {

  tone(buzzer, 784);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 698);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 659);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 587);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 523);
  delay(50);
  noTone(buzzer);
  delay(50);
}

/// @brief plays a static note five times
/// @todo add an optional parameter
void playSame() {

  tone(buzzer, 523);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 523);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 523);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 523);
  delay(50);
  noTone(buzzer);
  delay(50);

  tone(buzzer, 523);
  delay(50);
  noTone(buzzer);
  delay(50);
}

void setup() {
  pinMode(buzzer, AN_OUTPUT);

  //setting waiting for press to be true because we are waiting for it
  // awaiting_first_press = true;
  initSHT40();
  initButtonPin();
}

void loop() {
  // Serial.println("Is bool valid?: " + (String)awaiting_first_press);
  // TODO: print "Waiting: to the screen"
  //checking to see if we are not waiting for the button press
  sensors_event_t humidity, temp;
  
  if(!awaiting_first_press) {
 
    // TODO: resume from here
    //get and set the temp
    uint32_t timestamp = millis();
    sht4.getEvent(&humidity, &temp);
    timestamp = millis() - timestamp;
    
    //get and set the humidity
    p2_temperature = temp.temperature;
    p2_humidity = (double)humidity.relative_humidity;

    //call the heat index fuction with the new values and set to a variable
    heatIndex = calcHeatIndex(p2_temperature, p2_humidity);
    
    Serial.println("Humidity: " + (String)p2_humidity);
    Serial.println("Temp: " + (String)p2_temperature);

    //display the temp, humidity, heat index on screen
    //here
    Serial.println("Heat index: " + (String)heatIndex);

    //play buzzer noise
    if(heatIndex > (initialHeatIndex + .5)) {
      //if heat index is greater than initial, play ascending noise
      playAsc();
      Serial.println("Above average!");
    } else if(heatIndex < (initialHeatIndex - .5)) {
      //if heat index is less than initial, play descending noise
      playDesc();
      Serial.println("Below average!");

    } else {
      //if heat index is the same as the initial, play same noise
      playSame();
      Serial.println("About the same!");
    }
  }  // End of standard runtime loop
  else if (awaiting_first_press) {
    /* Button has not yet been pressed, flow-of-control should fall here until 
     the button press has been  received. */
    if(isReceivingPureInput()) {

      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!9510541!!!!!
      //set the humidity and temperature here
      //p2_temperature = ...;
      //p2_humidity = ...;
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      //this sets the inital heat index to the heat 
      //index of when the button was pressed for the first time
      initialHeatIndex = calcHeatIndex(p2_temperature, p2_humidity);
      Serial.println("Heat index: " + (String)initialHeatIndex);

      //setting the wait for first press to be false because we are no longer waiting.
      awaiting_first_press = false;
    } 
  } // End of first-time-press "block"
  
  if(millis() - msIdleTime > SERIAL_DELAY_MS) {
    String debugMsgHeader = "Systems nominal";
    msIdleTime = millis();    // reset the counter
    String debugMsg;

    // TEMPORARY: COMMENT BEFORE SUBMITTING
    // append more to debugMsg if we're still in "first-time" run
    debugMsg = !awaiting_first_press ? "." : ": Awaiting input.";
    Log.info(debugMsgHeader + debugMsg); 
  }
}