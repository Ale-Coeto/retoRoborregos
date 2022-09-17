// Motor izquierdo
int motor_i_adelante = 2;
int motor_i_atras = 3;

// Motor derecho
int motor_d_adelante = 4;
int motor_d_atras = 5;

// Ultrasonico delantero
#define echoPinDelantero 
#define trigPinDelantero 

// Ultrasonioc derecha
#define echoPinDerecha 
#define trigPinDerecha 

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

}

// Contador del encoder
void count()
  {
    pulses++;
  }

void adelante()
{
  while(pase <= 20)
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
}

void izquierda()
{
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
}

void derecha()
{
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
}
