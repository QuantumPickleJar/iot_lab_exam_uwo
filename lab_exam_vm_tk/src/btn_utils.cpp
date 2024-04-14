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

/// @brief Sets the pins mode of the pins wired to the button.
/// Also sets awaiting_first_press to false as we assume a new run
void initButtonPin() {
  awaiting_first_press = true;
  last_btn_state = digitalRead(pin_btn);
  pinMode(pin_btn, INPUT_PULLUP); // intialize button pin mode to pull-up resistor
}

/// @brief Minifying function to check if the right button is CURRENTLY being pushed, 
/// strictly in the instantaneous moment
bool isButtonActive() { return (digitalRead(pin_btn) == LOW); }

/// @brief The outward-facing function of this lib.  
/// Robustly checks the button to provide means of distinguishing debounce 
/// events from genuine human input. 
/// Accomplished by performing a second call to isButtonActive
bool isReceivingPureInput() {
  // first and foremost, get the latest button state
  int current_btn_state = isButtonActive() ? LOW : HIGH; 
  
  if (last_btn_state != current_btn_state) {      // has state changed? 
    // store + reset the timer, so we can evaluate the length of the event
    msSinceLastPress = millis();
  }

  // has NEW state been held for longer than the debounce delay?
  if (millis() - msSinceLastPress > DEBOUNCE_DELAY_MS) {
    if (last_btn_state != current_btn_state) {
      // we separately handle HIGH presses as they're what we're looking for
      if (current_btn_state == HIGH) {
        awaiting_first_press = false;    // future presses won't set 
        Serial.println("Button pressed!");
        return true;
      }
    }
  }
  // crucially, we have to set the GLOBALLY tracked member LAST
  last_btn_state = current_btn_state;
  return false;
}