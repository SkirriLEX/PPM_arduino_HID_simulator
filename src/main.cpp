// #define DEBUG

#define PPMPIN 2
#define CHANNUMBER 8

#include "Joystick.h"
#include <Arduino.h>

int16_t channel[CHANNUMBER];
int counter = 0;
long max_iter = 0;
long calibration[CHANNUMBER][4] = {
    {0, 990, 0, 1000}, //X
    {0, 990, 0, 1000}, //Y
    {0, 990, 0, 1000}, //Z
    {0, 990, 0, 1000}, //Rx
    {0, 990, 0, 1000}, //Ry
    {0, 990, 0, 1000}, //Rz
    {0, 990, 0, 1000}, //Ri
    {0, 990, 0, 1000}  //Rd
};

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, 0, 0,
                   true, true, true, true, true, true,
                   true, true, true, true, true);
uint16_t setmap(int i)
{
  long out = (channel[i] - calibration[i][0]) * (calibration[i][3] - calibration[i][2]);
  out = out / (calibration[i][1] - calibration[i][0]) + calibration[i][2];
  return out;
}

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif
  Joystick.setXAxisRange(calibration[0][2] , calibration[0][3]);
  Joystick.setYAxisRange(calibration[1][2] , calibration[1][3]);
  Joystick.setZAxisRange(calibration[2][2] , calibration[2][3]);
  Joystick.setRxAxisRange(calibration[3][2] , calibration[3][3]);
  Joystick.setRyAxisRange(calibration[4][2] , calibration[4][3]);
  Joystick.setRzAxisRange(calibration[5][2] , calibration[5][3]);
  Joystick.setThrottleRange (calibration[6][2] , calibration[6][3]);
  Joystick.setRudderRange (calibration[7][2] , calibration[7][3]);
  Joystick.begin(false);
  pinMode(PPMPIN, INPUT);
  while (counter < CHANNUMBER + 1)
  {
    counter++;
    channel[counter] = pulseIn(PPMPIN, HIGH);
    if (max_iter < channel[counter])
    {
      max_iter = channel[counter];
      counter = 0;
    }
  }
}

void loop()
{
  for (int i = 1; i <= CHANNUMBER; i++)
  {
    channel[i - 1] = pulseIn(PPMPIN, HIGH) - 585;
  }
#ifdef DEBUG
  for (int i = 1; i <= CHANNUMBER; i++)
  {
    Serial.print(channel[i - 1]);
    Serial.print(" ");
  }
  Serial.println();
#endif
  Joystick.setXAxis(setmap(0));
  Joystick.setYAxis(setmap(1));
  Joystick.setThrottle(setmap(2));
  Joystick.setZAxis(setmap(3));
  Joystick.setRxAxis(setmap(4));
  Joystick.setRyAxis(setmap(5));
  Joystick.setRzAxis(setmap(6));
  Joystick.setRudder(setmap(7));
  Joystick.sendState();
}