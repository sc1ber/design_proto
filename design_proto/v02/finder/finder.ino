//v02
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0;
int led_pin = 3; //set led pin
int buzzer_pin = 4; //set buzzer pin
void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
}
void loop(){
  if (radio.available()) {             //Looking for the data.
    char text[32] = "";                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data
    radio.read(&button_state, sizeof(button_state));    //Reading the data
    if(button_state == HIGH){
      tone(buzzer_pin, 1000);
      digitalWrite(led_pin, HIGH);
      Serial.println(text);
    }
    else  {
      noTone(buzzer_pin);
      digitalWrite(led_pin, LOW);
      Serial.println(text);}
    }
  delay(5);
}
