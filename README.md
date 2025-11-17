#include <ESP32_Thermistor.h>

/&ast;&ast;
 &ast; The ADCREF_PIN pin is a sacrificial pin to dynamically read the maximum ADC value for the ADC
 &ast; Wire this pin directly to the 3.3v output of the EPS32
 &ast; !!!! DO NOT CONNECT THE ADCREF_PIN PIN TO 5V / VCC PIN OR USE THIS PIN FOR ANYTHING ELSE !!!!
 &ast; !!!! YOU WILL BRICK YOUR DEVICE OR GET INACCURATE RESULTS IF YOU DO !!!!
&ast;/
#define ADCREF_PIN 0 // If the sacrificial ADC pin on the ESP32 is not set, ADC reference will default to 4095

#define TEMP1 2 // Thermistor to ESP32 pin 2
#define TEMP2 3 // Thermistor to ESP32 pin 3

/&ast;&ast;
 &ast; pin (required),
 &ast; thermistor resistance at 25C (required),
 &ast; series resistor (required),
 &ast; B-value (optional, defaults to 3950)
 &ast;/
ESP32_Thermistor temp1(TEMP1, 100000, 100000);
ESP32_Thermistor temp2(TEMP2, 10000, 10000, 3534);

void setup() {
  Serial.begin(115200);

  /&ast;&ast;
   &ast; Set the sacraficial ADC pin
  &ast;/
  temp1.setAdcRefPin(ADCREF_PIN);
  /&ast;&ast;
   &ast; setOffset()
   &ast; Additional optional calibration. Multiplies the ADC value with this amount.
   &ast; Example: 1.1 = +10% increased offset. 0.9 = -10% decreased offset
   &ast;/
  // temp1.setOffset(1.1);
  temp2.setAdcRefPin(ADCREF_PIN);
  /&ast;&ast;
   &ast; Thermistor hardware setup.
   &ast; Default: false = Thermistor to 3.3v, Series resistor to GND.
   &ast; Reversed: Set to true = Series resistor to 3.3v, Thermistor to GND
   &ast;/
  // temp1.setReversed(true);

  temp1.begin();
  temp2.begin();
}

void loop() {
  float t1 = temp1.read();
  Serial.printf("Thermistor 1: %.2f °C\n", t1);
  float t2 = temp2.read();
  Serial.printf("Thermistor 2: %.2f °C\n", t2);
  delay(2000);
}