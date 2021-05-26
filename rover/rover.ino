#include <DHT.h>  //Might have to install library for these, if you do just let me know and I'll give the link
#include <Wire.h> //I2C functions
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

// Distance Sensor
#define leftTrigPin A0
const int middleTrigPin(4), rightTrigPin(2);
const int leftEchoPin(6), middleEchoPin(5), rightEchoPin(3);
double leftDuration(0), middleDuration(0),  rightDuration(0);
int leftDistance(0), middleDistance(0), rightDistance(0);

// Temperature Humidity Sensor
#define DHTTYPE DHT11
#define DHT11_PINLeft A1
#define DHT11_PINRight A2
DHT dhtLeft(DHT11_PINLeft, DHTTYPE);
DHT dhtRight(DHT11_PINRight, DHTTYPE);
int leftTemperature(0), rightTemperature(0);
int leftHumidity(0), rightHumidity(0);

String lT,rT,lH,rH,lD,mD,rD;

void setup() {
  Serial.begin(9600);
  pinMode(leftTrigPin, OUTPUT); //Analog Pin A0 to digital output
  pinMode(DHT11_PINLeft, OUTPUT); //Analog Pin A1 to digital output
  pinMode(DHT11_PINRight, OUTPUT); //Analog Pin A2 to digital output
  dhtLeft.begin();
  dhtRight.begin();
  dhtLeft.read(DHT11_PINLeft);    // Initial Readings
  dhtRight.read(DHT11_PINRight);  // Initial Readings
  pinMode(leftTrigPin, OUTPUT);
  pinMode(middleTrigPin, OUTPUT);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);
  pinMode(middleEchoPin, INPUT);
  pinMode(rightEchoPin, INPUT);
  Wire.begin(2); // Join I2C connection with address #2
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    if (text == ">") {
      //communicate sensor readings
      // Left Distance Sensor
      digitalWrite(leftTrigPin, LOW);
      delay(2);
      digitalWrite(leftTrigPin, HIGH);
      delayMicroseconds(10); 
      digitalWrite(leftTrigPin, LOW);
      leftDuration = pulseIn(leftEchoPin, HIGH);
      leftDistance = leftDuration * 0.0343 / 2; // leftDistance contains the distance value(cm)
      // Middle Distance Sensor
      digitalWrite(middleTrigPin, LOW);
      delay(2);
      digitalWrite(middleTrigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(middleTrigPin, LOW);
      middleDuration = pulseIn(middleEchoPin, HIGH);
      middleDistance = middleDuration * 0.0343 / 2;  // middleDistance contains the distance value(cm)
      // Right Distance Sensor
      digitalWrite(rightTrigPin, LOW);
      delayMicroseconds(10);
      digitalWrite(rightTrigPin, HIGH);
      delayMicroseconds(10); 
      digitalWrite(rightTrigPin, LOW);
      rightDuration = pulseIn(rightEchoPin, HIGH);
      rightDistance = rightDuration * 0.0343 / 2; // rightDistance contains the distance value(cm)
      // Read Temperature/Humidity Sensors
      dhtLeft.read(DHT11_PINLeft);
      dhtRight.read(DHT11_PINRight);
      leftTemperature = dhtLeft.readTemperature();  // leftTemperature contains temperature value(C)
      rightTemperature = dhtRight.readTemperature();  // rightTemperature contains temperature value(C)
      leftHumidity = dhtLeft.readHumidity();  // leftHumidity contains humidity value(%)
      rightHumidity = dhtRight.readHumidity();  // rightHumidity contains humidity value(%)
      if (leftTemperature < 10) {lT = "0" + String(leftTemperature);}
      if (rightTemperature < 10) {lT = "0" + String(rightTemperature);}
      if (leftHumidity < 10) {lT = "0" + String(leftHumidity);}
      if (rightHumidity < 10) {lT = "0" + String(rightHumidity);}
      switch(leftDistance) {
        case leftDistance <10:
          lD = "000" + String(leftDistance);
        case leftDistance <100:
          lD = "00" + String(leftDistance);
        case leftDistance <1000:
          lD = "0" + String(leftDistance);
      }
      switch(middleDistance) {
        case middleDistance <10:
          mD = "000" + String(middleDistance);
        case middleDistance <100:
          mD = "00" + String(middleDistance);
        case middleDistance <1000:
          mD = "0" + String(middleDistance);
      }
      switch(rightDistance) {
        case rightDistance <10:
          rD = "000" + String(rightDistance);
        case rightDistance <100:
          rD = "00" + String(rightDistance);
        case rightDistance <1000:
          rD = "0" + String(rightDistance);
      }
      
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
      const char data[] = {'s','d',lT.charAt(0),lT.charAt(1),',',' ',rT.charAt(0),rT.charAt(1),',',' ',lH.charAt(0),lH.charAt(1),',',' ',rH.charAt(0),rH.charAt(1),',',' ',lD.charAt(0),lD.charAt(1),lD.charAt(2),lD.charAt(3),',',' ',mD.charAt(0),mD.charAt(1),mD.charAt(2),mD.charAt(3),',',' ',rD.charAt(0),rD.charAt(1),rD.charAt(2),rD.charAt(3)};
      radio.write(&data, sizeof(data));
      delay(500);
    }
    else if (text == "V") {
      //drop a tower
      Wire.write('V');
      Wire.write(0);
      Wire.write(0);
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
      const char confirm[] = "1";
      radio.write(&confirm, sizeof(confirm));
      delay(500);
    }
    else {
      //turn the rover to a certain direction and move it the corresponding distance
      Wire.write('^');
      String degrees = text[1] + text[2] + text[3];
      Wire.write(degrees.toInt());
      String distance = text[4] + text[5] + text[6] + text[7];
      Wire.write(degrees.toInt());
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
      const char confirm[] = "1";
      radio.write(&confirm, sizeof(confirm));
      delay(500);
    }
  }
}
