#include <Wire.h> //I2C functions
#include <Servo.h> // Servo Motors
#include <AFMotor.h> //DC Motors

const double SECONDS_TO_TURN_90 = 10;
const double SECONDS_TO_DRIVE_METER = 10;

bool firstTowerDropped(false), secondTowerDropped(false); //Indicates if the towers have been dropped
Servo servoMotorTop, servoMotorBottom; //Constructs both Servo Motors
int anglePositionFirst(0), anglePositionSecond(0); // Starting angles of the Servo Motors in degrees

// Constructs all four DC motors(Parameter corresponds to the motor shield's terminal)
AF_DCMotor frontLeftDC(1);
AF_DCMotor frontRightDC(2);
AF_DCMotor backLeftDC(3);
AF_DCMotor backRightDC(4);

int frontLeftSpeed(0), frontRightSpeed(0), backLeftSpeed(0), backRightSpeed(0); // Speed variables for each motor

void setup() {
  Wire.begin(2); // Join I2C connection with address #2
  //Pin allocation to Servo Motors
  servoMotorTop.attach(10); //SERVO_1 Pin
  servoMotorBottom.attach(9);  //SERVO_2 Pin
  servoMotorTop.write(0);   //Defaults top Servo Motor to zero degrees
  servoMotorBottom.write(0);  //Defaults bottom Servo Motor to zero degrees

  //Removes all power from all four DC motors and set speed to it's respectively speed variable
  frontLeftDC.run(RELEASE);
  frontLeftDC.setSpeed(frontLeftSpeed);
  frontRightDC.run(RELEASE);
  frontRightDC.setSpeed(frontRightSpeed);
  backLeftDC.run(RELEASE);
  backLeftDC.setSpeed(backLeftSpeed);
  backRightDC.run(RELEASE);
  backRightDC.setSpeed(backRightSpeed);
  
  Wire.onReceive(receiveEvent); // Registering event that runs when receives data from Nano
}

void loop() {
  delay(1000);
}

void receiveEvent(int howMany) { //Commented out section for negative values if we have time
  char action = Wire.read();
  int turnDegree = Wire.read();
  int driveDistance = Wire.read();
  
  if(action == '^') { // Rover Drive
    // Will turn one side of motors forward, one side backwards
    if(turnDegree > 0) { // Right(Positive) turn
      int turnScalar = turnDegree / 90;
      frontLeftDC.setSpeed(frontLeftSpeed);
      frontRightDC.setSpeed(frontRightSpeed);
      backLeftDC.setSpeed(backLeftSpeed);
      backRightDC.setSpeed(backRightSpeed);
      frontLeftDC.run(FORWARD);
      frontRightDC.run(BACKWARD);
      backLeftDC.run(FORWARD);
      backRightDC.run(BACKWARD);
      delay(turnScalar * SECONDS_TO_TURN_90 * 1000);
      frontLeftDC.run(RELEASE);
      frontRightDC.run(RELEASE);
      backLeftDC.run(RELEASE);
      backRightDC.run(RELEASE);
    }
//    else if(turnDegree < 0) { // Left(Negative) turn
//      frontLeftDC.setSpeed(frontLeftSpeed);
//      frontRightDC.setSpeed(frontRightSpeed);
//      backLeftDC.setSpeed(backLeftSpeed);
//      backRightDC.setSpeed(backRightSpeed);
//      frontLeftDC.run(BACKWARD);
//      frontRightDC.run(FORWARD);
//      backLeftDC.run(BACKWARD);
//      backRightDC.run(FORWARD);
//      
//      delay(SECONDS_TO_TURN_90 * 1000);
//      frontLeftDC.run(RELEASE);
//      frontRightDC.run(RELEASE);
//      backLeftDC.run(RELEASE);
//      backRightDC.run(RELEASE);
//    }

    if(driveDistance > 0) { // Positive drive
      double driveScalar = driveDistance / 1000;
      frontLeftDC.setSpeed(frontLeftSpeed);
      frontRightDC.setSpeed(frontRightSpeed);
      backLeftDC.setSpeed(backLeftSpeed);
      backRightDC.setSpeed(backRightSpeed);
      frontLeftDC.run(FORWARD);
      frontRightDC.run(FORWARD);
      backLeftDC.run(FORWARD);
      backRightDC.run(FORWARD);
      
      delay(driveScalar * SECONDS_TO_DRIVE_METER * 1000);
      frontLeftDC.run(RELEASE);
      frontRightDC.run(RELEASE);
      backLeftDC.run(RELEASE);
      backRightDC.run(RELEASE); 
    }
//    else if(DRIVE DRIVE < 0) {
//      frontLeftDC.setSpeed(frontLeftSpeed);
//      frontRightDC.setSpeed(frontRightSpeed);
//      backLeftDC.setSpeed(backLeftSpeed);
//      backRightDC.setSpeed(backRightSpeed);
//      frontLeftDC.run(BACKWARD);
//      frontRightDC.run(BACKWARD);
//      backLeftDC.run(BACKWARD);
//      backRightDC.run(BACKWARD);
//      
//      delay(SECONDS_TO_DRIVE_METER * 1000);
//      frontLeftDC.run(RELEASE);
//      frontRightDC.run(RELEASE);
//      backLeftDC.run(RELEASE);
//      backRightDC.run(RELEASE); 
//    }
  }
  else if(action == 'V') { // Tower Drop
    if(!firstTowerDropped) { // Bottom Tower drop
      servoMotorBottom.write(90);
      firstTowerDropped = true;
    }
    else if(!secondTowerDropped) { // Top Tower Drop
      servoMotorTop.write(90);
      secondTowerDropped = true;
    }
    else { //No more towers
      // Blink LED?
    }
  }
}
