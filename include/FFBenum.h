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

#ifndef FFBENUM_H
#define FFBENUM_H

#include "Arduino.h"

// EFFECT types enumeration ------------------------------------------

#define FFB_EFFECT_CONSTANT     0x01
#define FFB_EFFECT_RAMP         0x02
#define FFB_EFFECT_SQUARE       0x03
#define FFB_EFFECT_SINE         0x04
#define FFB_EFFECT_TRIANGLE     0x05
#define FFB_EFFECT_SAWTOOTHDOWN 0x06
#define FFB_EFFECT_SAWTOOTHUP   0x07
#define FFB_EFFECT_SPRING       0x08
#define FFB_EFFECT_DAMPER       0x09
#define FFB_EFFECT_INERTIA      0x0A
#define FFB_EFFECT_FRICTION     0x0B
#define FFB_EFFECT_CUSTOM       0x0C


// INPUT reports data structs definition -----------------------------

typedef union FFBPIDState { // FFB: PID State Input Report
    uint16_t data;
    struct {
      byte dataHi;
      byte dataLo;
    };
    struct {
      byte deviceIsPaused         : 1; //
      byte actuatorEnabled        : 1; //
      byte safetySwitch           : 1; //
      byte actuatorOverrideSwitch : 1; //
      byte actuatorPower          : 1; //
      byte padding                : 3; //
      byte effectPlaying          : 1; //
      byte effectBlockIndex       : 7; //
    };
} __packed FfbState_t;


// OUTPUT reports data structs definition ----------------------------

typedef struct {  // FFB: Set Effect Output Report
  byte  reportId;                 // = 1
  byte  effectBlockIndex;         // 1..40
  byte  effectType;               // 1..12 (effect usages: 26,27,30,31,32,33,34,40,41,42,43,28)
  uint16_t duration;              // 0..32767 ms
  uint16_t triggerRepeatInterval; // 0..32767 ms
  uint16_t samplePeriod;          // 0..32767 ms
  byte  gain;                     // 0..255  (physical 0..10000)
  byte  triggerButton;            // button ID (0..8)
  byte  enableAxis;               // bits: 0=X, 1=Y, 2=DirectionEnable
  byte  directionX;               // angle (0=0 .. 255=360deg)
  byte  directionY;               // angle (0=0 .. 255=360deg)
} __packed ReportSetEffect_t;

typedef struct {  // FFB: Set Envelope Output Report
  byte  reportId;               // = 2
  byte  effectBlockIndex;       // 1..40
  byte  attackLevel;            //
  byte  fadeLevel;              //
  uint16_t attackTime;          // ms
  uint16_t fadeTime;            // ms
} __packed ReportSetEnvelope_t;

typedef struct {  // FFB: Set Condition Output Report
  byte reportId;                // = 3
  byte effectBlockIndex;        // 1..40
  byte parameterBlockOffset;    // bits: 0..3=parameterBlockOffset, 4..5=instance1, 6..7=instance2
  byte cpOffset;                // 0..255
  int8_t  positiveCoefficient;  // -128..127
//  int8_t  negativeCoefficient;   // -128..127
//  byte positiveSaturation;    // -128..127
//  byte negativeSaturation;    // -128..127
//  byte deadBand;              // 0..255
} __packed ReportSetCondition_t;

typedef struct {  // FFB: Set Periodic Output Report
  byte  reportId;               // = 4
  byte  effectBlockIndex;       // 1..40
  byte  magnitude;              //
  int8_t  offset;               //
  byte  phase;                  // 0..255 (=0..359, exp-2)
  uint16_t period;              // 0..32767 ms
} __packed ReportSetPeriodic_t;

typedef struct {  // FFB: Set ConstantForce Output Report
  byte  reportId;               // = 5
  byte  effectBlockIndex;       // 1..40
  int16_t  magnitude;           // -255..255
} __packed ReportSetConstForce_t;

typedef struct {  // FFB: Set RampForce Output Report
  byte  reportId;               // = 6
  byte  effectBlockIndex;       // 1..40
  int8_t   start;               //
  int8_t   end;                 //
} __packed ReportSetRampForce_t;

typedef struct {  // FFB: Set CustomForceData Output Report
  byte  reportId;               // = 7
  byte  effectBlockIndex;       // 1..40
  byte  dataOffset;             //
  int8_t   data[12];            //
} __packed ReportSetCustomForceData_t;

typedef struct {  // FFB: Set DownloadForceSample Output Report
  byte  reportId;               // = 8
  int8_t   x;                   //
  int8_t   y;                   //
} __packed ReportSetDownloadForceSample_t;

typedef struct {  // FFB: Set EffectOperation Output Report
  byte  reportId;               // = 10
  byte  effectBlockIndex;       // 1..40
  byte  operation;              // 1=Start, 2=StartSolo, 3=Stop
  byte  loopCount;              //
} __packed ReportSetEffectOperation_t;

typedef struct {  // FFB: Block Free Output Report
  byte  reportId;               // = 11
  byte  effectBlockIndex;       // 1..40
} __packed ReportBlockFree_t;

typedef struct {  // FFB: Device Control Output Report
  byte  reportId;               // = 12
  byte  control;                // 1=Enable Actuators, 2=Disable Actuators, 4=Stop All Effects, 8=Reset, 16=Pause, 32=Continue
} __packed ReportDeviceControl_t;

typedef struct {  // FFB: DeviceGain Output Report
  byte  reportId;               // = 13
  byte  gain;                   //
} __packed ReportDeviceGain_t;

typedef struct {  // FFB: Set Custom Force Output Report
  byte  reportId;               // = 14
  byte  effectBlockIndex;       // 1..40
  byte  sampleCount;            //
  uint16_t samplePeriod;        // 0..32767 ms
} __packed ReportSetCustomForce_t;

// FEATURE reports data structs definition ----------------------------

typedef struct {  // FFB: Create New Effect Feature Report
  byte  reportId;               // = 1
  byte  effectType;             // Enum (1..12): ET 26,27,30,31,32,33,34,40,41,42,43,28
  uint16_t byteCount;           // 0..511
} __packed ReportNewEffect_t;

typedef struct {  // FFB: PID Block Load Feature Report
  byte  reportId;               // = 2
  byte  effectBlockIndex;       // 1..40
  byte  loadStatus;             // 1=Success,2=Full,3=Error
  uint16_t ramPoolAvailable;    // =0 or 0xFFFF?
} __packed ReportLoadPID_t;

typedef struct {  // FFB: PID Pool Feature Report
  byte  reportId;               // = 3
  uint16_t ramPoolSize;         //
  byte  maxSimultaneousEffects; //
  byte  memoryManagement;       // Bits: 0=DeviceManagedPool, 1=SharedParameterBlocks
} __packed ReportPoolPID_t;

/**
 * [ForceFeedback_ description]
 */
class ForceFeedback_
{
  public:
    FfbState_t state;
    ForceFeedback_(void);
    void sendState(void);
    bool recvData(void);
    byte getReportID(void);

  private:
    byte buffer[64];
    uint16_t recvLen;
};
extern ForceFeedback_ ForceFeedback;

extern const uint16_t _outReportSize[];

#endif
