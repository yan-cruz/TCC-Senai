#include <Servo.h>
#include "LiquidCrystal.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

const int sensorGas  = 0;

int valorSensorGas  = 0;  

LiquidCrystal lcd(9, 8, 5, 10, 12, 11);

Servo servo_13;

bool mensagemEnviada = false;

void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(1000);
  pinMode(A0, INPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  servo_13.attach(13, 500, 2500);
  lcd.begin(16, 2);
  Serial.begin(9600);

}

void loop(){
  valorSensorGas  = analogRead(sensorGas);
  Serial.print("Gas: ");
  Serial.println(valorSensorGas);

  if (analogRead(A0) > 600) {
    if (!mensagemEnviada){
    mySerial.println("AT");
    updateSerial();
    mySerial.println("AT+CMGF=1");
    updateSerial();
    mySerial.println("AT+CMGS=\"+55379XXXXXXXX\"");
    updateSerial();
    mySerial.print("Vazamento detectado!\nOs sinais foram disparados e o setor evacuado imediatamente.");
    updateSerial();
    mySerial.write(26);
    mensagemEnviada = true;
    }
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    servo_13.write(90);
    delay(1000);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    tone(6, 788, 500);
    delay(300);
    lcd.setCursor(0, 1);
    lcd.print("EVACUE O SETOR!");
    
    mensagemEnviada = true; 
  } 
  
  else {
    delay(1000);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    tone(7, 0.7, 1);
    delay(300);
    servo_13.write(0);
    lcd.setCursor(0, 1);
    lcd.print("Ambiente Seguro");
    mensagemEnviada = false; 
  }
  
  lcd.setCursor(0,0);  
  lcd.print("Gas: ");        
  lcd.print(valorSensorGas); 
  
}

void updateSerial(){
  
  while (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
  delay(500);
}