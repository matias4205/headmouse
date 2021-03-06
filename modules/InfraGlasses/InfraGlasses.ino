#include <Mouse.h>

int rightSensorPin  = A2;
int leftSensorPin = A3;    // IR Sensor

int lastLevelLeft = 0;
int lastLevelRight = 0;    // Previous IR level
int lastChangeLeft = 0;
int lastChangeRight = 0;  // Change in IR level
int changeThreshold = 5; // How hard a rising edge do we need?
 
//visualization
int primaryClickDuration = 100;
int secondaryClickDuration = 500;
float lastStartRight = 0;
float lastStartLeft = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int rightSensorValue = analogRead(rightSensorPin); //1020
  int leftSensorValue = analogRead(leftSensorPin); //1020

  //printValues(leftSensorValue, rightSensorValue);

  // look for rising edges
  lastChangeLeft = leftSensorValue - lastLevelLeft; //1020
  lastLevelLeft = leftSensorValue; //1020

  lastChangeRight = rightSensorValue - lastLevelRight;
  lastLevelRight = rightSensorValue;

  evaluateClick();
  
  delay(100);
}

void printValues(int _leftSensorValue, int _rightSensorValue){
  Serial.print("Left");
  Serial.println(_leftSensorValue);  // Read Data
  Serial.print("Right");
  Serial.println(_rightSensorValue);  // Read Data  
}

void evaluateClick(){
  int _currentMillis = millis();
  
  if (lastChangeRight <= -changeThreshold) {
    lastStartRight = _currentMillis;
  }else if (lastChangeRight >= changeThreshold) {
    int _millisDiff = _currentMillis - lastStartRight;

    if(_millisDiff >= secondaryClickDuration){
      if(Mouse.isPressed(MOUSE_LEFT)){
        Mouse.release(MOUSE_LEFT);
        Mouse.press(MOUSE_LEFT);
      }else{
        Mouse.press(MOUSE_LEFT);
      }
    }else if(_millisDiff >= primaryClickDuration){
      Mouse.click(MOUSE_RIGHT);
    } 

    lastStartRight = _currentMillis;
  }

  if (lastChangeLeft <= -changeThreshold) {
    lastStartLeft = _currentMillis;
  } else if (lastChangeLeft >= changeThreshold) {
    int _millisDiff = _currentMillis - lastStartLeft;

    if(_millisDiff >= secondaryClickDuration){
      Mouse.click(MOUSE_LEFT);
      delay(20);
      Mouse.click(MOUSE_LEFT);
    }else if(_millisDiff >= primaryClickDuration){
      Mouse.click(MOUSE_LEFT);      
    } 

    lastStartLeft = _currentMillis;
  } 
}