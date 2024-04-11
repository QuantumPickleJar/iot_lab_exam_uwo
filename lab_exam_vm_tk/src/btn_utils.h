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

/// @brief sets the PinMode of the button pins to INPUT_PULLUP
void initButtonPins();

/// @brief checks if the left button is currently being pressed, 
/// without performing any debounce checks
/// @return true or false
bool isLeftButtonActive();

/// @brief checks if the right button is currently being pressed, 
/// without performing any debounce checks
/// @return true or false
bool isRightButtonActive();

/// @brief checks whether or not the left button is pressed
bool isLeftReceivingPureInput();

/// @brief checks whether or not the right button is pressed
bool isRightReceivingPureInput();

#endif

