#define PPMPIN        2
#define CHANNUMBER    6

#include "Joystick.h"
#include <Arduino.h>
int16_t channel[CHANNUMBER]; 
int counter = 0;
long max_iter = 0;
long calibration [6][4]={
  {0,990,0, 1000}, //X
  {0,990,0, 1000}, //Y
  {0,990,0, 1000}, //Z
  {0,990,0, 1000}, //Rx
  {0,990,0, 1000}, //Ry
  {0,990,0, 1000}  //Rz
};

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, 12, 0,
  true, true, true, true, true, true,
  false, false, false, false, false);
uint16_t setmap(int i){
  long out = (channel[i] - calibration[i][0]) * (calibration[i][3] - calibration[i][2]);
  out = out  / (calibration[i][1] - calibration[i][0]) + calibration[i][2];
  return out;
}

void setup()
{
  Serial.begin(115200); 
  Joystick.setXAxisRange(calibration[0][2]-1000,calibration[0][3]);
  Joystick.setYAxisRange(calibration[1][2]-1000,calibration[1][3]);
  Joystick.setZAxisRange(calibration[2][2]-1000,calibration[2][3]);
  Joystick.setRxAxisRange(calibration[3][2]-1000,calibration[3][3]);
  Joystick.setRyAxisRange(calibration[4][2]-1000,calibration[4][3]);
  Joystick.setRzAxisRange(calibration[5][2]-1000,calibration[5][3]);
  pinMode(PPMPIN, INPUT); 
    while (counter < CHANNUMBER - 1 ) {
    counter++;
    channel[counter] = pulseIn(PPMPIN, HIGH);
    if (max_iter < channel[counter]) {
      max_iter = channel[counter];
      counter = 0;
    }
  }
  Joystick.begin(false);
} 

void loop()
{
  for(int i = 1; i <= CHANNUMBER; i++) 
  {
    channel[i-1]=pulseIn(PPMPIN, HIGH)-585;
  }
  for(int i = 1; i <= CHANNUMBER; i++) 
  {
    Serial.print(channel[i-1]);
    Serial.print(" ");
  }
  Serial.println();
  Joystick.setXAxis(setmap(0));
  Joystick.setYAxis(setmap(1));
  Joystick.setZAxis(setmap(2));
  Joystick.setRxAxis(setmap(3));
  Joystick.setRyAxis(setmap(4));
  Joystick.setRzAxis(setmap(5));
  Joystick.sendState();
}