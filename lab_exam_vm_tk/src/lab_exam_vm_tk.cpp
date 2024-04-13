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

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

/// These variables are for convenience and add a bit of overhead
const int buzzer = A2;

double p2_humidity, p2_temperature, initialHeatIndex, heatIndex;

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
  awaiting_first_press = true;
}

void loop() {
  Serial.println("");

  //checking to see if we are not waiting for the button press
  if(!awaiting_first_press) { 
    //get and set the temp
    //p2_temperature = ...;
    //get and set the humidity
    //p2_humidity = ...;

    //call the heat index fuction with the new values and set to a variable
    heatIndex = calcHeatIndex(p2_temperature, p2_humidity);

    //display the temp, humidity, heat index on screen
    //here

    //play buzzer noise
    if(heatIndex > (initialHeatIndex + .5)) {
      //if heat index is greater than initial, play ascending noise
      playAsc();

    } else if(heatIndex < (initialHeatIndex - .5)) {
      //if heat index is less than initial, play descending noise
      playDesc();

    } else {
      //if heat index is the same as the initial, play same noise
      playSame();

    }

  //else see if button is pressed and set stuff or just keep waiting
  } else {
    Serial.println("Checking for input...");
    if(isReceivingPureInput()) {

      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!9510541!!!!!
      //set the humidity and temperature here
      //p2_temperature = ...;
      //p2_humidity = ...;
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      //this sets the inital heat index to the heat 
      //index of when the button was pressed for the first time
      initialHeatIndex = calcHeatIndex(p2_temperature, p2_humidity);
      Serial.println("Heat index: ");

      Serial.print(initialHeatIndex);
      //setting the wait for first press to be false because we are no longer waiting.
      awaiting_first_press = false;
    } else {
    Serial.println("No input detected.");
    }

  }
  Serial.println("Systems test");
  delay(500);
}