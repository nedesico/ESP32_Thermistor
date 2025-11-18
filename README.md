<h2>Basic Usage:</h2>

```
#include <ESP32_Thermistor.h>

/**
 * The ADCREF_PIN is a sacrificial pin to dynamically read the maximum ADC value for the ESP32 ADC channel.
 * Wire this pin directly to the 3.3v output of the EPS32.
 * !!!! DO NOT CONNECT THE ADCREF_PIN TO THE 5V / VCC PIN OR USE THIS PIN FOR ANYTHING ELSE !!!!
 * !!!! YOU WILL BRICK YOUR DEVICE OR GET INACCURATE RESULTS IF YOU DO !!!!
*/
#define ADCREF_PIN 0 // If the sacrificial ADC pin on the ESP32 is not set, ADC reference will default to 4095

#define TEMP1 2 // Thermistor to ESP32 pin 2
#define TEMP2 3 // Thermistor to ESP32 pin 3

/**
 * pin (required),
 * thermistor resistance at 25C (required),
 * series resistor (required),
 * B-value (optional, defaults to 3950)
 */
ESP32_Thermistor temp1(TEMP1, 100000, 100000);
ESP32_Thermistor temp2(TEMP2, 10000, 10000, 3534);

void setup() {
  Serial.begin(115200);

  /**
   * Set the sacraficial ADC pin
  */
  temp1.setAdcRefPin(ADCREF_PIN);
  temp2.setAdcRefPin(ADCREF_PIN);

  /**
   * setOffset()
   * Additional optional calibration. Multiplies the ADC value with this amount.
   * Example: 1.1 = +10% increased offset. 0.9 = -10% decreased offset
   */
  temp2.setOffset(1.1);

  /**
   * Thermistor hardware setup.
   * Default: false = Thermistor to 3.3v, Series resistor to GND.
   * Reversed: Set to true = Series resistor to 3.3v, Thermistor to GND
   */
  temp2.setReversed(true);

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
```


<h2>Real world example:</h2>
<img src="https://raw.githubusercontent.com/nedesico/ESP32_Thermistor/refs/heads/main/rwe.jpg">

