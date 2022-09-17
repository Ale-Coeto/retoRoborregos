// Motor izquierdo
int motor_i_adelante = 2;
int motor_i_atras = 3;

// Motor derecho
int motor_d_adelante = 4;
int motor_d_atras = 5;

// Ultrasonico delantero
#define echoPinDelantero 22
#define trigPinDelantero 23

// Ultrasonioc derecha
#define echoPinDerecha 24
#define trigPinDerecha 25

//encoder
unsigned long time_old;
unsigned int pulses_per_turn = 20;
volatile byte pulses;
unsigned int rpm;
int encoder = 50;
int pase = 0;

void setup() 
{
  // Ultrasonicos
  pinMode(trigPinDelantero, OUTPUT);
  pinMode(echoPinDelantero, INPUT);
  pinMode(trigPinDerecha, OUTPUT);
  pinMode(echoPinDerecha, INPUT);

  // Motores
  pinMode(motor_i_adelante, OUTPUT);
  pinMode(motor_i_atras, OUTPUT);
  pinMode(motor_d_adelante, OUTPUT);
  pinMode(motor_d_atras, OUTPUT);

  // Encoder
  pinMode(encoder, INPUT)
  attachInterrupt(digityalPinToInterrupt(encoder), count, Falling);
  
}

void loop() 
{
  // 1 pared a la derecha y 0 adelante  Se mueve adelante
  if(((ultrasonico_derecha >=10) AND (ultrasonico_derecha <= 20)) AND (ultrasonico_adelante > 30))
  {
    adelante();
    delay(1000);  
  }

  // 0 pared a la derecha y 1 adelante  Gira derecha y se mueve adelante
  if(((ultrasonico_adelante >=10) AND (ultrasonico_adelante <= 20)) AND (ultrasonico_derecha > 30))
  {
    derecha();
    delay(1000);
  }

  // 1 pared a la derecha y 1 adelante  Gira izquierda y se mueve adelante 
  if(((ultrasonico_adelante >=10) AND (ultrasonico_adelante <= 20)) AND ((ultrasonico_derecha >= 30) AND (ultrasonico_derecha <= 20)))
  {
    izquierda();
    delay(1000);
  }

  // 0 pared a la derecha y 0 adelante  Gira derecha y se mueve adelante 
  if(((ultrasonico_adelante >=10) AND (ultrasonico_adelante <= 20)) AND (ultrasonico_derecha > 30))
  {
    derecha();
    delay(1000);
  }


// Ultrasonico adelante
void ultrasonico_adelante()
{
  digitalWrite(trigPinDelantero, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDelantero, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDelantero, LOW);
  duration = pulseIn(echoPinDelantero, HIGH);
  distanceDelantera = duration * 0.034 / 2;
  return(distanceDelantera);
}

// Ultrasonico derecha
void ultrasonico_derecha()
  {
    digitalWrite(trigPinDerecha, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinDerecha, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinDerecha, LOW);
    duration = pulseIn(echoPinDerecha, HIGH);
    distanceDerecha = duration * 0.034 / 2;
    return(distanceDerecha);
  }

// Contador del encoder
void count()
  {
    pulses++;
  }

//Movimientos básicos del robot
void adelante()
{
  while(pase <= 30)
  {
    digitalWrite(motor_i_adelante, HIGH);
    digitalWrite(motor_i_atras, LOW);
    digitalWrite(motor_d_adelante, HIGH);
    digitalWrite(motor_d_atras, LOW);

    if(millis()-time_old >=50)
      {
        detachInterrupt(digitalPinToInterrupt(encoder_pin1));
        rpm = (60 * 100 / pulses_per_turn )/ (millis() - time_old)* pulses;
        time_old=millis();
        pase = pulses + pase;
        pulses = 0;
        attachInterrupt(digitalPinToInterrupt(encoder_pin1), count, FALLING ); // Triggering count function everytime the encoder pin1 turns from 1 to 0
      }

    digitalWrite(motor_i_adelante, LOW);
    digitalWrite(motor_i_atras, LOW);
    digitalWrite(motor_d_adelante, LOW);
    digitalWrite(motor_d_atras, LOW);
    pase = 0;
  }
}

void atras()
{
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);

  if(millis()-time_old >=50)
      {
        detachInterrupt(digitalPinToInterrupt(encoder_pin1));
        rpm = (60 * 100 / pulses_per_turn )/ (millis() - time_old)* pulses;
        time_old=millis();
        pase = pulses + pase;
        pulses = 0;
        attachInterrupt(digitalPinToInterrupt(encoder_pin1), count, FALLING ); // Triggering count function everytime the encoder pin1 turns from 1 to 0
      }

  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  pase = 0;
}

void izquierda()
{
  // Girar a la izquierda (predefinido)
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(500);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);

// Se mueve adelante 1 unidad
  while(pase <= 30)
  {
    digitalWrite(motor_i_adelante, HIGH);
    digitalWrite(motor_i_atras, LOW);
    digitalWrite(motor_d_adelante, HIGH);
    digitalWrite(motor_d_atras, LOW);
    

    if(millis()-time_old >=50)
      {
        detachInterrupt(digitalPinToInterrupt(encoder_pin1));
        rpm = (60 * 100 / pulses_per_turn )/ (millis() - time_old)* pulses;
        time_old=millis();
        pase = pulses + pase;
        pulses = 0;
        attachInterrupt(digitalPinToInterrupt(encoder_pin1), count, FALLING ); // Triggering count function everytime the encoder pin1 turns from 1 to 0
      }

    digitalWrite(motor_i_adelante, LOW);
    digitalWrite(motor_i_atras, LOW);
    digitalWrite(motor_d_adelante, LOW);
    digitalWrite(motor_d_atras, LOW);
    pase = 0;
  }
}

void derecha()
{
  // Girar a la izquierda (predefinido)
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(500);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);

    while(pase <= 30)
  {
    digitalWrite(motor_i_adelante, HIGH);
    digitalWrite(motor_i_atras, LOW);
    digitalWrite(motor_d_adelante, HIGH);
    digitalWrite(motor_d_atras, LOW);
    

    if(millis()-time_old >=50)
      {
        detachInterrupt(digitalPinToInterrupt(encoder_pin1));
        rpm = (60 * 100 / pulses_per_turn )/ (millis() - time_old)* pulses;
        time_old=millis();
        pase = pulses + pase;
        pulses = 0;
        attachInterrupt(digitalPinToInterrupt(encoder_pin1), count, FALLING ); // Triggering count function everytime the encoder pin1 turns from 1 to 0
      }

    digitalWrite(motor_i_adelante, LOW);
    digitalWrite(motor_i_atras, LOW);
    digitalWrite(motor_d_adelante, LOW);
    digitalWrite(motor_d_atras, LOW);
    pase = 0;
  }
}
