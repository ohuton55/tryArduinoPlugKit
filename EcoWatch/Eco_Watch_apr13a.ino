#include <Modulino.h>
#include "thingProperties.h"
#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>
#include <Arduino_CloudConnectionFeedback.h>

// Create object instances
ModulinoThermo thermo;
ArduinoLEDMatrix matrix;
ModulinoPixels leds;

void setup() {
  Serial.begin(9600);
  delay(1500);

  matrix.begin();

  //Set up Arduino Cloud
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  waitForArduinoCloudConnection(matrix);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Call all necessary .begin() function
  Modulino.begin();
  thermo.begin();
  leds.begin();
}

void loop() {
  ArduinoCloud.update();

  // Assign environmental reading to variables. 'temperature' is a cloud variable.
  temperature = thermo.getTemperature();
  humidity = thermo.getHumidity();

  // Print the temperature and humidity values to Serial
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.print("°C");

  Serial.print ("          ");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println("%");

  // Convert numbers and assign them to strings
  String temperatureText = String(temperature, 0) + "°C";
  String humidityText    = String(humidity, 0) + "%";
 
  // Settings for the LED Matrix
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(100);
 
  // Add the text
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println("    " + temperatureText + ":" + humidityText);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

  // If 'lightswitch' is true, turn on the LEDs with a brightness of 5
  if (lightSwitch == true) {

    //Declare color variable
    ModulinoColor myColor = WHITE;
    // If the temperature is equal to or smaller than the minimum temperature turn all LEDs to blue
    if (temperature <= minTemp) {
      myColor = BLUE;
    // If the temperature is below the optimal but not at the minimum temperature, turn all LEDs to VIOLET
    } else if (temperature > minTemp && temperature < maxTemp) {
      myColor = VIOLET;
    // If the temperature is within the optimal range, turn all LEDs to green
    } else if (temperature >= maxTemp) {
      myColor = RED;
    }
 
    for (int i = 0; i < 8; i++) {
      if (i % 2 == 0) {
        leds.set(i, myColor, 15);
      }
    }
    leds.show();
    // If 'lightswitch' is false, set the brightness to 0 (turn them off)
    } else {
    for (int i = 0; i < 8; i++) {
      leds.set(i, WHITE, 0);
    }
    leds.show();
  }
}

//The functions below are called callback functions but we don't need them for this project.

void onMaxTempChange()  {}

void onMinTempChange()  {}

void onLightSwitchChange()  {}
