/*  ----------------------------------------------------------------
    http://www.prometec.net/nrf2401
    Prog_79_1_Emisor
    
    Programa para recibir strings mediante radios NRF2401
--------------------------------------------------------------------  
*/

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

//Defino el sentido de los motores
int m1_1_izq = A0;
int m1_2_izq = A1;
int m2_1_der = A2;
int m2_2_der = A3;

int pwm_m_izq = 3;
int pwm_m_der = 6;


#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4
#define O     5
#define X     6
#define CUADRADO 7
#define TRIANGULO 8
#define L1        9
#define L2        10
#define R1        11
#define R2        12
#define START     13
#define SELECT    14


int acelerar=0;
int msg[16];
int flag=1;
int errorr[14];
int botones_recividos[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
String Botones_print[14] = {
  "UP",
  "DOWN",
  "LEFT",
  "RIGHT",

  "O",
  "X",
  "CUADRADO",
  "TRIANGULO",

  "L1",
  "L2",
  "R1",
  "R2",

  "START",
  "SELECT"
};



RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void){
 pinMode(m1_1_izq,OUTPUT);
 pinMode(m1_2_izq,OUTPUT);
 pinMode(m2_1_der,OUTPUT);
 pinMode(m2_2_der,OUTPUT);
 pinMode(pwm_m_izq,OUTPUT);
 pinMode(pwm_m_der,OUTPUT);

  
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}
 
void loop(void){
Lectura_de_datos();
Decision_de_movimientos();

}

void Lectura_de_datos(){    
  if(radio.available()){
    int done = radio.read(msg, 1); 
    if(msg[0]==15){
      for(int x=0;x<14;x++) botones_recividos[x]=0;
    }
    if(msg[0]==0)  botones_recividos[0]=1; //UP
    if(msg[0]==1)  botones_recividos[1]=2; //DOWN
    if(msg[0]==2)  botones_recividos[2]=3; //LEFT
    if(msg[0]==3)  botones_recividos[3]=4; //RIGH
    if(msg[0]==4)  botones_recividos[4]=5; //O
    if(msg[0]==5)  botones_recividos[5]=6; //X
    if(msg[0]==6)  botones_recividos[6]=7; //Cuadrado
    if(msg[0]==7)  botones_recividos[7]=8; //triangulo
    if(msg[0]==8)  botones_recividos[8]=9; //L1
    if(msg[0]==9)  botones_recividos[9]=10; //l2
    if(msg[0]==10)  botones_recividos[10]=11; //R1
    if(msg[0]==11)  botones_recividos[11]=12; //R2
    if(msg[0]==12)  botones_recividos[12]=13; //Start
    if(msg[0]==13)  botones_recividos[13]=14; //Select
    Serial.println(botones_recividos[UP]);
    Serial.println(botones_recividos[R1]);
}else for(int x=0;x<14;x++) botones_recividos[x]=0;
}
void Decision_de_movimientos(){
  if(botones_recividos[R1]==12){
    Avanzar();
    Retroceder();  
    Giro_izq();
    Giro_der();
    Avanzar_giro_izq();
    Avanzar_giro_der();
    }
    else {
       analogWrite(pwm_m_izq,0);
     analogWrite(pwm_m_der,0);
      }
}
void Avanzar(){
  
   if((botones_recividos[UP]==2) && (botones_recividos[DOWN]==0) && (botones_recividos[LEFT]==0) && (botones_recividos[RIGHT]==0)){
       Sentido_de_giro_adelante_M();
       analogWrite(pwm_m_izq,255);
       analogWrite(pwm_m_der,255);       
      Serial.println("Avanzando");
   }
  }

  
void Retroceder(){
  if((botones_recividos[UP]==0) && (botones_recividos[DOWN]==3) && (botones_recividos[LEFT]==0) && (botones_recividos[RIGHT]==0)){
       Sentido_de_giro_atras_M();
       analogWrite(pwm_m_izq,255);
       analogWrite(pwm_m_der,255);       
      Serial.println("RETROCEDIENDO");
   }
  }

void Giro_izq(){
if((botones_recividos[UP]==0) && (botones_recividos[DOWN]==0) && (botones_recividos[LEFT]==4) && (botones_recividos[RIGHT]==0)){
       digitalWrite(m1_1_izq,HIGH);
       digitalWrite(m1_2_izq,LOW);
       digitalWrite(m2_1_der,LOW);
       digitalWrite(m2_2_der,HIGH);
      analogWrite(pwm_m_izq,255);
      analogWrite(pwm_m_der,255);
      Serial.println("Girando Izq Sobre el mismo eje");
    }
}
void Giro_der(){
  if((botones_recividos[UP]==0) && (botones_recividos[DOWN]==0) && (botones_recividos[LEFT]==0) && (botones_recividos[RIGHT]==5)){
       digitalWrite(m1_1_izq,LOW);
       digitalWrite(m1_2_izq,HIGH);
       digitalWrite(m2_1_der,HIGH);
       digitalWrite(m2_2_der,LOW);
      analogWrite(pwm_m_izq,255);
      analogWrite(pwm_m_der,255);
      Serial.println("Girando Der Sobre el mismo eje");
    }
}

void Avanzar_giro_izq(){
  if((botones_recividos[LEFT]==4)&&(botones_recividos[UP]==2)){
      Sentido_de_giro_adelante_M();
      analogWrite(pwm_m_izq,70);
      analogWrite(pwm_m_der,255);
      Serial.println("Avanzando a la izq");
    }
}

void Avanzar_giro_der(){
  if((botones_recividos[RIGHT]==5) && (botones_recividos[UP]==2)){
      Sentido_de_giro_adelante_M();
      analogWrite(pwm_m_izq,255);
      analogWrite(pwm_m_der,70);
      Serial.println("Avanzando a la der");
    }
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
