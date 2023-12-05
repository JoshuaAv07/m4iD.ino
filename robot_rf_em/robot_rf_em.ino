#include <SPI.h>  
#include <RF24.h>

const int joyY1 = A1;
const int joyY2 = A3;

RF24 radio(9, 10); // CE, CSN
const byte direccion[5] = "00001";

int datos[2];
 
void setup() {
  radio.begin();
  Serial.begin(9600); //Iniciamos la comunicacion serial

  radio.openWritingPipe(direccion);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int yValue1 = analogRead(joyY1);
  int yValue2 = analogRead(joyY2);
  
  Serial.print("Joystick 1: ");
  Serial.print(yValue1, DEC);
  Serial.print("\n");
  Serial.print("Joystick 2: ");
	Serial.print(yValue2, DEC);
  Serial.print("\n");

  datos[0] = yValue1;
  datos[1] = yValue2;

  bool ok = radio.write(&datos, sizeof(datos));
  if(ok) {
    Serial.print("Datos enviados: "); 
    Serial.print(datos[0]); 
    Serial.print(" , "); 
    Serial.println(datos[1]);
  }
  else {
    Serial.println("no se ha podido enviar");
  }
  delay(50);
}
