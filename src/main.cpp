/*
Del siguiente codigo quiero que le agregues de que cuando resiba los siguientes pulsos en el pin GPIO36:
2 pulsos -> sepa que ingreso una moneda de 50 centavos y lo agregue a la logica para mostrar  esto en la terminal
4 pulsos -> sepa que ingreso una moneda de 1 pesos y lo agregue a la logica para mostrar  esto en la terminal
6 pulsos -> sepa que ingreso una moneda de 2 pesos y lo agregue a la logica para mostrar  esto en la terminal
8 pulsos -> sepa que ingreso una moneda de 5 pesos y lo agregue a la logica para mostrar  esto en la terminal
10 pulsos -> sepa que ingreso una moneda de 10 pesos y lo agregue a la logica para mostrar  esto en la terminal



#include <Arduino.h>

const int coinSelector = 36; // GPIO 36 en el ESP32
const int signalCostFactor = 5; // Cada pulso de señal tiene un valor de 5p

int signalValue = 0; // Almacena el valor de entrada analógica

int state; // Estado actual
int lastState = 0; // Último estado

int balance = 0; // Pence
int coinValue = 0; // Valor actual de la moneda

int update = 1; // Usado para enviar una actualización

long updateDebounceTime = 0; // Último momento en que enviamos una actualización
long updateDebounceDelay = 500; // Actualizar 500 ms después del último pulso de señal

void setup() {
  Serial.begin(9600); // Inicializar comunicación serial a 9600 baudios

  delay(2000); // Esperar para asegurarse de que el selector de monedas haya iniciado

  Serial.println("Ready..");
}

void loop() {
  signalValue = analogRead(coinSelector); // Leer valor analógico del selector de monedas

  if (signalValue > 1000) {
    state = 1; // El estado es 1 cuando está alto
  } else {
    state = 0;

    // ¿Deberíamos enviar una actualización de saldo?
    if (update == 0) {
      if ((millis() - updateDebounceTime) > updateDebounceDelay) {
        Serial.print("Valor de la moneda: ");
        Serial.println(coinValue); // ADVERTENCIA: El valor de la moneda será incorrecto si se insertan monedas dentro de updateDebounceDelay, ajusta la demora y prueba

        Serial.print("Balance: ");
        Serial.println(balance); // Esto debería ser más preciso, ya que deberíamos obtener la misma cantidad de pulsos incluso si se insertan varias monedas al mismo tiempo

        coinValue = 0; // Restablecer el valor actual de la moneda

        update = 1; // Asegurarse de no ejecutarse nuevamente hasta la siguiente moneda
      }
    }
  } 

  if (state != lastState) {
    // Procesar nueva señal
    if (state == 1) {
      balance = balance + signalCostFactor; // Actualizar el saldo
      coinValue = coinValue + signalCostFactor; // Actualizar el valor de la moneda

      updateDebounceTime = millis(); // Actualizar el último momento en que procesamos una señal

      update = 0; // ¿Es hora de enviar una actualización ahora?
    }

    lastState = state; // Actualizar el último estado
  }

  delay(1);
}
*/

#include <Arduino.h>

const int coinSelector = 36; // GPIO 36 en el ESP32
const int signalCostFactor = 5; // Cada pulso de señal tiene un valor de 5p

int signalValue = 0; // Almacena el valor de entrada analógica

int state; // Estado actual
int lastState = 0; // Último estado

int balance = 0; // Pence
int coinValue = 0; // Valor actual de la moneda

int update = 1; // Usado para enviar una actualización

long updateDebounceTime = 0; // Último momento en que enviamos una actualización
long updateDebounceDelay = 500; // Actualizar 500 ms después del último pulso de señal

void setup() {
  Serial.begin(9600); // Inicializar comunicación serial a 9600 baudios

  delay(2000); // Esperar para asegurarse de que el selector de monedas haya iniciado

  Serial.println("Ready..");
}

void loop() {
  signalValue = analogRead(coinSelector); // Leer valor analógico del selector de monedas

  if (signalValue > 1000) {
    state = 1; // El estado es 1 cuando está alto
  } else {
    state = 0;

    // ¿Deberíamos enviar una actualización de saldo?
    if (update == 0) {
      if ((millis() - updateDebounceTime) > updateDebounceDelay) {
        if (coinValue > 0) {
          Serial.print("Ingresó una moneda de ");
          Serial.print(coinValue);
          Serial.println(" pesos");
        }

        coinValue = 0; // Restablecer el valor actual de la moneda

        update = 1; // Asegurarse de no ejecutarse nuevamente hasta la siguiente moneda
      }
    }
  } 

  if (state != lastState) {
    // Procesar nueva señal
    if (state == 1) {
      balance = balance + signalCostFactor; // Actualizar el saldo
      coinValue = coinValue + signalCostFactor; // Actualizar el valor de la moneda

      updateDebounceTime = millis(); // Actualizar el último momento en que procesamos una señal

      update = 0; // ¿Es hora de enviar una actualización ahora?
    }

    lastState = state; // Actualizar el último estado
  }

  delay(1);
}
