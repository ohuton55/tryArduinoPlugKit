#include "thingProperties.h"
#include <Arduino_LED_Matrix.h>
#include <Arduino_CloudConnectionFeedback.h>
#include <Modulino.h>
#include <Keyboard.h>
#include <Mouse.h>

ArduinoLEDMatrix matrix;
ModulinoButtons buttons;
ModulinoMovement imu;
ModulinoKnob knob;

int sensitivity = 20;

void setup() {
  Serial.begin(9600);
  delay(1500);

  matrix.begin();

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  waitForArduinoCloudConnection(matrix);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  Modulino.begin();
  buttons.begin();
  imu.begin();
  knob.begin();
  Keyboard.begin();
  Mouse.begin();

  buttons.setLeds(true, true, true);
}

void loop() {
  ArduinoCloud.update();

  bool a = false;
  bool b = false;
  bool c = false;

  if (buttons.update()) {
    
    a = buttons.isPressed(0);
    
    b = buttons.isPressed(1);
    
    c = buttons.isPressed(2);
    
      // Runs once when the button is clicked and adds to the stat menu in Dashboard
    if (a == true) {
      button1_counter = button1_counter + 1; //rename these to "button1_counter"
    }
    if (b) {
      button2_counter = button2_counter + 1;
    }
    if (c) {
      button3_counter = button3_counter + 1;
    }
  }
  // Continuously press the button/mouseclick 
  if (a) {
    Keyboard.write('b');
  } else {
    Keyboard.release('b');
  }
  if (b) { 
    Keyboard.write('e');
  } else {
    Keyboard.release('e');
  }
  if (c) {
    Mouse.press();
  } else {
    Mouse.release();
  }

  
  // Read the accelerometer and move the mouse
  imu.update();
  float x = imu.getX();
  float y = imu.getY();
  
  int position = knob.get();
  
  //sensitivity += knob.get();
  Serial.print("now position is: ");
  Serial.print(position);  
  Serial.print("now sensitivity is: ");
  int new_sensitivity = sensitivity + position;
  Serial.println(new_sensitivity);

  
  Mouse.move(-1 * (x * new_sensitivity), -1 * (y * new_sensitivity));
  
  //Update the stat menu with mouse movement
  mouseDist = mouseDist + abs(x * sensitivity) + abs(y * sensitivity);
}
