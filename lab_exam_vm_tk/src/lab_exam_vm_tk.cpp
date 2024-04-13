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

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

/// These variables are for convenience and add a bit of overhead
const int buzzer = A2;

double p2_humidity, p2_temperature;
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

void setup() {
  // Put initialization like pinMode and begin functions here
  //initButtonPin();
  pinMode(buzzer, OUTPUT);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  if(awaiting_first_press) { 
    
  // Handle first-time press, setting pilot_push_set appropriately
  //     set initial heat index
  
  // else after first-time-run:
  checkTemp();  // instruct the TMP36 to perform a new read
  p2_temperature = getTemperatureInUnit(Celsius);
  //p2_humidity =  /*getHumiditySHT40*/;
  // temp_utils -> write and call a getHeatIndex function
  
  // 
  

  } 
}