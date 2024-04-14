/****
 * @file btn_utils.cpp
 * @date 2024-04-02
 * @author Vincent Morrill (vince.codefactory@outlook.com)
 * @brief the source file for providing button detection functionality
 * @version 0.2
 * 
 * @remarks
 * Should look into function pointers, would be neat to be able to have btn_utils not know 
 * anything about its consumer, I think this is called a FunctionPtr?
 *****/
#include "btn_utils.h"
#include "Particle.h"

const int pin_btn = D2;   // pin connected to first button (D2)
// const int pin_btn_right = D3;   // pin connected to button furthest from board (D3)
const int DEBOUNCE_DELAY_MS = 200;  // how many ms to wait before checking

bool awaiting_first_press;
int last_btn_state = LOW;        // the internally managed instance of button's state
uint32_t msSinceLastPress;          // Tracks # of ms since last verified push

// Serial-monitor info that makes tracing significantly easier and faster
const String DEBUG_MSG_PRESSED = "Non-bouncing button push confirmed!";
const String DEBUG_MSG_INIT = "############ First press detected! ############";


/// @brief Sets the pins mode of the pins wired to the button.
/// Also sets awaiting_first_press to false as we assume a new run
void initButtonPin() {
  awaiting_first_press = true;
  last_btn_state = digitalRead(pin_btn);
  pinMode(pin_btn, INPUT_PULLUP); // intialize button pin mode to pull-up resistor
}

/// @brief Minifying function, adjusted to work with the debouncing state logic
bool isButtonActive() { return (digitalRead(pin_btn) == LOW); }

/// @brief The outward-facing function of this lib.  
/// Robustly checks the button to provide means of distinguishing debounce 
/// events from genuine human input. 
/// Accomplished by performing a second call to isButtonActive
bool isReceivingPureInput() {
  // first and foremost, get the latest button state
  // int current_btn_state = isButtonActive() ? LOW : HIGH; 
  bool current_btn_state = isButtonActive(); 

  if (current_btn_state != (last_btn_state == HIGH)) {      // has state changed? 
    // store + reset the timer, so we can evaluate the length of the event
    msSinceLastPress = millis();
    last_btn_state = current_btn_state ? HIGH : LOW;  // Store the new state
  }

  // has NEW state been held for longer than the debounce delay?
  if (millis() - msSinceLastPress > DEBOUNCE_DELAY_MS && current_btn_state) {
    // Serial.println();
    String debugBtnMsg = !awaiting_first_press ? DEBUG_MSG_PRESSED : DEBUG_MSG_INIT; 
    // additionally verbose output:
    Serial.println(debugBtnMsg);

    return true;
  }
  // Serial.println("ERROR: noise detected")
  return false;
}