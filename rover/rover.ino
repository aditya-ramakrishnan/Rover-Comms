#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
boolean commandReceived = false;
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

// Distance Sensor
const int leftTrigPin(8), middleTrigPin(12), rightTrigPin(2);
const int leftEchoPin(9), middleEchoPin(11), rightEchoPin(3);
double leftDuration(0), middleDuration(0),  rightDuration(0);
double leftDistance(0), middleDistance(0), rightDistance(0);

// Temperature Humidity Sensor
#include <DHT.h>  //Might have to install library for this, if you do just let me know and I'll give the link
#define DHTTYPE DHT11
const int DHT11_PINLeft(13), DHT11_PINRight(7);
DHT dhtLeft(DHT11_PINLeft, DHTTYPE);
DHT dhtRight(DHT11_PINRight, DHTTYPE);
double leftTemperature(0), rightTemperature(0); // You can change the variables to integer if easier, I couldn't figure out how to send doubles since I had a different setup in mind
double leftHumidity(0), rightHumidity(0);

void setup() {
  Serial.begin(9600);
  dhtLeft.begin();
  dhtRight.begin();
  pinMode(leftTrigPin, OUTPUT);
  pinMode(middleTrigPin, OUTPUT);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);
  pinMode(middleEchoPin, INPUT);
  pinMode(rightEchoPin, INPUT);
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
      
      //communicate sensor readings
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
      const char confirm[] = "Success!";
      radio.write(&confirm, sizeof(confirm));
    }
    else if (text == "V") {
      //drop a tower
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
      const char confirm[] = "Success!";
      radio.write(&confirm, sizeof(confirm));
    }
    else {
      //turn the rover to a certain direction and move it the corresponding distance
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
      const char confirm[] = "Success!";
      radio.write(&confirm, sizeof(confirm));
    }
  }
}
