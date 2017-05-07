#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

RF24 radio(9,10);

///////////////////////////////////////////////////////////////////////////////////////////////////////
//VARIABLES
///////////////////////////////////////////////////////////////////////////////////////////////////////
const uint64_t canal = 0xE8E8F0F0E1LL;
//Defino los puertos A0 hasta A1 como salidas para los motores-
int m1_1_izq = A0;  
int m1_2_izq = A1;
int m2_1_der = A2;
int m2_2_der = A3;
//Defino las salidas digitales 3 y 6 Como salidas de pwm.
int pwm_m_izq = 3;
int pwm_m_der = 6;  
//Defino las velocidades  
int pwm_duty_max = 255;       //Maxima velocidad.
int pwm_duty_medium= 70;      //Velocidad media.
int pwm_duty_off = 0;         //Sin velocidad.

int puntero,dato[16];
int botones_recividos[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
String Botones_print[14] = {"UP","DOWN","LEFT","RIGHT","CIRUCULO","EQUIS","CUADRADO","TRIANGULO","L1","L2","R1","R2","START","SELECT"};
///////////////////////////////////////////////////////////////////////////////////////////////////////
//DEFINES
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define UP        0
#define DOWN      1
#define LEFT      2
#define RIGHT     3
#define CIRCULO   4
#define EQUIS     5
#define CUADRADO  6
#define TRIANGULO 7
#define L1        8
#define L2        9
#define R1        10
#define R2        11
#define START     12
#define SELECT    13
#define cambio_de_estado 14
///////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES
///////////////////////////////////////////////////////////////////////////////////////////////////////
void Lectura_de_datos(){    
  if(radio.available()){                                                                              //Pregunto si se recibio algun dato
     int done = radio.read(dato, 1);                                                                  //Leo el valor recivido
     dato[0]-=1;                                                                                      //tiene un error de +1
     if(dato[0]==cambio_de_estado) for(puntero=0;puntero<14;puntero++) botones_recividos[puntero]=0;  //Si hubo un cambio de estado en el joystick reseto los valores de "Botones_recividos"       
     if(dato[0]==0)  botones_recividos[UP]=1; 
     if(dato[0]==1)  botones_recividos[DOWN]=2; 
     if(dato[0]==2)  botones_recividos[LEFT]=3; 
     if(dato[0]==3)  botones_recividos[RIGHT]=4;
     if(dato[0]==4)  botones_recividos[CIRCULO]=5; 
     if(dato[0]==5)  botones_recividos[EQUIS]=6; 
     if(dato[0]==6)  botones_recividos[CUADRADO]=7; 
     if(dato[0]==7)  botones_recividos[TRIANGULO]=8;
     if(dato[0]==8)  botones_recividos[L1]=9; 
     if(dato[0]==9)  botones_recividos[L2]=10;
     if(dato[0]==10) botones_recividos[R1]=11;
     if(dato[0]==11) botones_recividos[R2]=12; 
     if(dato[0]==12) botones_recividos[START]=13;
     if(dato[0]==13) botones_recividos[SELECT]=14;
     Serial.println(botones_recividos[UP]);
     Serial.println(botones_recividos[R1]);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void Avanzar(){
    Sentido_de_giro_adelante_M();
    analogWrite(pwm_m_izq,pwm_duty_max);
    analogWrite(pwm_m_der,pwm_duty_max);       
    Serial.println("AVANZANDO");   
  }
void Retroceder(){
    Sentido_de_giro_atras_M();
    analogWrite(pwm_m_izq,pwm_duty_max);
    analogWrite(pwm_m_der,pwm_duty_max);       
    Serial.println("RETROCEDIENDO");
}
void Giro_izq(){
    digitalWrite(m1_1_izq,HIGH);
    digitalWrite(m1_2_izq,LOW);
    digitalWrite(m2_1_der,LOW);
    digitalWrite(m2_2_der,HIGH);
    analogWrite(pwm_m_izq,pwm_duty_max);
    analogWrite(pwm_m_der,pwm_duty_max);
    Serial.println("GIRANDO IZQ");    
}
void Giro_der(){
    digitalWrite(m1_1_izq,LOW);
    digitalWrite(m1_2_izq,HIGH);
    digitalWrite(m2_1_der,HIGH);
    digitalWrite(m2_2_der,LOW);
    analogWrite(pwm_m_izq,pwm_duty_max);
    analogWrite(pwm_m_der,pwm_duty_max);
    Serial.println("GIRANDO DER");
    }

void Avanzar_giro_izq(){
    Sentido_de_giro_adelante_M();
    analogWrite(pwm_m_izq,pwm_duty_medium);
    analogWrite(pwm_m_der,pwm_duty_max);
    Serial.println("AVANZANDO IZQ");
    }

void Avanzar_giro_der(){
    Sentido_de_giro_adelante_M();
    analogWrite(pwm_m_izq,pwm_duty_max);
    analogWrite(pwm_m_der,pwm_duty_medium);
    Serial.println("AVANZANDO DER");
}    
void Retroceder_giro_izq(){
    Sentido_de_giro_atras_M();
    analogWrite(pwm_m_izq,pwm_duty_medium);
    analogWrite(pwm_m_der,pwm_duty_max);
    Serial.println("RETROCEDIENDO IZQ");
    }
void Retroceder_giro_der(){
    Sentido_de_giro_atras_M();
    analogWrite(pwm_m_izq,pwm_duty_max);
    analogWrite(pwm_m_der,pwm_duty_medium);
    Serial.println("RETROCEDIENDO DER");
    }   
void Sentido_de_giro_adelante_M(){
    digitalWrite(m1_1_izq,HIGH);
    digitalWrite(m1_2_izq,LOW);
    digitalWrite(m2_1_der,HIGH);
    digitalWrite(m2_2_der,LOW);
    } 
void Sentido_de_giro_atras_M(){       
    digitalWrite(m1_1_izq,LOW);
    digitalWrite(m1_2_izq,HIGH);
    digitalWrite(m2_1_der,LOW);
    digitalWrite(m2_2_der,HIGH);
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Decision_de_movimientos(){
    if(botones_recividos[R1]==11){                                                                      //Solo se movera si se apreta "R1"
        if((botones_recividos[UP]==1) && (botones_recividos[DOWN]==0) && (botones_recividos[LEFT]==0) && (botones_recividos[RIGHT]==0)) Avanzar();
        if((botones_recividos[UP]==0) && (botones_recividos[DOWN]==2) && (botones_recividos[LEFT]==0) && (botones_recividos[RIGHT]==0)) Retroceder();  
        if((botones_recividos[UP]==0) && (botones_recividos[DOWN]==0) && (botones_recividos[LEFT]==3) && (botones_recividos[RIGHT]==0)) Giro_izq();
        if((botones_recividos[UP]==0) && (botones_recividos[DOWN]==0) && (botones_recividos[LEFT]==0) && (botones_recividos[RIGHT]==4)) Giro_der();
        if((botones_recividos[UP]==1) && (botones_recividos[DOWN]==0) && (botones_recividos[LEFT]==3) && (botones_recividos[RIGHT]==0)) Avanzar_giro_izq();
        if((botones_recividos[UP]==1) && (botones_recividos[DOWN]==0) && (botones_recividos[LEFT]==0) && (botones_recividos[RIGHT]==4)) Avanzar_giro_der();
        if((botones_recividos[UP]==0) && (botones_recividos[DOWN]==2) && (botones_recividos[LEFT]==3) && (botones_recividos[RIGHT]==0)) Retroceder_giro_izq();
        if((botones_recividos[UP]==0) && (botones_recividos[DOWN]==2) && (botones_recividos[LEFT]==0) && (botones_recividos[RIGHT]==4)) Retroceder_giro_der();
      }else {
       analogWrite(pwm_m_izq,pwm_duty_off);
       analogWrite(pwm_m_der,pwm_duty_off);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//CONFIGURACIONES
///////////////////////////////////////////////////////////////////////////////////////////////////////    
void setup(void){
   Serial.begin(9600);                  //Inicio la comunicacion Serial.
   radio.begin();                       //Inicio el modulo RF
   radio.openReadingPipe(1,canal);      //Configuro el canal
   radio.startListening();              //Configuro como receptor

   //CONFIGURO TODO COMO SALIDA
   pinMode(m1_1_izq,OUTPUT);
   pinMode(m1_2_izq,OUTPUT);
   pinMode(m2_1_der,OUTPUT);
   pinMode(m2_2_der,OUTPUT);
   pinMode(pwm_m_izq,OUTPUT);
   pinMode(pwm_m_der,OUTPUT);    
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//PROGRAMA PRINCIPAL
///////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void){
  Lectura_de_datos();                   //Leo los datos que llegan del modulo RF
  Decision_de_movimientos();            //Decido el movimiento
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

