#ifndef ESP32_Thermistor_h
#define ESP32_Thermistor_h

#include <Arduino.h>

class ESP32_Thermistor {
  public:
    ESP32_Thermistor(uint8_t pin, // required
                     float ThermR, // required
                     float SeriesR, // required
                     float beta = 3950 // optional
                    );

    void    begin();
    float   read(); // Returns temperature in °C
    float   readRawVoltage(); // Maybe someone will find this useful
    void    setOffset(float offset); // A multiplier used to fine tune the results
    void    setReversed(bool reversed); // Thermistor hardware setup. Default = 0 = Thermistor to 3.3v, Resistor to GND. Reversed = Set to 1 = Resistor to 3.3v, Thermistor to GND
    void    setAdcRefPin(uint8_t pin); // Sets _adcRefMax if hardware is set up for it. Hardware setup: Wire a sacrificial ADC pin to the ESP32 3.3v output 

  private:
    uint8_t  _pin;
    float    _ThermR;
    float    _SeriesR;
    float    _beta = 3095;
    float    _offset = 1.0;
    float    _adcRefMax = 4095.0;
    bool     _reversed = false;
    uint8_t  _adcRefPin = 255;

    float   _readAnalogAvg(uint8_t pin);
    void    _updateAdcRef();
};

#endif