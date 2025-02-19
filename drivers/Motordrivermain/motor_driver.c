#include "motor_driver.h"

int speed (int iSpeed){
  int iAnalogSpeed = (iSpeed * 255) / 100;  //convert in analog scale
  analogWrite(dPinSpeed, iAnalogSpeed);   //send scaled speed wanted to the pin
  return 0;
}

int direction(int iDir){
//pin 2 and 4 to choose backwardvand forward
int iFlag;

if (iDir == 0)    //break
{
  analogWrite(dPinMove1, 0);
  analogWrite(dPinMove2, 0);
  iFlag = 0;

}

if (iDir == 1)    //Forward
{
  analogWrite(dPinMove1, 1);
  analogWrite(dPinMove2, 0);
  iFlag = 1;
}

if (iDir == 2)    //backward
{
  analogWrite(dPinMove1, 0);
  analogWrite(dPinMove2, 1);
  iFlag = 2;
}

  return iFlag;
}


int motorCurrent(){

  int iCurrentRead = analogRead(dPinCurrent1) + analogRead(dPinCurrent2);
  return iCurrentRead;

}


int enableMotor(int iOnOff){
  analogWrite(dPinOnOff, iOnOff);
  return 0;
}


int test(int iTest)
{
  if(iTest == 0){
    return 5;
    }

  if(iTest == 1){
    return 4;
    }
}
