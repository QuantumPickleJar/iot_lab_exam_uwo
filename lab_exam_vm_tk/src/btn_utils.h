/**
 * @file btn_utils.h
 * @version 0.2
 * @author Vincent Morrill (vince.codefactory@outlook.com)
 * @date 2024-04-02
 * @brief bare-bones library to provide push button detection in a circuit
 * @remarks none
 * @bug holding the button will be detected as many many presses in quick succession
 */
#ifndef BTN_UTILS_H
#define BTN_UTILS_H

extern const int pin_btn_left;   // pin connected to first button (D2)
extern const int pin_btn_right;   // pin connected to button furthest from board (D3)
extern bool awaiting_first_press; // whether or not btn has been pressed since RESET

/// @brief sets the PinMode of the button pins to INPUT_PULLUP
void initButtonPin();

/// @brief checks if the right button is currently being pressed, 
/// without performing any debounce checks
/// @return true or false
bool isButtonActive();

/// @brief checks whether or not the right button is pressed
bool isReceivingPureInput();

#endif

