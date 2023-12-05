#include <SPI.h>  
#include <RF24.h>

const int motor1Pin1 = 7; // Entrada 2 del puente-H
const int motor1Pin2 = 6; // Entrada 7 del puente-H

const int motor2Pin1 = 5; // Entrada 2 del puente-H
const int motor2Pin2 = 4; // Entrada 7 del puente-H

RF24 radio(9, 10); // CE, CSN
const byte direccion[5] = "00001";

int datos[2];
 
void setup() {
  Serial.begin(9600); //Iniciamos la comunicacion serial
  radio.begin();

  radio.openReadingPipe(0, direccion);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(motor1Pin1, OUTPUT); 
  pinMode(motor1Pin2, OUTPUT); 

  pinMode(motor2Pin1, OUTPUT); 
  pinMode(motor2Pin2, OUTPUT); 
}

void loop() {

  if (radio.available()){
    radio.read(&datos, sizeof(datos));
    Serial.println(datos[0]);
    Serial.println(datos[1]);

    if (datos[0] > 600) {
      Serial.println("Girando en sentido horario...");
      digitalWrite(motor1Pin1,LOW);
      digitalWrite(motor1Pin2,HIGH); 
    }

    else if (datos[0] < 300) {
      Serial.println("Girando en sentido anti-horario...");
      digitalWrite(motor1Pin1,HIGH);
      digitalWrite(motor1Pin2,LOW);
    }

    else if (datos[0] > 300 && datos[0] < 600) {
      Serial.println("Stop.");
      digitalWrite(motor1Pin1,LOW);
      digitalWrite(motor1Pin2,LOW);
    }
 
    if (datos[1] > 600) { 
      Serial.println("Girando en sentido anti-horario...");
      digitalWrite(motor2Pin1,HIGH);
      digitalWrite(motor2Pin2,LOW);
    }
 
    else if (datos[1] < 300) { 
      Serial.println("Girando en sentido horario...");
      digitalWrite(motor2Pin1,LOW);
      digitalWrite(motor2Pin2,HIGH);
    }

    else if (datos[1] > 300 && datos[1] < 600) {
      Serial.println("Stop.");
      digitalWrite(motor2Pin1,LOW);
      digitalWrite(motor2Pin2,LOW);
    }
  }
}
