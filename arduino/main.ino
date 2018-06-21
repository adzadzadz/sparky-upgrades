#include <Servo.h>

const int button1 = 9;
const int button2 = 10;
const int button3 = 11;
const int button4 = 12;
const int longPressTime = 2000; //milliseconds
unsigned long buttonTimer = 0; //milliseconds

Servo door;
float servoStop = 97.5;
float servoForward = 182.5;
float servoReverse = 2.5;
int door1 = 2;
int door2 = 3;
int door3 = 4;
int door4 = 5;

/**
 * Current selected device
 * 0 = "none"
 * 1 = "door"
 * 2 = "window"
 * @type int
 */
int device = 0;

/**
 * The device location to append the action to
 * 0 = none
 * 1 = up-left
 * 2 = up-right
 * 3 = down-left
 * 4 = down-right
 */
int location = 0;

/**
 * Current executed action for the selected device
 * 0 = none
 * 1 = forward
 * 2 = reverse
 * @type int
 */
int action = 0;

/**
 * Input values
 */
int input1, input2, input3, input4;
bool inputState = false;

void setup() {
  Serial.begin(9600);
  setTemp();
}

void loop() {
  
  while ( !device && !action && !location ) {
    setInputs();
    setDevice();
  }

  while ( device && !action && !location ) {
    setLocation(device);
    setAction(device);
  }
  
  resetAll();

}

/**
 * Temporary setup for button triggers
 * @return void
 */
void setTemp() {
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
}

/**
 * Get the input reads
 * @return void
 */
void setInputs() {
  input1 = digitalRead(button1);
  input2 = digitalRead(button2);
  input3 = digitalRead(button3);
  input4 = digitalRead(button4);
}

/**
 * Prints input for debugging
 * @return void
 */
void printInput() {
  Serial.print("button1: ");
  Serial.println(digitalRead(button1));
  Serial.print("button2: ");
  Serial.println(digitalRead(button2));
  Serial.print("button3: ");
  Serial.println(digitalRead(button3));
  Serial.print("button4: ");
  Serial.println(digitalRead(button4));
  Serial.println("=====================");
}

/**
 * Identifies the device for the action
 */
void setDevice() {
  if (input1 || input2 || input3 || input4) {
    
    // Mark the time input1 has been triggered
    if (inputState == false) {
      inputState = true;
      buttonTimer = millis();
    }
    
    Serial.print("ml: ");
    Serial.println(millis() - buttonTimer);
    // calculate the time to greater than or equal to the press time
    if ( (millis() - buttonTimer) >= longPressTime ) {
      device = 1;
      inputState = false;
    }

    
    
  } else {
    // always reset the timer disregarding if 
    // the interval has been finished or not
    inputState = false;
  }
}

/**
 * Set the location of the device that the action will append
 */
void setLocation(int device) {
  if ( device == 1 ) {
    if ( input1 ) {
      location = 1; // loc 1 = upper-left
    } else if ( input2 ) {
      location = 2; // loc 2 = upper-right
    } else if ( input3 ) {
      location = 3; // loc 3 = lower-left
    } else if ( input4 ) {
      location = 4; // loc 4 = lower-right
    } else {
      location = 0; // none
    }
  }

}

/**
 * Executes the action for the selected device
 */
void setAction(int device) {
  // if door, then find which one and open it
  if ( device == 1 ) {
    if ( location == 1 ) {
      door.attach(door1);
    } else if ( location == 2 ) {
      door.attach(door2);
    } else if ( location == 3 ) {
      door.attach(door3);
    } else if ( location == 4 ) {
      door.attach(door4);
    }

    if ( door.attached() ) {
      door.write(servoForward);
      delay(500);
      door.write(servoReverse);
      delay(500);
      door.detach();
    }
  }
  
}

/**
 * Clear all previous instructions
 */
void resetAll() {
  device = 0;
  location = 0;
  action = 0;
}