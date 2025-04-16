#include <Modulino.h>
#include "thingProperties.h"
#include <Arduino_LED_Matrix.h>
#include <Arduino_CloudConnectionFeedback.h>

// Create object instances
ArduinoLEDMatrix matrix;
ModulinoBuzzer buzzer;
ModulinoButtons buttons;
ModulinoKnob knob;
ModulinoPixels pixels;
ModulinoDistance distance;

int lastKnobVal = 0;
int currentKnobVal = 0;

int minFrequency = 0;
int maxFrequency = 0;

int ledPosition = 0;
int distPosition = 0;

void setup() {
  Serial.begin(9600);
  initProperties();

  matrix.begin();

  //Set up Arduino Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  waitForArduinoCloudConnection(matrix);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Call all necessary .begin() function
  Modulino.begin();
  buzzer.begin();
  buttons.begin();
  knob.begin();
  pixels.begin();
  distance.begin();

  knob.set(0);
  buttons.setLeds(true, true, true);
}

// Play the sound
void playSound(int newFrequency){
  if (distance.available()) {
    distPosition = distance.get();
    Serial.print("distance: ");
    Serial.println(distPosition);
    duration = distPosition;
  }
  buzzer.tone(newFrequency, duration);
}
int readKnob() {
  lastKnobVal = currentKnobVal;
  currentKnobVal = knob.get();
  if (currentKnobVal > lastKnobVal) {
    return 1;
  } else if (currentKnobVal < lastKnobVal) {
    return -1;
  } else {
    return 0;
  }
}
void showPixel(bool isHighTone) {
  if (isHighTone) {
    pixels.set(ledPosition, RED, 10);
  } else {
    pixels.set(ledPosition, BLUE, 10);
  }
}
void loop() {
  ArduinoCloud.update();
  // Declare button variables
  bool button_a,button_b,button_c = false;

  int knobDirection = readKnob();

  // Check for button presses
  if (buttons.update()) {
    // Change frequency based on button press
    if (buttons.isPressed(0)) {
      playSound(frequency);
    }
    if (buttons.isPressed(1)) {
      playSound(frequency * 1.25);
    }
    if (buttons.isPressed(2)) {
      playSound(frequency * 1.5);
    }
  }

  if (knob.isPressed()) {
    knobCnt++;
  }
  isHighTone = knobCnt % 2 == 0;
  if (isHighTone) {
    minFrequency = 524;
    maxFrequency = 785;
  } else {
    minFrequency = 262;
    maxFrequency = 523;
  }

  if (knobDirection == 1) {
    // we multiply for 1.12 because it is the average rate between tone in a musical scale
    frequency = frequency * 1.12 ;
  } else if (knobDirection == -1) {
    frequency = frequency / 1.12;
  }

  if (frequency > maxFrequency) {
    frequency = maxFrequency;
  }else if (frequency < minFrequency) {
    frequency = minFrequency;
  }
  
  ledPosition = ledPosition + knobDirection;

  pixels.clear();

  showPixel(isHighTone);
  
  pixels.show();
  
  if (ledPosition < 0) {
    ledPosition = 0;
  } else if (ledPosition >= 7) {
    ledPosition = 7;
  }
}

//The functions below are called callback functions but we don't need them for this project.

void onDurationChange() {}

void onFrequencyChange() {}

/*
  Since MaxFrequency is READ_WRITE variable, onMaxFrequencyChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onMaxFrequencyChange()  {
  // Add your code here to act upon MaxFrequency change
}
/*
  Since MinFrequency is READ_WRITE variable, onMinFrequencyChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onMinFrequencyChange()  {
  // Add your code here to act upon MinFrequency change
}
/*
  Since Frequency2 is READ_WRITE variable, onFrequency2Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onFrequency2Change()  {
  // Add your code here to act upon Frequency2 change
}
/*
  Since KnobCnt is READ_WRITE variable, onKnobCntChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onKnobCntChange()  {
  // Add your code here to act upon KnobCnt change
}
/*
  Since IsHighTone is READ_WRITE variable, onIsHighToneChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onIsHighToneChange()  {
  // Add your code here to act upon IsHighTone change
}