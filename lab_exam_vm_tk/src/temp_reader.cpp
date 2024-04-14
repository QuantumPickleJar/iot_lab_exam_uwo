/*
 * Project myProject
 * Author: Your Name
 * Date:
 * Bugs:
 * Ubidots will not allow multiple Tokens to be created (at least in our case)
 * so we had to have one member remove their yet-to-be-graded token from Lab8
 * to be used here.
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "temp_utils.h"
#include "btn_utils.h"
#include "Adafruit_SSD1306_RK.h"
#include "Ubidots.h"
#include "secrets.h"

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define UBIVAR_TEMP_F "temp_f" 
#define UBIVAR_HUMIDITY "humidity"
#define UBIVAR_HEAT_INDEX "heat_index"


SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

// extern Adafruit_SHT4x sht4;  // defined in temp_utils
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Ubidots ubidots(TOKEN, UBI_HTTP);

/// These variables are for convenience and add a bit of overhead
const int pin_buzzer = A2;
const int SERIAL_DELAY_MS = 10000;

/// @brief timer that allows the cycle to continue without being blocked by a delay call
uint32_t msIdleTime;    


double p2_humidity, p2_temperature, initialHeatIndex, heatIndex;
extern bool awaiting_first_press;

/// @brief plays an ascending tone of chirps on the buzzer
void playAsc() {

  tone(pin_buzzer, 523);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 587);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 659);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 698);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 784);
  delay(100);
  noTone(pin_buzzer);
  delay(100);
}

/// @brief plays a descending tone of chirps on the buzzer
void playDesc() {

  tone(pin_buzzer, 784);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 698);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 659);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 587);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 523);
  delay(100);
  noTone(pin_buzzer);
  delay(100);
}

/// @brief plays a static note five times
/// @todo add an optional parameter
void playSame() {

  tone(pin_buzzer, 523);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 523);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 523);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 523);
  delay(100);
  noTone(pin_buzzer);
  delay(100);

  tone(pin_buzzer, 523);
  delay(100);
  noTone(pin_buzzer);
  delay(100);
}

void resetScreen(){ 
  display.clearDisplay();      // clear any existing pixels
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
}

/// @brief Prints a simple msg to the screen to hint to operators that 
/// their screen is working and not defective
/// displays "Waiting..." on the screen
void initScreen() {
  resetScreen();
  display.println("Waiting...");
  display.display();
}

/// @brief Prints the temperature information to the OLED screen
void printTempInfo() {
  resetScreen();      // clear the display  
  display.printlnf("H: %3.1f%%", p2_humidity);
  display.printlnf("T: %3.1fF", p2_temperature);
  display.printlnf("HI: %3.1fF", heatIndex);
  display.display();    // don't forget to refresh the display buffer!
}

void setup() {
  // pulled from the example library, won't work without this 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);  // halt!
  } 

  pinMode(pin_buzzer, OUTPUT);
  initSHT40();
  initButtonPin();
  initScreen();
}

void loop() {
  sensors_event_t humidity, temp;
  
  //checking to see if we are not waiting for the button press
  if(!awaiting_first_press) {
    uint32_t timestamp = millis();
    sht4.getEvent(&humidity, &temp);
    timestamp = millis() - timestamp;
    
    //get and set the temp + humidity
    p2_temperature = convertTemperature(temp.temperature, Celsius, Fahrenheit);
    p2_humidity = (double)humidity.relative_humidity;

    //call the heat index fuction with the new values and set to a variable
    heatIndex = calcHeatIndex(p2_temperature, p2_humidity);
    
    Serial.println("Humidity: " + (String)p2_humidity);
    Serial.println("Temp: " + (String)p2_temperature);

    ubidots.add(UBIVAR_TEMP_F, p2_temperature);
    ubidots.add(UBIVAR_HUMIDITY, p2_humidity);
    ubidots.add(UBIVAR_HEAT_INDEX, heatIndex);

    ubidots.send();

    //display the temp, humidity, heat index on screen here
    Serial.println("Heat index: " + (String)heatIndex);
    printTempInfo();
    
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
    //  Button has not yet been pressed, flow-of-control should fall here until 
    //  the button press has been received. 
    if(isReceivingPureInput()) {  // on first time run: if button pressed...
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