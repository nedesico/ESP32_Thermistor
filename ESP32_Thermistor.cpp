#include "ESP32_Thermistor.h"
#include "HardwareSerial.h"

#define ESP32T_SAMPLES 10
#define ESP32T_SAMPLEDELAY 1

ESP32_Thermistor::ESP32_Thermistor(uint8_t pin, float ThermR, float SeriesR, float beta) {
  _pin          = pin;
  _ThermR       = ThermR;
  _SeriesR      = SeriesR;
  _beta         = beta;
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
  float sum;
  int count;
  for (uint8_t i = 0; i < ESP32T_SAMPLES; i++) {
    sum += analogRead(pin);
    count++;
    delay(ESP32T_SAMPLEDELAY);
  }
  return sum / count;
}

float ESP32_Thermistor::readADCMaxReference() {
  return _adcRefMax;
}

float ESP32_Thermistor::readRawVoltage() {
  return _adcValue * 3.1f / _adcRefMax;
}

float ESP32_Thermistor::readADCValue() {
  return _adcValue;
}

float ESP32_Thermistor::readResistance() {
  return _resistance;
}

float ESP32_Thermistor::read() {
  _adcValue = _readAnalogAvg(_pin);
  if (_adcValue < 10 || _adcValue > _adcRefMax - 10) return NAN;
  _resistance = (_SeriesR * (_adcValue / (_adcRefMax - _adcValue)));
  float steinhart;
  if(_reversed) {
    steinhart = (_resistance/_ThermR);
  } else {
    steinhart = (_ThermR/_resistance);
  }
  steinhart = log(steinhart);
  steinhart = (steinhart/_beta);
  steinhart = (steinhart+(1 / (25 + 273.15)));
  steinhart = (1 / steinhart);
  steinhart = (steinhart-273.15);
  _steinhart = steinhart * _offset;
  if(_decimal==1) {
    return (floor(_steinhart * 10) / 10);
  }
}

void ESP32_Thermistor::setOffset(float offset) {
  _offset = offset;
}

void ESP32_Thermistor::setAdcRefPin(uint8_t pin) { // New setter implementation
  _adcRefPin = pin;
}

void ESP32_Thermistor::toDecimal(int decimal) {
  _decimal = decimal;
}
