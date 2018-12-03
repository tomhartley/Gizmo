//Code to drive two stepper motors to generate sweet patterns in sand

#include <AccelStepper.h>

// defines pins numbers
const int stepPinA = 15; //A is the linear motor
const int dirPinA = 21; 

const int stepPinB = 22; //B is the radial motor
const int dirPinB = 23;

const int stopPin = 18; //The pin of the E-Stop (low-going)

//Create two steppers objects
AccelStepper linear(1, stepPinA, dirPinA);
AccelStepper circular(2, stepPinB, dirPinB);

void setup()
{
    linear.setMaxSpeed(200.0);
    linear.setAcceleration(100.0);
    
    linear.setPinsInverted(true,false,false);
    
    circular.setMaxSpeed(30000.0);
    circular.setAcceleration(1000000.0);
    circular.setSpeed(500.0);
   
    pinMode(stopPin,INPUT_PULLUP);
    homeLinear();
}

void loop()
{
    //Set the speed to the potentiometer position divided by 3 (so it's in a reasonable range)
    linear.setMaxSpeed(float(analogRead(A2))/3);
    
    if (linear.currentPosition() < 5) { //cycle in and out
      linear.moveTo(2250);
    } else if (linear.currentPosition() > 2245) {
      linear.moveTo(0);
    }
    linear.run(); //update movement of linear motor
    circular.runSpeed(); //update movement of other motor
}

void homeLinear() //This homes the linear position using the end stop, then sets the stepper so a position of 0 is in the middle 
{
    linear.moveTo(linear.currentPosition()+10000); //move further than possible.

    while (digitalRead(stopPin)!=0) {
      linear.run();
    }
    
    linear.setCurrentPosition(2300);
    linear.runToNewPosition(0); //(center)
}

void vibrate() { //Unused so far, but if this is run, it will vibrate the plate for about a minute to clear the sand.
  linear.runToNewPosition(0);
  for (int i = 0; i<350; i++) {
    circular.runToNewPosition(circular.currentPosition()+400);
    circular.runToNewPosition(circular.currentPosition()-400);
  }
  circular.setSpeed(500.0);
}

