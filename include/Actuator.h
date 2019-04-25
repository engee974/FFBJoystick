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

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Arduino.h"

/**
 * [Actuator_ Class PID controller of the axe's Motion]
 * @param inPosition        ADC pin to read the Motion Position
 * @param inForce           ADC pin to read the Motion Force
 * @param outCtrlMotion     PWN pin to control the Motion
 * @param outCtrlDirection  Digital pin (OUTPUT) to set the Motion Direction
 */
class Actuator_
{
  public:
    Actuator_ (byte inPosition, byte inForce, byte outCtrlMotion, byte outCtrlDirection) :
      positionInput(inPosition), forceInput(inForce), ctrlMotionOutput(outCtrlMotion), ctrlDiretionOutput(outCtrlDirection)
    { }
    void compute(void);
    void setPID( double Kp, double Ki, double Kd);

  private:
    bool enabled;
    // ADC input
    byte positionInput;
    byte forceInput;
    // PWM output
    byte ctrlMotionOutput;
    // Digital output
    byte ctrlDiretionOutput;

    // Set Points
    double positionSetpoint;
    double forceSetpoint;
    // Variables to compute PID
    unsigned long lastComputeMillis;
    unsigned long currComputeMillis;
    double elapseTime;
    double lastPositionError;
    double cumPositionError;
    double lastForceError;
    double cumForceError;
};

#endif
