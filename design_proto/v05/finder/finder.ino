#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);

//for battery variables
const byte address[6]="00001";
//char batt[3] = "000";

//for testing RF
boolean button_state = 0;
int led_pin = 3; //set led pin
int buzzer_pin = 4; //set buzzer pin

//for testing potentiometer
int pot_pin = A0; //set potentiometer analog pin
int pot_val = 0; //set potentiometer value
char pot_str[] = ""; //set string potentiometer

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Receiver Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1,address);
    radio.enableAckPayload();
    radio.writeAckPayload(1, &pot_val, sizeof(pot_val));
    radio.startListening();
    pinMode(led_pin, OUTPUT);
}

void loop() {
    if ( radio.available() ) {
      //char text[32] = "";                 //Saving the incoming data
      //radio.read(&text, sizeof(text));    //Reading the data
      radio.read(&button_state, sizeof(button_state));    //Reading the data
      if(button_state == HIGH){
        tone(buzzer_pin, 1000);
        digitalWrite(led_pin, HIGH);
        //Serial.println(text);
      }
      else  {
        noTone(buzzer_pin);
        digitalWrite(led_pin, LOW);
        //Serial.println(text);
      }
      pot_val = analogRead(pot_pin); //read potentiometer value
      Serial.print("Battery level : ");
      Serial.println(pot_val); //print potentiometer value
      radio.writeAckPayload(1, &pot_val, sizeof(pot_val)); //send message 
    }
}
