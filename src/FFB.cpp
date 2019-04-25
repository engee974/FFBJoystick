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

#include "FFBenum.h"
#include "FFBdesc.h"

#if defined(_USING_HID)

const uint16_t _outReportSize[] PROGMEM = {
  0,                                      /* 0 - not used */
  sizeof(ReportSetEffect_t),              /* 1 */
  sizeof(ReportSetEnvelope_t),            /* 2 */
  sizeof(ReportSetCondition_t),           /* 3 */
  sizeof(ReportSetPeriodic_t),            /* 4 */
  sizeof(ReportSetConstForce_t),          /* 5 */
  sizeof(ReportSetRampForce_t),           /* 6 */
  sizeof(ReportSetCustomForceData_t),     /* 7 */
  sizeof(ReportSetDownloadForceSample_t), /* 8 */
  0,                                      /* 9 - not used */
  sizeof(ReportSetEffectOperation_t),     /* 10 */
  sizeof(ReportBlockFree_t),              /* 11 */
  sizeof(ReportDeviceControl_t),          /* 12 */
  sizeof(ReportDeviceGain_t),             /* 13 */
  sizeof(ReportSetCustomForce_t),         /* 14 */
};

/**
 * [ForceFeedback_::ForceFeedback_ description]
 */
ForceFeedback_::ForceFeedback_(void){
  recvLen = 0;
  state.data = 0;
}

/**
 * [ForceFeedback_::sendState description]
 */
void ForceFeedback_::sendState(void){
  HID.SendReport(2,&state,sizeof(FfbState_t));
}

/**
 * [ForceFeedback_::recvData description]
 * @return  [description]
 */
bool ForceFeedback_::recvData(void){
  if(HID.Available()){
    recvLen = HID.RecvReport(buffer);
  }
  if (recvLen)
    return true;

  return false;
}

/**
 * [ForceFeedback_::getReportID description]
 * @return  [description]
 */
byte ForceFeedback_::getReportID(void){
  return buffer[0];
}

ForceFeedback_ ForceFeedback;

#endif
