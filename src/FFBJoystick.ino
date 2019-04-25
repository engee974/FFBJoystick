/* MIT License
**
** Copyright (c) 2019, Nicolas Grondin
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/

//note that for the due the hid controller only works through the native port
#include "HIDjoy.h"
#include "FFBenum.h"
#include "FFBdesc.h"


// Défine buttons matrix 4x8 : 32 buttons
const byte buttonRow[] = { 23, 24, 25, 26 };
const byte buttonCol[] = { 27, 28, 29, 30, 31, 32, 33, 34 };
const short buttonRowCount = sizeof(buttonRow) / sizeof(buttonRow[0]);
const short buttonColCount = sizeof(buttonCol) / sizeof(buttonCol[0]);

// Define 8-way hats matrix : 2 x 2x4
const byte hatRow[][2] = { {35, 36}, {37, 38} };
const byte hatCol[] = { 39, 40, 41, 42 };
const short hatRowCount = sizeof(hatRow[0]) / sizeof(hatRow[0][0]);
const short hatColCount = sizeof(hatCol) / sizeof(hatCol[0]);

// Define encoders : 4 x encoders
const byte encoder[][2] = { {44, 45}, {46, 47}, {48, 49}, {50, 51} };

/**
 * [setup description]
 */
void setup() {
  // change the resolution to 12 bits
  analogReadResolution(12);
  //REG_ADC_MR = 0x10380180;
  ADC -> ADC_CHER = 0x3CFF; // Enable ADC channels 0 to 7
  ADC -> ADC_MR |= 0x80;    // Set Freerun mode
  ADC -> ADC_CR = 2;        // Start converter

  // Set Pins for buttons matrix
  for (short col = 0; col < buttonColCount; col++) {
    pinMode(buttonCol[col], INPUT);
  }
  for (short row = 0; row < buttonRowCount; row++) {
    pinMode(buttonRow[row], INPUT_PULLUP);
  }

  // Set Pins for hats matrix
  for (short col = 0; col < hatColCount; col++) {
    pinMode(hatCol[col], INPUT);
  }
  for (short hat = 0; hat < 2; hat++) {
    for (short row = 0; row < hatRowCount; row++) {
      pinMode(hatRow[hat][row], INPUT_PULLUP);
    }
  }

  //zero out the joystick state
  Joystick.clearState();
  ForceFeedback.state.actuatorEnabled = 0;
}

/**
 * [readButtons description]
 * @return [description]
 */
bool readButtons() {
  uint32_t curButtons = Joystick.state.buttons.bitfield;

  Joystick.state.buttons.bitfield = 0;

  // iterate the rows
  for (short row = 0; row < buttonRowCount; row++) {
    // row: set to output to low
    byte curRow = buttonRow[row];
    pinMode(curRow, OUTPUT);
    digitalWrite(curRow, LOW);

    // col: interate through the cols
    for (short col = 0; col < buttonColCount; col++) {
      byte curCol = buttonCol[col];
      pinMode(curCol, INPUT_PULLUP);
      if (!digitalRead(curCol))
        Joystick.state.buttons.bitfield |= (1 << ((row * 8) + col));
      //else
      //  Joystick.state.buttons.bitfield &= ~(1 << ((row * 8) + col));
      pinMode(curCol, INPUT);
    }
    // disable the row
    pinMode(curRow, INPUT);
  }

  return (curButtons != Joystick.state.buttons.bitfield);
}

/**
 * [readHats description]
 * @param  nbHats [description]
 * @return        [description]
 */
bool readHats(short nbHats) {
  byte curHatsData = Joystick.state.hats.data;

  Joystick.state.hats.data = 0x88;

  // iterate hats
  for (short hat = 0; hat < nbHats; hat++) {
    // iterate the rows
    for (short row = 0; row < hatRowCount; row++) {
      // row: set to output to low
      byte curRow = hatRow[hat][row];
      pinMode(curRow, OUTPUT);
      digitalWrite(curRow, LOW);

      // col: interate through the cols
      for (short col = 0; col < hatColCount; col++) {
        byte curCol = hatCol[col];
        pinMode(curCol, INPUT_PULLUP);
        if (!digitalRead(curCol))
          Joystick.state.hats.data = ( (col + (row * 4)) << (hat * 4) );

        pinMode(curCol, INPUT);
      }
      // disable the row
      pinMode(curRow, INPUT);
    }
  }

  return (curHatsData != Joystick.state.hats.data);
}

/**
 * [readAxis description]
 * @return [description]
 */
bool readAxis() {
  unsigned short lastAxis;
  unsigned short newAxis[8];
  bool axisChanged = false;

  noInterrupts();
  // Read Axis
  for(short a = 0; a < 8; a++) {
    while ((ADC->ADC_ISR & (1<<(7-a))) != (unsigned int) (1<<(7-a)));
    newAxis[a] = ADC->ADC_CDR[7-a];
  }
  interrupts();

  // Set X
  lastAxis = Joystick.state.x.axis;
  Joystick.state.x.axis = map(newAxis[0], 0, 4095, AXIS_MIN, AXIS_MAX);
  axisChanged |= (lastAxis != newAxis[0]);

  // Set Y
  lastAxis = Joystick.state.y.axis;
  Joystick.state.y.axis = map(newAxis[1], 0, 4095, AXIS_MIN, AXIS_MAX);
  axisChanged |= (lastAxis != newAxis[1]);

  // Set Z
  lastAxis = Joystick.state.z.axis;
  Joystick.state.z.axis = map(newAxis[2], 0, 4095, AXIS_MIN, AXIS_MAX);
  axisChanged |= (lastAxis != newAxis[2]);

  // Set Rx
  lastAxis = Joystick.state.xRot.axis;
  Joystick.state.xRot.axis = map(newAxis[3], 0, 4095, AXIS_MIN, AXIS_MAX);
  axisChanged |= (lastAxis != newAxis[3]);

  // Set Ry
  lastAxis = Joystick.state.yRot.axis;
  Joystick.state.yRot.axis = map(newAxis[4], 0, 4095, AXIS_MIN, AXIS_MAX);
  axisChanged |= (lastAxis != newAxis[4]);

  // Read Rz
  lastAxis = Joystick.state.zRot.axis;
  Joystick.state.zRot.axis = map(newAxis[5], 0, 4095, AXIS_MIN, AXIS_MAX);
  axisChanged |= (lastAxis != newAxis[5]);

  // Read Slider
  lastAxis = Joystick.state.slider.axis;
  Joystick.state.slider.axis = map(newAxis[6], 0, 4095, AXIS_MIN, AXIS_MAX);
  axisChanged |= (lastAxis != newAxis[6]);

  // Read Dial
  lastAxis = Joystick.state.dial.axis;
  Joystick.state.dial.axis = map(newAxis[7], 0, 4095, AXIS_MIN, AXIS_MAX);
  axisChanged |= (lastAxis != newAxis[7]);

  return axisChanged;
}

/**
 * [loop description]
 */
void loop() {
  static bool joyStateChanged = false;

  // Update Joystick state
  static unsigned long lastJoySateMillis = 0;
  if (millis() - lastJoySateMillis >= 200) {
    joyStateChanged |= readButtons();
    joyStateChanged |= readAxis();
    joyStateChanged |= readHats(1);
    lastJoySateMillis = millis();
  }

  // Send Joystick state report
  static unsigned long lastSendStateMillis = 0;
  if (joyStateChanged || millis() - lastSendStateMillis >= 500) {
    Joystick.sendState();
    joyStateChanged = false;;
    lastSendStateMillis = millis();
  }

  // Read FFB reports
  if (ForceFeedback.recvData()) {
    switch (ForceFeedback.getReportID()) {
      case FFB_EFFECT_CONSTANT:
        // TODO
        break;
      case FFB_EFFECT_RAMP:
        // TODO
        break;
      case FFB_EFFECT_SQUARE:
        // TODO
        break;
      case FFB_EFFECT_SINE:
        // TODO
        break;
      case FFB_EFFECT_TRIANGLE:
        // TODO
        break;
      case FFB_EFFECT_SAWTOOTHUP:
        // TODO
        break;
      case FFB_EFFECT_SAWTOOTHDOWN:
        // TODO
        break;
      case FFB_EFFECT_SPRING:
        // TODO
        break;
      case FFB_EFFECT_DAMPER:
        // TODO
        break;
      case FFB_EFFECT_INERTIA:
        // TODO
        break;
      case FFB_EFFECT_FRICTION:
        // TODO
        break;
      case FFB_EFFECT_CUSTOM:
        // TODO
        break;
      default:
        // TODO
        break;
    }
  }

  // Send FFB state
  static unsigned long lastFfbSendStateMillis = 0;
  if (millis() - lastFfbSendStateMillis >= 500) {
    ForceFeedback.sendState();
    lastFfbSendStateMillis = millis();
  }

}
