/*
=========================================================
AGILIS - Robot Sumo Controlado por PS4
Microcontrolador: ESP32 DevKit V1
Driver de motores: TB6612FNG
Control inalámbrico: PlayStation 4 (Bluetooth clásico)

Core ESP32 recomendado: 2.0.17
=========================================================
*/

#include <PS4Controller.h>
#include <SparkFun_TB6612.h>

// =============================
// DEFINICIÓN DE PINES TB6612
// =============================

// Motor Izquierdo (Canal A)
#define AIN1 13
#define AIN2 14
#define PWMA 26

// Motor Derecho (Canal B)
#define BIN1 12
#define BIN2 27
#define PWMB 25

// Pin Standby del driver
#define STBY 33

// =============================
// CONFIGURACIÓN DE DIRECCIÓN
// =============================
// Si un motor gira invertido cambiar 1 por -1
const int offsetA = 1;
const int offsetB = 1;

// Crear objetos motores
Motor motorLeft  = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motorRight = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// =============================
// PARÁMETROS DE VELOCIDAD
// =============================
int maxSpeed = 255;   // Velocidad máxima PWM
int minSpeed = 40;    // Velocidad mínima útil
int deadZone = 10;    // Zona muerta joystick

// =============================
// SETUP
// =============================
void setup() {

  Serial.begin(115200);

  // Activar driver
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  // Iniciar Bluetooth PS4
  PS4.begin();

  Serial.println("===================================");
  Serial.println("AGILIS LISTO - Esperando PS4...");
  Serial.println("===================================");
}

// =============================
// LOOP PRINCIPAL
// =============================
void loop() {

  // Solo ejecutar si el control está conectado
  if (PS4.isConnected()) {

    int forwardValue = PS4.LStickY();  // Adelante / atrás
    int turnValue    = PS4.RStickX();  // Giro

    bool moving = false;

    // =====================================
    // BOTON X → AVANCE MAXIMO
    // =====================================
    if (PS4.Cross()) {
      forward(motorLeft, motorRight, maxSpeed);
      Serial.println("BOTON X - FULL FORWARD");
      moving = true;
    }

    // =====================================
    // FLECHA DERECHA → GIRO DIFERENCIAL
    // =====================================
    else if (PS4.Right()) {
      motorLeft.drive(200);
      motorRight.drive(-200);
      Serial.println("FLECHA DERECHA - GIRO DIFERENCIAL");
      moving = true;
    }

    // =====================================
    // FLECHA IZQUIERDA → GIRO DIFERENCIAL
    // =====================================
    else if (PS4.Left()) {
      motorLeft.drive(-200);
      motorRight.drive(200);
      Serial.println("FLECHA IZQUIERDA - GIRO DIFERENCIAL");
      moving = true;
    }

    // =====================================
    // JOYSTICK IZQUIERDO → ADELANTE PROPORCIONAL
    // =====================================
    else if (forwardValue > deadZone) {
      int speed = map(forwardValue, deadZone, 127, minSpeed, maxSpeed);
      forward(motorLeft, motorRight, speed);
      Serial.printf("Forward PWM: %d\n", speed);
      moving = true;
    }

    // =====================================
    // JOYSTICK IZQUIERDO → ATRAS PROPORCIONAL
    // =====================================
    else if (forwardValue < -deadZone) {
      int speed = map(forwardValue, -deadZone, -128, minSpeed, maxSpeed);
      back(motorLeft, motorRight, speed);
      Serial.printf("Backward PWM: %d\n", speed);
      moving = true;
    }

    // =====================================
    // JOYSTICK DERECHO → GIRO PROPORCIONAL
    // =====================================
    else if (turnValue > deadZone) {
      int speed = map(turnValue, deadZone, 127, minSpeed, maxSpeed);
      motorLeft.drive(speed);
      motorRight.drive(-speed);
      Serial.printf("Turn Right PWM: %d\n", speed);
      moving = true;
    }

    else if (turnValue < -deadZone) {
      int speed = map(turnValue, -deadZone, -128, minSpeed, maxSpeed);
      motorLeft.drive(-speed);
      motorRight.drive(speed);
      Serial.printf("Turn Left PWM: %d\n", speed);
      moving = true;
    }

    // =====================================
    // SI NO HAY MOVIMIENTO → FRENO ELECTRICO
    // =====================================
    if (!moving) {
      brake(motorLeft, motorRight);
      Serial.println("FRENO ACTIVO");
    }

    // Mostrar batería del control
    Serial.printf("Bateria PS4: %d\n", PS4.Battery());

    delay(40);  // estabilidad
  }
}
