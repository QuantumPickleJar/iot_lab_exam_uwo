/****
 * @author Vincent Morrill (vince.codefactory@outlook.com)
 * @date 2024-03-22
 * @bug << Rounding >>
 * I didn't program any actual rounding in, so don't be surprised if you tried
 * "checking my math"!  It'll be off by a margin not exceeding 5 degrees per unit
 * 
 * @remark I realize it seems weird to write so much to do this for us here, 
 * but I figure if we end up using the temperature sensor again, I can just 
 * drag and drop this in like a library! 
 * 
 * My robust additions ended up making the conversion from Cel to Far functions 
 * obsoselete, ah well.  It took a lot of resillience to solidify my understanding, 
 * but I'm comfortable enough with `extern` and include guards to explain to a peer
****/
#ifndef TEMP_UTILS_H
#define TEMP_UTILS_H

extern const int pin_temp;            // pin (A2) reading VOUT pin of TMP36
extern const int MOCK_WIND_MAX_SPEED; // Since we don't have an anemometer
extern const int MAX_ANALOG_VAL;      // this should be used in any unit conversions

extern const int P2_VOLTAGE_IN_MV; // the voltage supplied to the Photon 2
extern double stored_temp;     // stores the calcualted reading from pin_temp

enum TemperatureUnit                  // desired units to use in a singular call
{
  Fahrenheit,
  Celsius,
  Kelvin
};

extern TemperatureUnit enforcedUnit;  // in case only a single line is available for displaying

void initTemperaturePins();

/*  [         UNIT-RELATED FUNCTIONS          ] */

/// @brief Sets the current temperature unit
/// @param unit The desired temperature unit
void setDefaultTempUnit(TemperatureUnit unit);

/// @brief Converts a temperature from one unit to another
/// @param temperature The numerical measurement for the temperature
/// @param fromUnit unit of incoming temperature
/// @param toUnit desired unit to adjust calculations for  
/// @returns The temperature reading in the current unit
double convertTemperature(
  double temperature, TemperatureUnit fromUnit, TemperatureUnit toUnit); 

/// @brief converts a celsius measurement to fahrenheit a one 
/// @returns the equivalent supplied temperature, in Farenheit
double convertCelToFar(double temp_celsius);

/// @brief converts a fahrenheit measurement to a celsius one 
/// @returns the equivalent supplied temperature, in Celsius 
double convertFarToCel(double temp_farenheit);

/*  [         WIND FUNCTIONS          ] */

/// @brief Calculates the wind chill using the formula
/// @f$ T_{wc} = 35.74 + 0.6215T_{a} - 35.75v^{0.16} + 0.4275T_{a}V @f$
/// where @f$T_{wc}@f$ 
/// @param windSpeed how fast the wind is blowing, in mph
/// @param airTemp (Optional) temperature, should the calculation be desired
/// @return the calculated wind chill
double calculateWindChill(double windSpeed, double airTemp = stored_temp);

int mockRandomWindSpeed(int speedLimit = MOCK_WIND_MAX_SPEED);

/*  [         I/O RELATED FUNCTIONS          ] */
/// @brief Reads pin_temp to get an analog reading from the TMP36
/// @returns call to analogRead() 
double getTMP36VOutMv();

/// @brief returns the temperature in the desired unit
double getTemperature();

/// @brief returns the temperature in the supplied unit
double getTemperatureInUnit(TemperatureUnit unit);

/// @brief obtains a new reading on the TMP36, in a loop()-friendly format 
void checkTemp();
#endif