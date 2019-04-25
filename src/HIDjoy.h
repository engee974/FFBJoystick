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

#ifndef HIDJOY_H
#define HIDJOY_H

#include <HID.h>

#if !defined(_USING_HID)
#warning "Using legacy HID core (non pluggable)"
#else

typedef union button32 {
  uint32_t bitfield;
  struct {
    byte data1;
    byte data2;
    byte data3;
    byte data4;
  };
  struct {
    byte b00: 1;	byte b01: 1;	byte b02: 1;	byte b03: 1;
    byte b04: 1;	byte b05: 1;	byte b06: 1;	byte b07: 1;
    byte b08: 1;	byte b09: 1;	byte b10: 1;	byte b11: 1;
    byte b12: 1;	byte b13: 1;	byte b14: 1;	byte b15: 1;
    byte b16: 1;	byte b17: 1;	byte b18: 1;	byte b19: 1;
    byte b20: 1;	byte b21: 1;	byte b22: 1;	byte b23: 1;
    byte b24: 1;	byte b25: 1;	byte b26: 1;	byte b27: 1;
    byte b28: 1;	byte b29: 1;	byte b30: 1;	byte b31: 1;
  };
} button32_t;

#define HAT_UP			  0
#define HAT_UPRIGHT		1
#define HAT_RIGHT		  2
#define HAT_DOWNRIGHT	3
#define HAT_DOWN		  4
#define HAT_DOWNLEFT	5
#define HAT_LEFT		  6
#define HAT_UPLEFT		7
#define HAT_CENTER		8

typedef union hats8 {
  byte data;
  struct {
    byte switch1: 4;
    byte switch2: 4;
  };
} hats8_t;

#define AXIS_CENTER	0
#define AXIS_MIN	-32767
#define AXIS_MAX	32767

typedef union axis16 {
  int16_t axis;
  struct {
    byte dataHi;
    byte dataLo;
  };
} axis16_t;

typedef struct JoyState { 		// Pretty self explanitory. Simple state to store all the joystick parameters
  //8 signed 16-bit axes
  axis16_t	x;
  axis16_t	y;
  axis16_t	z;
  axis16_t	xRot;
  axis16_t	yRot;
  axis16_t	zRot;
  axis16_t	slider;
  axis16_t	dial;
  //32 buttons
  button32_t	buttons;
  //2 4-bit hat switches
  hats8_t   hats;
} JoyState_t;

class Joystick_
{
  public:
    JoyState_t state;
    Joystick_(void);
    void clearState(void);
    void sendState(void);
};
extern Joystick_ Joystick;

// HID Joystick Descriptor -------------------------------------------
#define HID_JOY_DESC(...) \
  0x05, 0x01,           /*  Usage Page (Generic Desktop) */ \
  0x09, 0x04,           /*  Usage (Joystick) */ \
  0xA1, 0x01,           /*  Collection (Application) */ \
  0x85, 0x01,           /*  REPORT_ID 1 */ \
  0x09, 0x01,           /*    Usage (Pointer) */ \
  0x16, 0x01, 0x80,     /*     Logical Minimum (-32767) */ \
  0x26, 0xFF, 0x7F,     /*     Logical Maximum (32767) */ \
  0xA1, 0x00,           /*    Collection (Physical) */ \
  0x09, 0x30,           /*      Usage (X) */ \
  0x09, 0x31,           /*      Usage (Y) */ \
  0x75, 0x10,           /*       Report Size (16) */ \
  0x95, 0x02,           /*       Report Count (2) */ \
  0x81, 0x02,           /*      Input (Data,Var,Abs) */ \
  0xC0,                 /*    End Collection */ \
  0x09, 0x32,           /*    Usage (Z) */ \
  0x09, 0x33,           /*    Usage (Rx) */ \
  0x09, 0x34,           /*    Usage (Ry) */ \
  0x09, 0x35,           /*    Usage (Rz) */ \
  0x09, 0x36,           /*    Usage (Slider) */ \
  0x09, 0x37,           /*    Usage (Dial) */ \
  0x75, 0x10,           /*     Report Size (16) */ \
  0x95, 0x06,           /*     Report Count (6) */ \
  0x81, 0x02,           /*    Input (Data,Var,Abs) */ \
  \
  0x05, 0x09,           /*    Usage Page (Button) */ \
  0x19, 0x01,           /*    Usage Minimum (Button #1) */ \
  0x29, 0x20,           /*    Usage Maximum (Button #32) */ \
  0x15, 0x00,           /*     Logical Minimum (0) */ \
  0x25, 0x01,           /*     Logical Maximum (1) */ \
  0x75, 0x01,           /*     Report Size (1) */ \
  0x95, 0x20,           /*     Report Count (32) */ \
  0x55, 0x00,           /*     Unit Exponent (0) */ \
  0x65, 0x00,           /*     Unit (None) */ \
  0x81, 0x02,           /*    Input (Data,Var,Abs) */ \
  \
  0x05, 0x01,           /*    Usage Page (Generic Desktop) */ \
  0x09, 0x39,           /*    Usage (Hat switch) */ \
  0x15, 0x00,           /*     Logical Minimum (0) */ \
  0x25, 0x07,           /*     Logical Maximum (7) */ \
  0x35, 0x00,           /*     Physical Minimum (0) */ \
  0x46, 0x3B, 0x01,     /*     Physical Maximum (315) */ \
  0x65, 0x14,           /*     Unit (Eng Rot:Angular Pos) */ \
  0x75, 0x04,           /*     Report Size (4) */ \
  0x95, 0x01,           /*     Report Count (1) */ \
  0x81, 0x02,           /*    Input (Data,var,Abs) */ \
  /* padding */ \
  0x25, 0x00,           /*     Logical Maximum (0) */ \
  0x45, 0x00,           /*     Physical Maximum (0) */ \
  0x75, 0x04,           /*     Report Size (4) */ \
  0x95, 0x01,           /*     Report Count (1) */ \
  0x81, 0x01           /*    Input (Cnst,Ary,Abs) */


#endif
#endif
