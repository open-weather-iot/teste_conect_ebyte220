/*
Firmware LoRa com Rasp Pico W e módulo E220
14/11/2022
Transmissor
M0 - 1
M1 -0
*/
#include "Arduino.h"
#include "LoRa_E220.h"
int cont = 0;

LoRa_E220 e220ttl(&Serial2, 2, 10, 11); //  RX AUX M0 M1

void setup() {
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
 

 

  // INICIA O MÓDULO LORA E220
  e220ttl.begin();

  // ENVIA UM DADO DE TESTE
  Serial.println("ENVIANDO UMA MENSAGEM DE TESTE");
  //ResponseStatus rs = e220ttl.sendBroadcastFixedMessage(23, String(10,DEC);
  // Verfica se o dado foi enviado com sucesso
 // Serial.println(rs.getResponseDescription());
}

void loop() {
   digitalWrite(10,HIGH);
   digitalWrite(11,LOW);
    Serial.println("Teste LoRa EByte");
    Serial.println("Pct:" + String(cont));
  ResponseStatus rs = e220ttl.sendBroadcastFixedMessage(23, String(cont++, DEC));
  // Verfica se o dado foi enviado com sucesso
  Serial.println(rs.getResponseDescription());
  delay(200);
}

