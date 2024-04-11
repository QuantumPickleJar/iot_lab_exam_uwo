/**
 * @date: 2024-03-22
 * @author: Vincent Morrill
 * @brief:
 * Series of temperature reading functions centered around the TMP36 (3 pin version).
 * Includes some mock wind chill computations for testing
 * @bug <<< Large Wind Chills >>>
 * I think my system for storing a desired unit ended up backfiring, because the wind chill
 * coming out doesn't hold up mathematically.  Ex: At ~76F, with a mph wind it calculates 21.3F.
 * Digging deeper uncovers the nature of wind chill calcualtion, supposedly the  formula is only accurate
 * below 40F and below and wind speeds above 3 mph. 
 * 
 * For temperatures above 50F, the wind chill temperature is usually higher than the actual
 * air temperature, due to sun and warm air pushed 
 * 
 * @remark: 
 * First time seeing the enum name operator (::)
 * had a lot of back-and-forth feelings about my library files as I proceeded
 * in the end, I'm glad I stuck it out and refactored out the button functions
 * 
 * There were also a lot of comments since I'm lacking in exposure to C++. 
 * I've done my best to remove non-essential ones and leave enough to pick back
 * up again in the future
*/
#include "cmath"
#include "tmp36_utils.h" 
#include "Particle.h"

// default to Fahrenheit, because Americans 
TemperatureUnit enforcedUnit = Fahrenheit; // for singular conversion jobs

const int pin_temp = A2;
const int MOCK_WIND_MAX_SPEED = 20;     // desired ceiling of faked wind speed
double stored_temp;

void initTemperaturePins()
{
  pinMode(pin_temp, AN_INPUT); // since temperature is analog
}

/// @brief sets the current temperature unit
/// @param unit The desired temperature unit
void setDefaultTempUnit(TemperatureUnit unit) { enforcedUnit = unit; }

/// @brief returns the last stored temperature 
/// @sa checkTemp
double getTemperature() { return stored_temp; }

/// @brief QoL method that provides temperature in a specified unit
/// @return the temperature, converted appropriately
/// @sa TemperatureUnit
double getTemperatureInUnit(TemperatureUnit desiredUnit) {
  double convertedTemp;
  convertedTemp = convertTemperature(stored_temp, enforcedUnit, desiredUnit);
  return convertedTemp;
}


/// @brief converts a temperature from one unit to another
/// @param temperature The temperature reading in Celsius
/// @param fromUnit Use when known: unit of the incoming temperature 
/// @returns temperature in the matching unit as `toUnit`
double convertTemperature(double temp, TemperatureUnit fromUnit, TemperatureUnit toUnit) {
  double convertedTemp, tempInCelsius;

  // conversion will require two units, so we enforce starting our conversion from C
  switch(fromUnit) {
    case Fahrenheit:
      tempInCelsius = (temp - 32) * 5.0/9.0;      // Fahrenheit to Celsius
      break;
    case Celsius:
      tempInCelsius = temp;
      break;                       // no conversion needed, fallthrough to default
    case Kelvin:
      tempInCelsius = temp - 273.15;              // Kelvin to Celsius
      break;
    default: // treat as Celsius otherwise
      tempInCelsius = temp;
      break;
  }

  switch(toUnit) {
    case Fahrenheit:
      convertedTemp = (tempInCelsius * 9.0/5.0) + 32;    // Celsius to Fahrenheit
      break;
    case Celsius:
      convertedTemp = tempInCelsius;                    // Celsius to... Celsius
      break;
    case Kelvin: 
      convertedTemp = tempInCelsius + 273.15;          // Celsius to Kelvin
      break;
    default:              // per header, will default to Celsius
      convertedTemp = tempInCelsius;
      break;
  }

  return convertedTemp;
}


/// @brief Given the wind speed, uses the formula
/// @f$ T_{wc} = 35.74 + 0.6215T_{a} - 35.75v^{0.16} + 0.4275T_{a}*v @f$
/// where @f$T_{wc} @f$ is the air temperature in *Fahrenheit*, and @f$v@f$ 
/// is the wind speed in miles per hour
/// (!) does NOT update stored_temp, and assumes that the incoming unit is C
/// @return the wind chill 
double calculateWindChill(double windSpeed, double airTemp)
{
    // to avoid confusion with enforcedUnit, we can assure that we get F by 
    // performing a new reading here and convert directly to F
    double tempFahrenheit, result;   
    enforcedUnit = Fahrenheit;
    tempFahrenheit = getTemperatureInUnit(enforcedUnit);

    // to reduce the long and ugly lines, I split the formula into thirds:
    double alpha, beta, gamma; 
    // Let T = air temperature, and V = wind speed
    // 1: (35.74 + 0.6215 * T) ^ 0.16 
    // 2: (- 35.75 * v) ^ 0.16
    // 3: 0.4275 * T * V
    
    alpha = 35.74 + 0.6215 * tempFahrenheit;
    beta = 35.75 * pow(windSpeed, 0.16);
    gamma = 0.4275 * tempFahrenheit * pow(windSpeed, 0.16);  

    // now we just combine them and we're done!
    result = alpha - beta + gamma;
    return result;
}


/// @brief generates a new windspeed between 0 and the MOCK_MAX_WIND_SPEED
/// @param speedLimit highest allowed number generated, defaults to 20
/// @return the wind speed
int mockRandomWindSpeed(int speedLimit) {
  int mockSpeed = random((speedLimit + 1));       // random is 0-based
  Log.info("Mocked winds of %2d mph", mockSpeed);
  return mockSpeed;
}

/// @brief checks, then updates the stored temperature 
void checkTemp() {
  double voltage = getTMP36VOutMv();
  
  double tempCelsius;               // we'll worry about the unit at the end
  // per datasheet, 750mV at 25C, with a rate-of-change of 10mV/1C.  We
  tempCelsius = (voltage - 500.0)/ 10.0; // note we're using the mV at 0C (500), not 750
 
  if (enforcedUnit == Celsius) {
    stored_temp = tempCelsius; 
  }
  else {
    // In one line, we can now go from raw voltage to the desired unit!
    stored_temp = convertTemperature(tempCelsius, Celsius, enforcedUnit);
  }
}

/// @brief reads the ADC voltage of the TMP36 to get its mV output
/// @return the converted voltage from analogRead, in mV
double getTMP36VOutMv() {
  // first, we'll get our unsigned int between 0 and 4095
  int reading = analogRead(pin_temp);
  
  double milliVolts = (double)reading * ((double)P2_VOLTAGE_IN_MV / (double)MAX_ANALOG_VAL);
  // since we're going between units, best that we round fairly aggressively
  milliVolts = round(milliVolts * 10) / 10; // mathematically truncates beyond tenths place
  return milliVolts;
}