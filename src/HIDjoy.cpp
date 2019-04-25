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

#include "HIDjoy.h"

#if defined(_USING_HID)

#include "FFBdesc.h"

static const byte _hidReportDescriptorJoystick[] PROGMEM = {
// 32 x buttons, 1 x 8-way hat, 8 16-bit axes and FFB
  HID_JOY_DESC(),
  HID_FFB_DESC(),
  0xC0                  /*  End Collection (Application) */
};

//constructor that does nothing
Joystick_::Joystick_(void){
	clearState();
	//append the discriptor to a thing idk
	static HIDSubDescriptor node(_hidReportDescriptorJoystick, sizeof(_hidReportDescriptorJoystick));
  HID.AppendDescriptor(&node);
}

//reset joy state to defaults
void Joystick_::clearState(void){
	state.x.axis = AXIS_CENTER;
	state.y.axis = AXIS_CENTER;
  state.z.axis = AXIS_MIN;
	state.xRot.axis = AXIS_CENTER;
	state.yRot.axis = AXIS_CENTER;
	state.zRot.axis = AXIS_CENTER;
	state.slider.axis = AXIS_MIN;
	state.dial.axis = AXIS_MIN;
	state.hats.switch1 = HAT_CENTER;
	state.hats.switch2 = HAT_CENTER;
	state.buttons.bitfield = 0;
}
//format and send state report. The report data format must match the one defined in the descriptor exactly
//or it either won't work, or the pc will make a mess of unpacking the data

void Joystick_::sendState(void){
	byte data[]{
    //axes
		state.x.dataHi,
		state.x.dataLo,
		state.y.dataHi,
		state.y.dataLo,
		state.z.dataHi,
		state.z.dataLo,
		state.xRot.dataHi,
		state.xRot.dataLo,
		state.yRot.dataHi,
		state.yRot.dataLo,
		state.zRot.dataHi,
		state.zRot.dataLo,
		state.slider.dataHi,
		state.slider.dataLo,
		state.dial.dataHi,
		state.dial.dataLo,
    //buttons
    state.buttons.data1,
    state.buttons.data2,
    state.buttons.data3,
    state.buttons.data4,
    //hats
    state.hats.data
	};
	//HID_SendReport(Report number, array of values in same order as HID descriptor, length)
	HID.SendReport(1,data,sizeof(data));

}
Joystick_ Joystick;

#endif
