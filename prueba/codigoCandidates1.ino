#include <Servo.h>  //libreria servomotor

//---------------------------------Pines-----------------------------------------
// Motor izquierdo
int motor_i_adelante = 5;
int motor_i_atras = 6;

// Motor derecho
int motor_d_adelante = 3;
int motor_d_atras = 4;

// Ultrasonico adelante
#define echoPinDelantero 24
#define trigPinDelantero 25

// Ultrasonico derecha
#define echoPinDerecha 22
#define trigPinDerecha 23 

// Ultrasonico izquierda
#define echoPinIzquierda 27
#define trigPinIzquierda 26

// Ultrasonico general
int duration = 0;
int distanceDelantera = 0;
int distanceDerecha = 0;
int distanceIzquierda = 0;

// Sensor color
#define S0_PIN 37
#define S1_PIN 35
#define S2_PIN 39
#define S3_PIN 41
#define OUT_PIN 43
int rojo = 0;  
int verde = 0;  
int azul = 0; 

//LEDSSSS
#define ledVerde 46
#define ledAzul 44
#define ledRojo 42

// Servo
Servo servoBrazo; //objeto servomotor brazo
int servoBrazoPin = 8;
int posBrazo = 90;
Servo servoColor; //objeto servomotor color
int servoColorPin = 9;
int posColor = 0;

//--------------------------------Variables-----------------------------------------
int frecuenciaRojo = 0;
int frecuenciaVerde = 0;
int frecuenciaAzul = 0;
int colorRojo;
int colorVerde;
int colorAzul;
int tiempo = 100;

// variables para zona 1
int direccion = 3;
int movimiento = 0;
int posicion_x = 4;
int posicion_y = 2;
int contador = 0;
int x_encontrado = 1;
int colorx;
int meta = 12;
int matriz[5][3]; //Matriz con colores
boolean linea_izquierda = true;
boolean linea_adelante = false;
boolean linea_derecha = false;


// Unidad: representa cada cuadro de la pista en zona 1
struct Unidad{
 int movimientos[4] = {0,0,0,0};  //Serie de movimientos para llegar a esa unidad
 int x; //Posición x
 int y; //Posición y
 int numero; //Número en la matriz 4x3
 int color;
 int dir;
};

Unidad camino[50]; //Camino recorrido

Unidad crearUnidad(int a, int b, int c ,int d){
  Unidad celda;
   celda.movimientos[0] = a;
   celda.movimientos[1] = b;
   celda.movimientos[2] = c;
   celda.movimientos[3] = d;
   return celda;
}

int numero(int x, int y){
  int suma = (y*4) + (x+1);
  return suma;
  
}

// variables para zona 2
int color_zona2;
int color_cubo;

//Variables zona 3
int color_zona3;

//Zona en la que se encuentra
int zona = 1;


//---------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  Setup   <<<<<<<<<<<<<<<<<<<<<<<<---------------------------------------------------------
void setup() 
{
  Serial.begin(9600); 

  // Ultrasonicos
  pinMode(trigPinDelantero, OUTPUT);
  pinMode(echoPinDelantero, INPUT);
  pinMode(trigPinDerecha, OUTPUT);
  pinMode(echoPinDerecha, INPUT);
  pinMode(trigPinIzquierda, OUTPUT);
  pinMode(echoPinIzquierda, INPUT);

  // Motores
  pinMode(motor_i_adelante, OUTPUT);
  pinMode(motor_i_atras, OUTPUT);
  pinMode(motor_d_adelante, OUTPUT);
  pinMode(motor_d_atras, OUTPUT);

  //Leds
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  digitalWrite(ledRojo, LOW);
  delay(2000);
  digitalWrite(ledRojo, LOW);


  // sensor color
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(OUT_PIN, INPUT);
  digitalWrite(S0_PIN,HIGH);  
  digitalWrite(S1_PIN,HIGH); 

  digitalWrite(S0_PIN, HIGH);
  digitalWrite(S1_PIN, LOW);

  // servo
  servoBrazo.attach(servoBrazoPin);
  servoColor.attach(servoColorPin);
  servoBrazo.write(posBrazo);
  servoColor.write(posColor);

  

  //checkPoint(); //checar zona para lack of progress
//  0 = rojo
//  1 verde
//  2 azul
//  3 amarillo
//  4 morado
//  5 rosa
//  6 verde oscuro
//  7 blanco
  
  
  if (checarColor2() == 4 )
  { //Morado 
    zona = 3;
  }
  else if(checarColor2() == 5)
  { //Rosa (rampa)
    zona = 4;
  }
  else if(checarColor() == 0 || checarColor() == 1 || checarColor() == 2)
  {
    zona = 2;
  } else {
    zona = 1;
  }
  
  
}


//---------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  Loop   <<<<<<<<<<<<<<<<<<<<<<<<---------------------------------------------------------
void loop() 
{
  //Sensores ultrasónicos
  
  //Delantero
  digitalWrite(trigPinDelantero, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDelantero, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDelantero, LOW);
  int duration = 0;
  duration = pulseIn(echoPinDelantero, HIGH);
  int distanceDelantera = 0;
  distanceDelantera = duration * 0.034 / 2;

  //Derecha
  digitalWrite(trigPinDerecha, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDerecha, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDerecha, LOW);
  duration = pulseIn(echoPinDerecha, HIGH);
  int distanceDerecha = 0;
  distanceDerecha = duration * 0.034 / 2;

  //Izquierda
  digitalWrite(trigPinIzquierda, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinIzquierda, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinIzquierda, LOW);
  duration = pulseIn(echoPinIzquierda, HIGH);
  int distanceIzquierda = 0;
  distanceIzquierda = duration * 0.034 / 2;

  //------------------------------------------------------------------------Zona 1----------------------------------------------------------------------------------------
  // Para cuando esta en zona 1
  if (zona == 1)
  {

   //----------------------------------------------------------------------------------                                                                                      
    if (((posicion_x == 0) && (posicion_y == 0)) && (contador == 0))                   //Primera vez que llega al checkpoint
    {
          contador = contador + 1; //Indica que ya llegó al checkpoint
          meta = movimiento; //Indica el numero de movimientos para llegar al checkpoint


          //Voltea 180
          derecha(); 
          delay(500);
          
          direccion = direccion + 1;    
          if (direccion == 4)
            {
              direccion = 0;
            }
            
          atras();
          delay(500);
          corto_adelante();
          delay(500);
          derecha();
          delay(500);
          
          direccion = direccion + 1;
          if (direccion == 4)
            {
              direccion = 0;
            }
        
    }
    //----------------------------------------------------------------------------------                                                                                                                                                                              
    else if (((posicion_x == 4) && (posicion_y == 2)) && (contador == 1))                //Vuelve a llegar al inicio
    {
          //Se voltea 180
          derecha();
          delay(600);
          derecha();
          delay(600);
          atras();
          delay(500);
          corto_adelante();
          delay(500);       
          contador++;
          
          //Verifica la posición de la 'x'
          if(matriz[0][0] == matriz[0][2] && matriz[2][0] == matriz[0][0] && matriz[2][0] == matriz[2][2])
          {
                x_encontrado = 1;
                colorx = matriz[0][0];
          }
      
          else 
          {
                colorx = matriz[1][0];
          }

          //Realiza el camino corto (sin dead-ends)
          caminoCorto();
      
    }

   //----------------------------------------------------------------------------------                                                                 
      if (((posicion_x == 0) && (posicion_y == 0)) && (contador == 2))                   //Segunda vez en checkpoint (origen)
      {
          while(direccion !=2)
          {
            derecha();
            delay(500);
            direccion += 1;
            
            if(direccion == 4){
              direccion = 0;
            }
          }
          zona = 2;
      }

      //----------------------------------------------------------------Movimiento-----------------------------------------------------------------
      
      // 1 pared a la derecha y 0 adelante  Se mueve adelante
      if ((distanceDerecha <= 30) && (distanceDelantera > 30))
      {
           
        adelante();
        
        if (direccion == 0)
        {
          posicion_y = posicion_y + 1;
        }
        else if (direccion == 1)
        {
          posicion_x = posicion_x + 1;
        }
        else if (direccion == 2)
        {
          posicion_y = posicion_y - 1;
        }
        else if (direccion == 3)
        {
          posicion_x = posicion_x - 1;
        }   
        
        matriz[posicion_x][posicion_y] = checarColor();
        camino[movimiento] = crearUnidad(1,0,0,0);

        prenderLed(checarColor());
        
        camino[movimiento].numero = numero(posicion_x,posicion_y);
        camino[movimiento].x = posicion_x;
        camino[movimiento].y = posicion_y;
        camino[movimiento].dir = direccion;

        movimiento = movimiento + 1;
  
      
        delay(500);  
      }


    
      // 0 pared a la derecha y 1 adelante  Gira derecha y se mueve adelante-------
      else if((distanceDerecha > 30) && (distanceDelantera <= 30))
      {

        if(distanceIzquierda < 20)
        {
              derecha();
              delay(500);
              atras();
              delay(500);
              corto_adelante();
              delay(500);
    
              camino[movimiento] = crearUnidad(2,4,5,1);
        } 
        
        else
        {
              derecha();
              delay(500);
              camino[movimiento] = crearUnidad(2,1,0,0);
        }
  
        direccion = direccion + 1;
        if (direccion == 4)
        {
              direccion = 0;
        }
  
        delay(500);
        adelante();
        if (direccion == 0)
        {
          posicion_y = posicion_y + 1;
        }
        else if (direccion == 1)
        {
          posicion_x = posicion_x + 1;
        }
        else if (direccion == 2)
        {
          posicion_y = posicion_y - 1;
        }
        else if (direccion == 3)
        {
          posicion_x = posicion_x - 1;
        }
       
        
        matriz[posicion_x][posicion_y] = checarColor();
        prenderLed(checarColor());
        
        camino[movimiento].numero = numero(posicion_x,posicion_y);
        camino[movimiento].x = posicion_x;
        camino[movimiento].y = posicion_y;
        camino[movimiento].dir = direccion;

        movimiento = movimiento + 1;

        delay(500);
      }


    
      // 1 pared a la derecha y 1 adelante  Gira izquierda ---------------------
      else if((distanceDerecha <= 30) && (distanceDelantera <= 30))
      {
       
       
        camino[movimiento] = crearUnidad(3,4,5,0);

        camino[movimiento].numero = numero(posicion_x,posicion_y);
        camino[movimiento].x = posicion_x;
        camino[movimiento].y = posicion_y;
        
        izquierda();
        direccion = direccion - 1;
        
        if (direccion == -1)
        {
          direccion = 3;
        }

        camino[movimiento].dir = direccion;
        movimiento = movimiento + 1;

        delay(500);
        atras();
        delay(500);
        corto_adelante();
        delay(500);
      }


    
      // 0 pared a la derecha y 0 adelante  Gira derecha y se mueve adelante -----------------
      else if((distanceDerecha > 30) && (distanceDelantera > 30))
      {
         
        if(distanceIzquierda <= 30)
        {
              derecha();
              delay(500);
              atras();
              delay(500);
              corto_adelante();
              camino[movimiento] = crearUnidad(2,4,5,1);
        }        
        else 
        {
          derecha();
          camino[movimiento] = crearUnidad(2,1,0,0);
        }
  
        direccion = direccion + 1;
        if (direccion == 4)
        {
          direccion = 0;
        }
        
        delay(500);
        
        adelante();
        if (direccion == 0)
        {
          posicion_y = posicion_y + 1;
        }
        else if (direccion == 1)
        {
          posicion_x = posicion_x + 1;
        }
        else if (direccion == 2)
        {
          posicion_y = posicion_y - 1;
        }
        else if (direccion == 3)
        {
          posicion_x = posicion_x - 1;
        }

        camino[movimiento].numero = numero(posicion_x,posicion_y);
        camino[movimiento].x = posicion_x;
        camino[movimiento].y = posicion_y;
        camino[movimiento].dir = direccion;

        
        movimiento = movimiento +1;
        matriz[posicion_x][posicion_y] = checarColor();
        prenderLed(checarColor());
       
        delay(500);
      }
  }
  //---------------------------------------------------------------------------Zona 2------------------------------------------------------------------
  
  if (zona == 2)
  {
      adelante();
      delay(500);
      adelante();
      delay(500);
      adelante();
      delay(500);
      color_zona2 = checarColor();
      prenderLed(color_zona2);
      delay(500);
        
      posColor = 95;
      servoColor.write(posColor);
      delay(500);
  
      izquierda();
      delay(500);
      atras();
      delay(500);
      corto_adelante();
      delay(500);
      izquierda();
      delay(500);
      atras();
      delay(500);
      corto_adelante();
      delay(500);
        
      adelante();
      delay(500);
      
      adelante();
      delay(500);
        
      derecha();
      delay(500);
  
        
      atras();
      delay(500);
      corto_adelante();
      delay(500);
        
      delay(1000);
      corto_adelante();
      delay(500);
      posBrazo = 0;
      servoBrazo.write(posBrazo);
      delay(2000);
  
      atras();
      delay(500);
      corto_adelante();
      delay(500);
      adelante();
      delay(500);
  
      color_cubo = checarColor();
      prenderLed(checarColor());
    
      if (color_cubo == color_zona2) 
      {
        atras();
        delay(500);
        corto_adelante();
        delay(500);
        derecha();
        delay(500);
        adelante();
        delay(500);
        
        posBrazo = 90;
        servoBrazo.write(posBrazo);
        delay(2000);
        izquierda();
        delay(500);
        adelante();
        delay(500);
        adelante();
        delay(500);
        adelante();
        delay(500);
        posColor = 0;
        servoColor.write(posColor); 
        delay(500);
      }
      
      else 
      {
        adelante();
        delay(500);
        derecha();
        delay(500);
        atras();
        delay(500);
        corto_adelante();
        delay(500);
        adelante();
        delay(500);
        
        posBrazo = 90;
        servoBrazo.write(posBrazo);
        delay(2000);
        izquierda();
        delay(500);
        adelante();
        delay(500);
        posColor = 0;
        servoColor.write(posColor);
        delay(500); 
    }
  
  }
  //-----------------------------------------------------------------------------Zona 3-------------------------------------------------------------------------------
  if(zona == 3){
    
            posicion_x=0;
            posicion_y=0;
            direccion=1;
      int count = 0;
          if (count == 0){
      adelante_zona3();
      delay(tiempo);
      posicion_x=0;
      posicion_y=0;
      direccion=1;
      count++;
    }

    else if (posicion_x == 2 && posicion_y == 3){
      if (direccion == 1){
        izquierda();
        delay(tiempo);
        adelante_zona3();
        delay(tiempo);
        adelante_zona3();
        delay(tiempo);
      }
    }

    else if (posicion_x==2 && posicion_y==5){
      zona = 4;
    }

    digitalWrite(trigPinDelantero, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinDelantero, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinDelantero, LOW);
    int duration = 0;
    duration = pulseIn(echoPinDelantero, HIGH);
    int distanceDelantera = 0;
    distanceDelantera = duration * 0.034 / 2;
        
    digitalWrite(trigPinDerecha, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinDerecha, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinDerecha, LOW);
    duration = pulseIn(echoPinDerecha, HIGH);
    int distanceDerecha = 0;
    distanceDerecha = duration * 0.034 / 2;
    
    digitalWrite(trigPinIzquierda, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinIzquierda, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinIzquierda, LOW);
    duration = pulseIn(echoPinIzquierda, HIGH);
    int distanceIzquierda = 0;
    distanceIzquierda = duration * 0.034 / 2;
  
  // pared    der                  frente                izq
  //          si                       no                  no   
    if (distanceDerecha < 30  && distanceDelantera > 30  && distanceIzquierda > 30)
    {
      adelante_linea();
      delay(tiempo);
      izquierda();
      delay(tiempo);
      atras_zona3();
      delay(tiempo);
      corto_adelante();
      delay(tiempo);
      linea_izquierda = adelante_linea();    // variable para declarar
      delay(tiempo);
      
      if (linea_adelante == true && linea_izquierda == false)
      {
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }

      else if (linea_adelante == true && linea_izquierda == true)
      {
        izquierda();
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
  
      else  // hay linea en posicion 0 y no en  1
      {
        derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
    }
  
  // pared    der                  frente                izq
  //          no                       si                  no   
    else if (distanceDerecha > 30  && distanceDelantera < 30  && distanceIzquierda > 30)
    {
      izquierda(); if (direccion==0){direccion=3;} else if (direccion==1){direccion=0;} else if (direccion==2){direccion=1;}  else if (direccion==3){direccion=2;}
      delay(tiempo);
      linea_izquierda = adelante_linea();
      delay(tiempo);
      izquierda();
      delay(tiempo);
      atras_zona3();
      delay(tiempo);
      corto_adelante();
      delay(tiempo);
      izquierda();
      delay(tiempo);
      linea_derecha = adelante_linea();  // Variable para declarar
      delay(tiempo);
  
      if (linea_izquierda == true  && linea_derecha == true)
      {
        derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
        delay(tiempo);
        atras_zona3();
        delay(tiempo);
        corto_adelante();
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_izquierda == true && linea_derecha == false)
      {
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_izquierda == false && linea_derecha == true)
      {
        derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else
      {
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }    
    }
  
  // pared    der                  frente                izq
  //          no                       no                  si   
    else if (distanceDerecha > 30 && distanceDelantera > 30  && distanceIzquierda < 30)
    {
      linea_adelante = adelante_linea();
      delay(tiempo);
      derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
      delay(tiempo);
      linea_derecha = adelante_linea();
      delay(tiempo);
  
      if (linea_adelante == true && linea_derecha == true) //posible caso que nunca va a suceder
      {
        derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_adelante == true && linea_derecha == false)
      {
          atras_zona3();
          delay(tiempo);
          corto_adelante();
          delay(tiempo);
          adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
          delay(tiempo);
        }
        else if (linea_adelante == false  && linea_derecha == false)
        {
          adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
          delay(tiempo);  
        } 
        else 
        {
          izquierda();
          delay(tiempo); 
          adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
          delay(tiempo); 
        }
      }
  
  
  
  // pared    der                  frente                izq
  //          si                       si                  no   
    else if (distanceDerecha < 30  && distanceDelantera < 30  && distanceIzquierda > 30)
    {
      izquierda();
      delay(tiempo);
      atras_zona3();
      delay(tiempo);
      corto_adelante();
      delay(tiempo);
      linea_izquierda = adelante_linea();
      delay(tiempo);
  
      if (linea_izquierda == true)
      {
        izquierda();
        delay(tiempo);
        atras_zona3();
        delay(tiempo);
        corto_adelante();
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else
      {
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }  
    }
  
  // pared    der                  frente                izq
  //          no                       si                  si   
    else if (distanceDerecha > 30  && distanceDelantera < 30  && distanceIzquierda < 30)
    {
      derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
      delay(tiempo);
      atras_zona3();
      delay(tiempo);
      corto_adelante();
      delay(tiempo);
      linea_derecha = adelante_linea();
      delay(tiempo);
  
      if (linea_derecha == true)
      {
        derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
        delay(tiempo);
        atras_zona3();
        delay(tiempo);
        corto_adelante();
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else
      {
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
    }
  // pared    der                  frente                izq
  //          si                       si                  si   
    else if (distanceDerecha < 30  && distanceDelantera < 30  && distanceIzquierda < 30)
    {
      // el robot estara en la porteria
      derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
      delay(tiempo);
      atras_zona3();
      delay(tiempo);
      corto_adelante();
      delay(tiempo);
      derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
      delay(tiempo);
      atras_zona3();
      delay(tiempo);
      corto_adelante();
      delay(tiempo);
      adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
      delay(tiempo);
    }  
    else if (distanceDerecha > 30  && distanceDelantera > 30  && distanceIzquierda > 30) 
    {
      linea_adelante = adelante_linea();
      delay(tiempo);
      izquierda();
      delay(tiempo);
      linea_izquierda = adelante_linea();
      delay(tiempo);
      izquierda();
      delay(tiempo);
      izquierda();
      delay(tiempo);
      linea_derecha = adelante_linea();
      delay(tiempo);
      izquierda();
      delay(tiempo);
  
      if (linea_izquierda == true  && linea_adelante == true  && linea_derecha == true)
      {
        derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
        delay(tiempo);
        derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_izquierda == true  && linea_adelante == false  && linea_derecha == false)
      {
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_izquierda == false  && linea_adelante == true  && linea_derecha == false)
      {
        izquierda();
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_izquierda == false  && linea_adelante == false  && linea_derecha == true)
      {
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_izquierda == true  && linea_adelante == true  && linea_derecha == false)
      {
        derecha(); if (direccion==0){direccion=1;} else if (direccion==1){direccion=2;} else if (direccion==2){direccion=3;} else if (direccion==3){direccion=0;}
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_izquierda == true  && linea_adelante == false  && linea_derecha == true)
      {
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else if (linea_izquierda == false  && linea_adelante == true  && linea_derecha == true)
      {
        izquierda();
        delay(tiempo);
        adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
      else
      {
        izquierda();
        delay(tiempo);
       adelante_zona3(); if (direccion==0){posicion_y++;} else if (direccion==1){posicion_x++;} else if (direccion==2){posicion_y--;} else if (direccion==3){posicion_x--;}
        delay(tiempo);
      }
    }
   }    
  


  //-----------------------------------------------------------------------------Zona 4--------------------------------------------------------------------------------
  if(zona == 4){
    servoBrazo.write(90);
    servoColor.write(90);
      delay(500); 
    atras1();
    atras1();
    atras1();
    atras1();
    atras1();

  }
  
}


//---------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  Funciones   <<<<<<<<<<<<<<<<<<<<<<<<---------------------------------------------------------

// movimientos basicos del robot
void adelante() //-------------------------------------
{
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(500);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
}

void atras() //-------------------------------------
{
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(800);
  
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);

  delay(100);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  delay(500);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  
  delay(100);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(500);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
}

void atras1() //-------------------------------------
{
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(5000);
 
}
void corto_atras() //-------------------------------------
{
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(70);

  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(1000);
 
}
//void izquierda()
//{
//  // Girar a la izquierda (predefinido)
//  digitalWrite(motor_i_adelante, LOW);
//  digitalWrite(motor_i_atras, HIGH);
//  digitalWrite(motor_d_adelante, HIGH);
//  digitalWrite(motor_d_atras, LOW);
//  delay(680); // Ajustar tiempo para establecer bien el giro
//  digitalWrite(motor_i_adelante, LOW);
//  digitalWrite(motor_i_atras, LOW);
//  digitalWrite(motor_d_adelante, LOW);
//  digitalWrite(motor_d_atras, LOW);
//}

void corto_izquierda() //-------------------------------------
{
  // Girar a la izquierda (predefinido)
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(100); // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
}

//void derecha()
//{
//  // Girar a la izquierda (predefinido)
//  digitalWrite(motor_i_adelante, HIGH);
//  digitalWrite(motor_i_atras, LOW);
//  digitalWrite(motor_d_adelante, LOW);
//  digitalWrite(motor_d_atras, HIGH);
//  delay(680);                               // Ajustar tiempo para establecer bien el giro
//  digitalWrite(motor_i_adelante, LOW);
//  digitalWrite(motor_i_atras, LOW);
//  digitalWrite(motor_d_adelante, LOW);
//  digitalWrite(motor_d_atras, LOW);
//}

void derecha() //-------------------------------------
{
  int tiempo_delays = 75;
  // Girar a la izquierda (predefinido)
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(tiempo_delays);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays);
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(tiempo_delays);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); 
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(tiempo_delays);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); 
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(tiempo_delays);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); 
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(tiempo_delays);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); 
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(tiempo_delays);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); 
}
void izquierda() //-------------------------------------
{
  int tiempo_delays = 75;
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays); // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
  delay(tiempo_delays);
}
void derecha_180() //-------------------------------------
{
  // Girar a la izquierda (predefinido)
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(1500);                               // Ajustar tiempo para establecer bien el giro
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
}

void corto_adelante() //-------------------------------------
{
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(170);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
}

void alto() //-------------------------------------
{
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
}

//int ultrasonico_adelante()
//{
  //digitalWrite(trigPinDelantero, LOW);
  //delayMicroseconds(2);
  //digitalWrite(trigPinDelantero, HIGH);
  //delayMicroseconds(10);
  //digitalWrite(trigPinDelantero, LOW);
  //int duration = 0;
  //duration = pulseIn(echoPinDelantero, HIGH);
  //int distanceDelantera = 0;
  //distanceDelantera = duration * 0.034 / 2;
  //Serial.println(distanceDelantera);
  //return distanceDelantera;
//}

// Ultrasonico derecha
//int ultrasonico_derecha()
//{
  //digitalWrite(trigPinDerecha, LOW);
  //delayMicroseconds(2);
  //digitalWrite(trigPinDerecha, HIGH);
  //delayMicroseconds(10);
  //digitalWrite(trigPinDerecha, LOW);
  //int duration = 0;
  //duration = pulseIn(echoPinDerecha, HIGH);
  //int distanceDerecha = 0;
  //distanceDerecha = duration * 0.034 / 2;
  //Serial.println(distanceDerecha);
  //return distanceDerecha;
//}

void prenderColor(){ //-------------------------------------
  //contador += 1;
      delay(3000);
      if (colorx == 0) 
      {
        digitalWrite(ledVerde, LOW);
        digitalWrite(ledAzul, LOW);
        digitalWrite(ledRojo, HIGH);
        delay(5000);
        digitalWrite(ledRojo, LOW);
      }
      else if (colorx == 1) 
      {
        digitalWrite(ledVerde, HIGH);
        digitalWrite(ledAzul, LOW);
        digitalWrite(ledRojo, LOW);
        delay(5000);
        digitalWrite(ledVerde, LOW);
      }
      else if (colorx == 2) 
      {
        digitalWrite(ledVerde, LOW);
        digitalWrite(ledAzul, HIGH);
        digitalWrite(ledRojo, LOW);
        delay(5000);
        digitalWrite(ledAzul, LOW);
      }
}

boolean adelante_linea(){
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  
  int contadorx = 0;
  boolean white;
  int color;
  
  while(contadorx <=15){
        delay(10);
        digitalWrite(motor_i_adelante, LOW);
        digitalWrite(motor_i_atras, LOW);
        digitalWrite(motor_d_adelante, LOW);
        digitalWrite(motor_d_atras, LOW);
        
        if(checarColor() == 7){
            delay(150);
            digitalWrite(motor_i_adelante, LOW);
            digitalWrite(motor_i_atras, HIGH);
            digitalWrite(motor_d_adelante, LOW);
            digitalWrite(motor_d_atras, HIGH);
          return true;
        }
          delay(10);
          digitalWrite(motor_i_adelante, HIGH);
          digitalWrite(motor_i_atras, LOW);
          digitalWrite(motor_d_adelante, HIGH);
          digitalWrite(motor_d_atras, LOW);
          contadorx++;
  }

    digitalWrite(motor_i_adelante, LOW);
    digitalWrite(motor_i_atras, LOW);
    digitalWrite(motor_d_adelante, LOW);
    digitalWrite(motor_d_atras, LOW);
    delay(150);
    digitalWrite(motor_i_adelante, LOW);
    digitalWrite(motor_i_atras, HIGH);
    digitalWrite(motor_d_adelante, LOW);
    digitalWrite(motor_d_atras, HIGH);
    return false;
}



void caminoLargo() { //---------------------------------------------------------- Camino largo
      delay(1000);
      prenderLed(0);
        prenderLed(1);
          prenderLed(2);
      
      for(int c = 0; c < movimiento; c++){
          for (int i = 0; i < 4; i++){
                if(camino[c].movimientos[i]  == 1){
                  adelante();
                  delay(1000);
                }else if(camino[c].movimientos[i] == 2){
                  derecha();
                  delay(1000);
      
                }else if(camino[c].movimientos[i] == 3){
                  izquierda();  
                  delay(1000);
      
                } else if(camino[c].movimientos[i]  == 4){
                  atras();
                  delay(1000);
      
                } else if(camino[c].movimientos[i]  == 5){
                  corto_adelante();
                  delay(1000);
      
                }
                if(camino[c].x == x_encontrado && camino[c].y == 1){                       
                                prenderColor();
                }
          }
      
      }
}

void caminoCorto() {   //------------------------------------------------------- Camino corto
  
  delay(1000);
  prenderLed(0);
    prenderLed(1);
      prenderLed(2);
     
  //adelante();
  delay(500);
  boolean found = false;
  int checador = 0;
  int indice = -1;
  int vez = 0;

  //Primer for para recorrer las unidades del camino recorrido hasta el checkpoint
  for(int c = 0; c < meta; c++){
    
        //Segundo for para revisar si en el recorrido se llegó a la misma unidad (dead end)
        for(int j = c+1; j < meta; j++){
                if(camino[c].numero == camino[j].numero && j-1 > 2){ //En caso de que si, el nuevo 
                     indice = j;                                     //indice sera j para iniciar después del dead end
                } 
        } 
                //For para revisar cada movimiento realizado para llegar a la unidad y repetirlo
                for (int i = 0; i < 4; i++){
         
                            if(camino[c].movimientos[i]  == 1){
                              adelante();
                              delay(1000);
                            }else if(camino[c].movimientos[i] == 2){
                              derecha();
                              delay(1000);
                  
                            }else if(camino[c].movimientos[i] == 3){
                              izquierda();  
                              delay(1000);
                  
                            } else if(camino[c].movimientos[i]  == 4){
                              atras();
                              delay(1000);
                  
                            } else if(camino[c].movimientos[i]  == 5){
                              corto_adelante();
                              delay(1000);
                  
                            }
                            if(camino[c].x == x_encontrado && camino[c].y == 1 && vez == 0){
                              vez++;
                          prenderColor();
                        }
                   }

        //Si el índice cambió, se debe confirmar que el robot inicie en la misma posición después del dead end
        if (indice != -1){

                  if(camino[c].dir - camino[indice].dir == 1 || camino[c].dir - camino[indice].dir == -3 ){
                    izquierda();
                    delay(500);
                  } else if (camino[c].dir - camino[indice].dir == 2 || camino[c].dir - camino[indice].dir == -2){
                    derecha();
                    delay(500);
                    derecha();
                    delay(500);
                  } else if (camino[c].dir - camino[indice].dir == 3 || camino[c].dir - camino[indice].dir == -1){
                    derecha();
                    delay(500);
                  }
                 
                  camino[indice].movimientos[0]=0;
                  camino[indice].movimientos[1]=0;
                  camino[indice].movimientos[2]=0;
                  camino[indice].movimientos[3]=0;                   
          c = indice-1; // c será el índice al volver a iniciar un nuevo ciclo
          indice = -1;
        } 

        if(camino[c].numero == 1){ //Si llega al checkpoint
          break;
        }
  }
      
    delay(1000);
  prenderLed(0);
    prenderLed(1);
      prenderLed(2);
}


int checarColor() //-------------------------------------Para laberinto
{
  
  digitalWrite(S2_PIN, LOW);  
  digitalWrite(S3_PIN, LOW);   
  rojo = pulseIn(OUT_PIN, digitalRead(OUT_PIN) == HIGH ? LOW : HIGH);  
  digitalWrite(S3_PIN, HIGH);   
  azul = pulseIn(OUT_PIN, digitalRead(OUT_PIN) == HIGH ? LOW : HIGH);  
  digitalWrite(S2_PIN, HIGH);    
  verde = pulseIn(OUT_PIN, digitalRead(OUT_PIN) == HIGH ? LOW : HIGH);  

  Serial.print("   ");  
  Serial.print(rojo, DEC);  
  Serial.print("   ");  
  Serial.print(verde, DEC);  
  Serial.print("   ");  
  Serial.print(azul, DEC);  

  if (rojo < azul && verde > azul && rojo < 35) 
  {  
   return 0;   
  }   
  else if (azul < rojo && azul < verde && verde < rojo)  
  {  
   return 2;       
  }  

  else if (rojo > verde && azul > verde )  
  {  
   return 1;       
  }  
  else {
    if(zona == 1){
    corto_atras();
    checarColor();
    }
    return 0; 
  }
  delay(900);     
  }  

int checarColor2(){ //-------------------------------------Para checkpoints
  //0 = rojo
  //1 verde
  //2 azul
  //3 amarillo
  //4 morado
  //5 rosa
  //6 verde oscuro
  //7 blanco

  int color=0;
  int r, g, b;
  r = process_red_value();
  delay(200);
  g = process_green_value();
  delay(200);
  b = process_blue_value();
  delay(200);


        if (r > 10 && r < 30 && g > 25 && g < 45 && b > 40 && b < 79){
          return 3; //amarillo
        }
        else if (r > 80 && r < 150 && g > 100 && g < 230 && b > 45 && b <110){
          return 4; //morado
        }
        
        else if (r > 5 && r < 25 && g > 35 && g < 78 && b > 20 && b < 40){
          return 5; //rosa
        }
        else if (r > 15 && r < 35 && g > 15 && g <35 && b > 10 && b <30){
          return 7; //blanco
        }
        else {
             return -1; 
        }


        
      return 3;

}

int process_red_value() //-------------------------------------
{
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, LOW);
  int pulse_length = pulseIn(OUT_PIN, LOW);
  return pulse_length;
}

int process_green_value() //-------------------------------------
{
  digitalWrite(S2_PIN, HIGH);
  digitalWrite(S3_PIN, HIGH);
  int pulse_length = pulseIn(OUT_PIN, LOW);
  return pulse_length;
}

int process_blue_value() //-------------------------------------
{
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, HIGH);
  int pulse_length = pulseIn(OUT_PIN, LOW);
  return pulse_length;
}

void prenderLed(int color) //-------------------------------------
{
  delay(500);
  if(color == 0)
  {
    digitalWrite(ledRojo, HIGH);
  }
  else if (color == 1)
  {
    digitalWrite(ledVerde, HIGH);
  }
  else if (color == 2)
  {
    digitalWrite(ledAzul, HIGH);
  }
  delay(500);
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAzul, LOW);
}

void adelante_zona3() //-------------------------------------
{
  digitalWrite(motor_i_adelante, HIGH);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, HIGH);
  digitalWrite(motor_d_atras, LOW);
  delay(200);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
}

void atras_zona3() //-------------------------------------
{
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay(200);
  
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);

  delay(100);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, HIGH);
  delay( tiempo);
  digitalWrite(motor_i_adelante, LOW);
  digitalWrite(motor_i_atras, LOW);
  
  delay(100);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, HIGH);
  delay( tiempo);
  digitalWrite(motor_d_adelante, LOW);
  digitalWrite(motor_d_atras, LOW);
}
