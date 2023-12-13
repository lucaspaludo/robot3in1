#include <BluetoothSerial.h>
#include <Ultrasonic.h>

#define pinIn1 27
#define pinIn2 14
#define pinIn3 12
#define pinIn4 13

#define triggerEsquerda 19
#define echoEsquerda 18
#define triggerMeio 33
#define echoMeio 32
#define triggerDireita 26
#define echoDireita 25

#define botaoBluetooth 15
#define botaoSumo 2
#define botaoLinha 5

#define sensorLinhaEsquerda 22
#define sensorLinhaDireita 3
#define velocidade 50

BluetoothSerial bluetooth;

unsigned int distancia = 0;
Ultrasonic ultrasonicEsquerda(triggerEsquerda, echoEsquerda);
Ultrasonic ultrasonicMeio(triggerMeio, echoMeio);
Ultrasonic ultrasonicDireita(triggerDireita, echoDireita);


void setup() {

  pinMode(pinIn1, OUTPUT);
  pinMode(pinIn2, OUTPUT);
  pinMode(pinIn3, OUTPUT);
  pinMode(pinIn4, OUTPUT);
  pinMode(botaoBluetooth, INPUT);
  pinMode(botaoSumo, INPUT);
  pinMode(botaoLinha, INPUT);
  pinMode(sensorLinhaEsquerda, INPUT);
  pinMode(sensorLinhaDireita, INPUT);

  Serial.begin(115200);
  bluetooth.begin("Esp32");
  Serial.println("Dispositivo iniciado, pode começar a parear...");

}

void loop() {
  
    bool estadoBotaoBluetooth = digitalRead(botaoBluetooth);
    bool estadoBotaoSumo = digitalRead(botaoSumo);
    bool estadoBotaoLinha = digitalRead(botaoLinha);

    if (estadoBotaoBluetooth) {
      Serial.println("Modo Bluetooth");
      roboBluetooth();
      delay(50);
    }

    else if (estadoBotaoSumo) {
      Serial.println("Modo Sumô");
      roboSumo();
      delay(50);
    }

    else if (estadoBotaoLinha) {
      Serial.println("Modo Seguidor de Linha");
      roboLinha();
      delay(50);
    }

    else {
      digitalWrite(pinIn1, LOW);
      digitalWrite(pinIn2, LOW);
      digitalWrite(pinIn3, LOW);
      digitalWrite(pinIn4, LOW);
      delay(50);
    }

  }

  void roboBluetooth() { 

      if (Serial.available()) {
          bluetooth.write(Serial.read());
      }

      if (bluetooth.available()) {
        char recebido = bluetooth.read();
        Serial.write(recebido);

      if (recebido == 'F') {
        digitalWrite(pinIn1, HIGH);
        digitalWrite(pinIn4, HIGH);
      }

      if (recebido == 'B') {
        digitalWrite(pinIn2, HIGH);
        digitalWrite(pinIn3, HIGH);
      }

      if (recebido == 'L') {
        digitalWrite(pinIn1, HIGH);
      }

      if (recebido == 'R') {
        digitalWrite(pinIn4, HIGH);                                                                                                                                                                                                                                                                                                                                                                           
      }

      if (recebido == 'S') {
        digitalWrite(pinIn1, LOW);
        digitalWrite(pinIn2, LOW);
        digitalWrite(pinIn3, LOW);
        digitalWrite(pinIn4, LOW);
      }
     delay(20);
    }
  }


  void roboSumo() {

    int distanciaEsquerda;
    long microsecEsquerda = ultrasonicEsquerda.timing();
    distanciaEsquerda = ultrasonicEsquerda.convert(microsecEsquerda, Ultrasonic::CM);

    int distanciaMeio;
    long microsecMeio = ultrasonicMeio.timing();
    distanciaMeio = ultrasonicMeio.convert(microsecMeio, Ultrasonic::CM);

    int distanciaDireita;
    long microsecDireita = ultrasonicDireita.timing();
    distanciaDireita = ultrasonicDireita.convert(microsecDireita, Ultrasonic::CM);
        
    if (distanciaEsquerda < 15) {

      digitalWrite(pinIn1, HIGH);
      delay(500);
          
      digitalWrite(pinIn1, HIGH);
      digitalWrite(pinIn4, HIGH);
      delay(1000);

      digitalWrite(pinIn1, LOW);
      digitalWrite(pinIn4, LOW);
      delay(30);

      digitalWrite(pinIn2, HIGH);
      digitalWrite(pinIn3, HIGH);
      delay(1000);

      digitalWrite(pinIn2, LOW);
      digitalWrite(pinIn3, LOW);
      delay(300);

    }

    else if (distanciaMeio < 15) {

        digitalWrite(pinIn1, HIGH);
        digitalWrite(pinIn4, HIGH);
        delay(1000);

        digitalWrite(pinIn1, LOW);
        digitalWrite(pinIn4, LOW);
        delay(30);

        digitalWrite(pinIn2, HIGH);
        digitalWrite(pinIn3, HIGH);
        delay(1000);

        digitalWrite(pinIn2, LOW);
        digitalWrite(pinIn3, LOW);
        delay(300);

    }

    else if (distanciaDireita < 15) {

      digitalWrite(pinIn4, HIGH);
      delay(500);

      digitalWrite(pinIn1, HIGH);
      digitalWrite(pinIn4, HIGH);
      delay(1000);

      digitalWrite(pinIn1, LOW);
      digitalWrite(pinIn4, LOW);
      delay(30);

      digitalWrite(pinIn2, HIGH);
      digitalWrite(pinIn3, HIGH);
      delay(1000);

      digitalWrite(pinIn2, LOW);
      digitalWrite(pinIn3, LOW);
      delay(300);

    }

    else {
      Serial.println("Modo neutro");
      digitalWrite(pinIn1, LOW);
      digitalWrite(pinIn2, LOW);
      digitalWrite(pinIn3, LOW);
      digitalWrite(pinIn4, LOW);
    }
  }

  void roboLinha() {
    bool estadoE = digitalRead(sensorLinhaEsquerda);
    bool estadoD = digitalRead(sensorLinhaDireita);

    if (!estadoD && !estadoE){
      digitalWrite(pinIn1, HIGH);
      digitalWrite(pinIn4, HIGH);
      digitalWrite(pinIn2, LOW);
      digitalWrite(pinIn3, LOW);
      Serial.println("Frente");
    }

    if (estadoD && estadoE) {
      digitalWrite(pinIn1, LOW);
      digitalWrite(pinIn2, LOW);
      digitalWrite(pinIn3, LOW);
      digitalWrite(pinIn4, LOW);
      Serial.println("Parado");
    }

    if (!estadoD && estadoE) {
      digitalWrite(pinIn1, HIGH);
      digitalWrite(pinIn2, LOW);
      digitalWrite(pinIn3, LOW);  
      digitalWrite(pinIn4, LOW);
      
      Serial.println("Direita");
    }

    if (estadoD && !estadoE) {
      digitalWrite(pinIn1, LOW);
      digitalWrite(pinIn2, LOW);
      digitalWrite(pinIn3, LOW);  
      digitalWrite(pinIn4, HIGH);
      Serial.println("Esquerda");
    }


  }

  




