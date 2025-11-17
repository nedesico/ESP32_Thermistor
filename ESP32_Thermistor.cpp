#include "ESP32_Thermistor.h"
#include "HardwareSerial.h"

#define ESP32T_SAMPLES 10
#define ESP32T_SAMPLEDELAY 1

ESP32_Thermistor::ESP32_Thermistor(uint8_t pin, float ThermR, float beta, float SeriesR) {
  _pin          = pin;
  _ThermR       = ThermR;
  _beta         = beta;
  _SeriesR      = SeriesR;
}

void ESP32_Thermistor::begin() {
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  _updateAdcRef();  // Measure reference if you have a clean ADCREF_PIN wired to ESP32 3.3v output
}

void ESP32_Thermistor::_updateAdcRef() {
  if (_adcRefPin != 255) {
    float refReading = _readAnalogAvg(_adcRefPin);
    if (refReading > 100 && refReading < 4095) {
      _adcRefMax = refReading;
    }
  }
}

float ESP32_Thermistor::_readAnalogAvg(uint8_t pin) {
  if (pin == 255) pin = _pin;
  float sum = 0;
  int count = 0;
  for (uint8_t i = 0; i < ESP32T_SAMPLES; i++) {
    sum += analogRead(pin);
    count++;
    delay(ESP32T_SAMPLEDELAY);
  }
  return sum / count;
}

float ESP32_Thermistor::readRawVoltage() {
  float adc = _readAnalogAvg(_pin);
  return adc * 3.1f / _adcRefMax;
}

float ESP32_Thermistor::read() {
  int adcValue = _readAnalogAvg(_pin);
  if (adcValue < 10 || adcValue > _adcRefMax - 10) return NAN;
  float resistance = (_SeriesR * (adcValue / (_adcRefMax - adcValue)));
  float steinhart;
  if(_reversed) {
    steinhart = (resistance/_ThermR);
  } else {
    steinhart = (_ThermR/resistance);
  }
  steinhart = log(steinhart);
  steinhart = (steinhart/_beta);
  steinhart = (steinhart+(1 / (25 + 273.15)));
  steinhart = (1 / steinhart);
  steinhart = (steinhart-273.15);
  return steinhart * _offset;
}

void ESP32_Thermistor::setOffset(float offset) {
  _offset = offset;
}

void ESP32_Thermistor::setAdcRefPin(uint8_t pin) { // New setter implementation
  _adcRefPin = pin;
}