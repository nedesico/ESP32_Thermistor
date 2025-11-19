#include <ESP32_Thermistor.h>

/**
 * The ADCREF_PIN is a sacrificial pin on the same ADC channel of the Thermistors,
 * to dynamically read the maximum available ADC value for that ESP32 ADC channel.
 * Wire this ADC pin directly to the 3.3v output of the ESP32 without a resistor.
 * If this pin is not wired up, or configured, the maximum ADC value will default to 12-bit 4095
 * which may cause inaccurate readings, in which case you may use the setOffset(float val) to help with calibration.
 *
 * !!!! DO NOT CONNECT THE ADCREF_PIN TO THE 5V / VCC PIN OR USE THIS PIN FOR ANYTHING ELSE !!!!
 * !!!! YOU WILL BRICK YOUR DEVICE OR GET INACCURATE RESULTS IF YOU DO !!!!
*/
#define ADCREF_PIN 0 // Sacraficial ADC channel pin to 3.3v

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
   * setAdcRefPin()
   * Set the sacraficial ADC pin
   * (optional, but recommended)
  */
  temp1.setAdcRefPin(ADCREF_PIN);
  temp2.setAdcRefPin(ADCREF_PIN);

  /**
   * setOffset()
   * (optional)
   * Additional optional calibration. Multiplies the temperature value with this amount.
   * Example: 1.1 = +10% increased offset. 0.9 = -10% decreased offset
   */
  temp2.setOffset(1.1);

  /**
   * Thermistor hardware setup.
   * (optional)
   * Default Setup: Set to false = Thermistor goes to 3.3v, Series resistor goes to GND.
   * Reversed Setup: Set to true = Series resistor goes to 3.3v, Thermistor goes to GND
   * Affects the direction of the temperature change. If you notice that your temperature
   * readings go up, when they should be going down, or vice versa, you can add or remove
   * this setting as required, to help fix the issue.
   */
  temp2.setReversed(true);


  /**
   * Begin the readings
   */
  temp1.begin();
  temp2.begin();
}

void loop() {
  /**
   * read()
   * (required)
   * Returns the sensor readings in °C
   * There is no option for Fahrenheit. To convert to Fahrenheit use the formula below:
   * float tF1 = ( ( temp1.read() * 9/5 ) + 32 );
   */
  float t1 = temp1.read();
  Serial.printf("Thermistor 1: %.2f °C\n", t1);
  float tF1 = ( ( t1 * 9/5 ) + 32 );
  Serial.printf("Thermistor 1: %.2f Fahrenheit\n", tF1);

  float t2 = temp2.read();
  Serial.printf("Thermistor 2: %.2f °C\n", t2);
  delay(2000);
}
