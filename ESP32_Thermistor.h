#ifndef ESP32_Thermistor_h
#define ESP32_Thermistor_h

#include <Arduino.h>

class ESP32_Thermistor {
  public:
    ESP32_Thermistor();

    void    begin(uint8_t pin, // required
                     float ThermR, // required
                     float SeriesR, // required
                     float beta = 3950 // optional
                    );
    float   read(); // Returns temperature in °C
    float   readADCMaxReference(); // Get the max ADC reference value
    float   readRawVoltage(); // Get the raw voltage of the pin
    float   readADCValue(); // Get the average ADC value of the pin
    float   readResistance(); // Get the average ADC value of the pin
    void    setOffset(float offset); // A multiplier used to fine tune the results
    void    toDecimal(int decimal); // Return temperature up to this decimal point. Defaults to nearest 0.10;
    void    setReversed(bool reversed); // Thermistor hardware setup. Default = 0 = Thermistor to 3.3v, Resistor to GND. Reversed = Set to 1 = Resistor to 3.3v, Thermistor to GND
    void    setAdcRefPin(uint8_t pin); // Sets _adcRefMax if hardware is set up for it. Hardware setup: Wire a sacrificial ADC pin to the ESP32 3.3v output 

  private:
    uint8_t  _pin;
    float    _ThermR;
    float    _SeriesR;
    float    _beta = 3095;
    float    _offset = 1.0;
    int      _decimal = 1;
    float    _adcRefMax = 4095.0;
    float    _adcValue = 0.0;
    float    _resistance = 0.0;
    bool     _reversed = false;
    uint8_t  _adcRefPin = 255;
    float    _steinhart;

    float   _readAnalogAvg(uint8_t pin);
    void    _updateAdcRef();
};

// https://downloads.arduino.cc/libraries/logs/github.com/nedesico/ESP32_Thermistor/
#endif