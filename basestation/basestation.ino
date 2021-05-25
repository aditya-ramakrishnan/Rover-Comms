#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
String command;
int counter = 0;
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

//Command List:
//1) move: mv
////a)direction: # (in clockwise dir'n)(type with 3 characters: 3 degrees = 003)
////c)distance: # (in cm)(type with 4 digits)
//2) receive and send sensor data: tsr
//3) drop tower: drt

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.print("/> ");
  while (Serial.available()==0) {
    
  }
  command=Serial.readString();
  Serial.print(command);
  if(command.charAt(0)=='m') {
    //command = "^" + command.substring(2);
    const char text[9] = {'^',command.charAt(2),command.charAt(3),command.charAt(4),command.charAt(5),command.charAt(6),command.charAt(7),command.charAt(8)};
    radio.write(&text, sizeof(text));
    //delay(100);
  }
  else if(command.charAt(0)=='t') {
    const char text[] = ">";
    radio.write(&text, sizeof(text));
    //delay(100);
  }
  else if(command.charAt(0)=='d'){
    const char text[] = "V";
    radio.write(&text, sizeof(text));
    //delay(100);
  }
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  while (true) {
    counter++;
    if (radio.available()) {
      char confirm[32] = "";
      radio.read(&confirm, sizeof(confirm));
      if (confirm == "Success!") {
        Serial.println(confirm);
        break;
      }      
    }
    if (counter == 100) {
      break;
    }
  }  
}
