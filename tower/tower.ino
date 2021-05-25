#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
boolean commandReceived = false;
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
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
    if (text != "Success!" && commandReceived == false) {
      Serial.println(text);
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
      radio.write(&text, sizeof(text));
      commandReceived = true;
    }
    if (text == "Success" && commandReceived == true) {
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
      const char confirm[] = "Success!";
      radio.write(&confirm, sizeof(confirm));
      commandReceived == false;
    }
  }
}