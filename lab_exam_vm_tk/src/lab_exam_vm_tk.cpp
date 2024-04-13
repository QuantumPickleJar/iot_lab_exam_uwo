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
#include "btn_utils.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

/// These variables are for convenience and add a bit of overhead
const int buzzer = A2;

double p2_humidity, p2_temperature, initialHeatIndex, heatIndex;
bool awaiting_first_press;

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

double getHeatIndex(double temperature, double humidity) {

  //initializing the variables for the equation
  double c1, c2, c3, c4, c5, c6, c7, c8, c9;
  c1 = -42.379;
  c2 = 2.04901523;
  c3 = 10.14333127;
  c4 = -.22475541;
  c5 = -6.83783 * pow(10,-3);
  c6 = -5.481717 * pow(10,-2);
  c7 = 1.22874 * pow(10,-3);
  c8 = 8.5282 * pow(10,-4);
  c9 = -1.99 * pow(10,-6);

  //heat index equation
  double hI = c1 + (c2 * temperature) 
                 + (c3 * humidity) 
                 + (c4 * temperature * humidity) 
                 + (c5 * pow(temperature, 2)) 
                 + (c6 * pow(humidity, 2)) 
                 + (c7 * pow(temperature, 2) * humidity) 
                 + (c8 * temperature * pow(humidity, 2)) 
                 + (c9 * pow(temperature, 2) * pow(humidity, 2));

  //returns the heat index
  return hI;

}

void setup() {

  pinMode(buzzer, AN_OUTPUT);

  //setting waiting for press to be true because we are waiting for it
  awaiting_first_press = true;
}

void loop() {

  //checking to see if we are not waiting for the button press
  if(!awaiting_first_press) { 
    
    //get and set the temp
    //p2_temperature = ...;
    //get and set the humidity
    //p2_humidity = ...;

    //call the heat index fuction with the new values and set to a variable
    heatIndex = getHeatIndex(p2_temperature, p2_humidity);

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

    if(isReceivingPureInput()) {

      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //set the humidity and temperature here
      //p2_temperature = ...;
      //p2_humidity = ...;
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      //this sets the inital heat index to the heat 
      //index of when the button was pressed for the first time
      initialHeatIndex = getHeatIndex(p2_temperature, p2_humidity);

      //setting the wait for first press to be false because we are no longer waiting.
      awaiting_first_press = false;


    }

  } 
}