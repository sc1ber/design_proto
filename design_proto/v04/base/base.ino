#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); 

const byte address[6]="00001";

char batt[3];

int button_pin = 2;
int led_1 = 8;
int led_2 = 7;
int led_3 = 6;
boolean button_state=0;

void setup() {
    Serial.begin(9600);
    Serial.println("Transmitter Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.enableAckPayload();
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(address);
    pinMode(led_1, OUTPUT);
    pinMode(led_2, OUTPUT);
    pinMode(led_3, OUTPUT);
}

void loop() {
  button_state = digitalRead(button_pin);
  if(button_state == HIGH)  {
    const char text[] = "Your Button State is HIGH";
    radio.write(&text, sizeof(text));
  }
  else  {
    const char text[] = "Your Button State is LOW";
    radio.write(&text, sizeof(text));
  } 
  if ( radio.isAckPayloadAvailable() ) {
      radio.read(&batt,sizeof(batt));
      Serial.print("Message Receieved: ");
      //for (int i = 0; i < sizeof(batt); i++) Serial.print(batt[i],HEX);
      Serial.println(batt);
      ledbatt();
  }
  radio.write(&button_state, sizeof(button_state));
}

void ledbatt(){
  //light up leds according to battery level
  if(strcmp(batt, "000") == 0){
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,LOW);
  }
  if(strcmp(batt, "001") == 0){
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,HIGH);
  }
  if(strcmp(batt, "010") == 0){
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,HIGH);
    digitalWrite(led_3,LOW);
  }
  if(strcmp(batt, "011") == 0){
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,HIGH);
    digitalWrite(led_3,HIGH);
  }
  if(strcmp(batt, "100") == 0){
    digitalWrite(led_1,HIGH);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,LOW);
  }
  if(strcmp(batt, "101") == 0){
    digitalWrite(led_1,HIGH);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,HIGH);
  }
  if(strcmp(batt, "110") == 0){
    digitalWrite(led_1,HIGH);
    digitalWrite(led_2,HIGH);
    digitalWrite(led_3,LOW);
  }
}
