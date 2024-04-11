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

const int pin_btn_left = D2;   // pin connected to first button (D2)
const int pin_btn_right = D3;   // pin connected to button furthest from board (D3)

uint32_t msSinceLastPress;          // Tracks # of ms since last verified push
const int DEBOUNCE_DELAY_MS = 200;  // how many ms to wait before checking

/// @brief sets the pins mode of the pins wired to the buttons
void initButtonPin() {
  awaiting_first_press = false;
  pinMode(pin_btn_left, INPUT_PULLUP); // intialize button pin mode to pull-up resistor
  pinMode(pin_btn_right, INPUT_PULLUP); // intialize button pin mode to pull-up resistor
}

/// @brief minifying function to check if the right button is CURRENTLY being pushed, 
/// strictly in the instantaneous moment
bool isButtonActive() { return (digitalRead(pin_btn_right) == LOW); }

/// @brief The outward-facing function of this lib.  
/// Robustly checks the button to provide means of
/// distinguishing debounce events from genuine human input
bool isReceivingPureInput() {
  if (isButtonActive()) {     // is button peing pushed?
    if (millis() - msSinceLastPress > DEBOUNCE_DELAY_MS) {
        // if so, has it been a *reasonable* amount of time?
        msSinceLastPress = millis();    // reset time of press
        awaiting_first_press = true;    // future presses won't set 
        return true;
      }
    } 
  return false;
}