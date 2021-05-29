#include <Wire.h> //I2C functions
#include <Servo.h> // Servo Motors
#include <AFMotor.h> //DC Motors

const double SECONDS_TO_TURN_90 = 1;     //<--- Change 1 to how long it takes to turn 90 degrees in seconds
const double SECONDS_TO_DRIVE_METER = 1; //<--- Change 1 to how long it takes to drive one meter in seconds

bool topTowerDropped(false), bottomTowerDropped(false); //Indicates if the towers have been dropped
Servo servoMotorTop, servoMotorBottom; //Constructs both Servo Motors
int anglePositionTop(0), anglePositionBottom(0); // Starting angles of the Servo Motors in degrees

// Constructs all four DC motors(Parameter corresponds to the motor shield's terminal)
AF_DCMotor frontLeftDC(1);
AF_DCMotor frontRightDC(2);
AF_DCMotor backLeftDC(3);
AF_DCMotor backRightDC(4);

int frontLeftStraight(100), frontRightStraight(100), backLeftStraight(100), backRightStraight(100); // <--- Change 10 to speed when driving straight(0 to 255)
int frontLeftTurn(50), frontRightTurn(50), backLeftTurn(50), backRightTurn(50); // <--- Change 50 to speed when turning(0 to 255, probably lower than straight speed to prevent tipping)

void setup() {
  Wire.begin(2); // Join I2C connection with address #2
  //Pin allocation to Servo Motors
  servoMotorTop.attach(10); //SERVO_1 Pin
  servoMotorBottom.attach(9);  //SERVO_2 Pin
  servoMotorTop.write(0);   //Defaults top Servo Motor to zero degrees
  servoMotorBottom.write(0);  //Defaults bottom Servo Motor to zero degrees

  //Removes all power from all four DC motors and set speed to it's respectively speed variable
  frontLeftDC.run(RELEASE);
  frontLeftDC.setSpeed(0);
  frontRightDC.run(RELEASE);
  frontRightDC.setSpeed(0);
  backLeftDC.run(RELEASE);
  backLeftDC.setSpeed(0);
  backRightDC.run(RELEASE);
  backRightDC.setSpeed(0);
  
  Wire.onReceive(receiveEvent); // Registering event that runs when receives data from Nano
}

void loop() {
  delay(1000);
}

void receiveEvent(int howMany) { //Commented out section for if we have time
  char action = Wire.read();
  int turnDegree = Wire.read();
  int driveDistance = Wire.read();
  
  if(action == '^') { // Rover Drive
    // Will turn one side of motors forward, one side backwards
    if(turnDegree > 0) { // Right(Positive) turn
      int turnScalar = turnDegree / 90;
      frontLeftDC.setSpeed(frontLeftTurn);
      frontRightDC.setSpeed(frontRightTurn);
      backLeftDC.setSpeed(backLeftTurn);
      backRightDC.setSpeed(backRightTurn);
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
//      frontLeftDC.setSpeed(frontLeftTurn);
//      frontRightDC.setSpeed(frontRightTurn);
//      backLeftDC.setSpeed(backLeftTurn);
//      backRightDC.setSpeed(backRightTurn);
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
      frontLeftDC.setSpeed(frontLeftStraight);
      frontRightDC.setSpeed(frontRightStraight);
      backLeftDC.setSpeed(backLeftStraight);
      backRightDC.setSpeed(backRightStraight);
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
//      frontLeftDC.setSpeed(frontLeftStraight);
//      frontRightDC.setSpeed(frontRightStraight);
//      backLeftDC.setSpeed(backLeftStraight);
//      backRightDC.setSpeed(backRightStraight);
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
    if(!bottomTowerDropped) { // Bottom Tower drop
      servoMotorBottom.write(90);
      bottomTowerDropped = true;
    }
    else if(!topTowerDropped) { // Top Tower Drop
      servoMotorTop.write(90);
      topTowerDropped = true;
    }
    else { //No more towers
      // Blink LED?
    }
  }
}
