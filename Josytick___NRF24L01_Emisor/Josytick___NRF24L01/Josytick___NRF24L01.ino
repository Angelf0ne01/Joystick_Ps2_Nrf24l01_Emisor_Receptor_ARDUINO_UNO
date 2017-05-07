#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include <PS2X_lib.h>
PS2X ps2x;

RF24 radio(9, 10);                       
///////////////////////////////////////////////////////////////////////////////////////////////////////
//VARIABLES
///////////////////////////////////////////////////////////////////////////////////////////////////////
const uint64_t canal = 0xE8E8F0F0E1LL;    // Usamos este canal
char Dato[1];                                                  
int flag, Botones[14],error,puntero;                            
byte type = 0;                                                  
int cambio_de_estado[14];
int Botones_Send_Numero[14] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};    
String Botones_Send_String[14] = {"UP","DOWN","LEFT","RIGHT","O","X","CUADRADO","TRIANGULO","L1","L2","R1","R2","START","SELECT"};

///////////////////////////////////////////////////////////////////////////////////////////////////////
//DEFINES
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define up        0
#define down      1
#define left      2
#define right     3
#define circulo   4
#define equis     5
#define cuadrado  6
#define triangulo 7
#define l1        8
#define l2        9
#define r1        10
#define r2        11
#define start     12
#define select    13

#define cambio_de_estado_n  15
///////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES
///////////////////////////////////////////////////////////////////////////////////////////////////////
   /*********************************************************************************************
   **Con esta funcion lo que hago es leer el estado de todos los botones que posee el joystick **
   **Si alguno de los botones es apretado su estado sera guardado en el array "Botones"        **
   **********************************************************************************************/
void Lectura_de_datos() {   
   for (puntero=0; puntero<14;puntero++) Botones[puntero]=0;//Reseto los datos de Botones.
   ps2x.read_gamepad();                                     //Leeo los estados de los botones del joystick.
  //LECTURA DE LOS BOTONES IZQUIERDOS ("ARRIBA",ABAJO","IZQUIERDA","DERECHA").
  if (ps2x.Button(PSB_PAD_UP))   Botones[up] =       1;           
  if (ps2x.Button(PSB_PAD_DOWN)) Botones[down] =     1;         
  if (ps2x.Button(PSB_PAD_LEFT)) Botones[left] =     1;         
  if (ps2x.Button(PSB_PAD_RIGHT))Botones[right] =    1;         
  //LECTURA DE LOS BOTONES DERECHOS ("CIRCULO","EQUIS","CUADRADO","TRIANGULO").
  if (ps2x.Button(PSB_RED))      Botones[circulo] =   1;
  if (ps2x.Button(PSB_BLUE))     Botones[equis] =     1;
  if (ps2x.Button(PSB_PINK))     Botones[cuadrado] =  1;
  if (ps2x.Button(PSB_GREEN))    Botones[triangulo] = 1;
  //LECTURA DE LOS BOTONES R1-R2 Y L1-L2
  if (ps2x.Button(PSB_L1))       Botones[l1] =      1;
  if (ps2x.Button(PSB_L2))       Botones[l2] =      1;
  if (ps2x.Button(PSB_R1))       Botones[r1] =      1;
  if (ps2x.Button(PSB_R2))       Botones[r2] =      1;
 //LECTURA DE LOS BOTONES "START" y 
  if (ps2x.Button(PSB_START))    Botones[start] =   1;
  if (ps2x.Button(PSB_SELECT))   Botones[select] =  1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void Envio_de_datos(){
  for(puntero=0;puntero<14;puntero++){  
     if(Botones[puntero] == 1){                         //Pregunto que botones han sido pulsados. Si es asi ...
      //  Serial.println(Botones_Send_String[puntero]);   //Muestro en pantalla que boton fue apretado.
        Dato[0]=Botones_Send_Numero[puntero];           //Guardo en dato el numero del boton que fue apretado. Ejemplo "UP" corresponde al nuermo 1, "DOWN" al Numero 2.. etc.
        radio.write(Dato,1);                            //Envio el dato.
     }                                                  //Cierre del if                                                    
    if(cambio_de_estado[puntero]!= Botones[puntero]){    //Pregunto si se apreto o se dejo de apretar algun boton.
        Serial.println("cambio de estado");              //Muestro en pantalla que se esta ejecutando el cambio de estado.
        Dato[0]=cambio_de_estado_n;                      //Guardo en la variable dato  el valor de cambio de estado (15).
        radio.write(Dato,1);                             //envio un dato indicando que hubo un cambio de estado en los botones que estan pulsados.
    }
    cambio_de_estado[puntero]=Botones[puntero];          //Actualizo la variable con los nuevos valores de los botones que se apretaron o dejaron de apretar.
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//CONFIGURACIONES
///////////////////////////////////////////////////////////////////////////////////////////////////////
  void setup() {
    Serial.begin(9600);                                   //Inicio la transmicion Serie.
    radio.begin();                                        //Inicio la transmicion RF.
    radio.openWritingPipe(canal);                         //Configuro el canal.
    error == ps2x.config_gamepad(4, 6, 5, 7, true, true); //Configuracion de puertos (clock,command,attention,data,pressure,rumble)
    if (error == 0) Serial.println("Configurado");        //Si no hay error, muestro en pantalla que esta configurado.
    type = ps2x.readType();                               //Verifico si hay comunicacion entrel el joystick y el arduino.
    if ( type == 1) Serial.println("Conectado.");         //Muestro en pantalla que el Joystick esta conectado.
  
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////
//PROGRAMA PRINCIPAL
///////////////////////////////////////////////////////////////////////////////////////////////////////
  void loop(){
      while (error!=1) {                                    //Si error es igual a 1 Significa que el joystick no esta conectado con el arduino.
        Lectura_de_datos();                                 //Leo los datos del joystick
        Envio_de_datos();                                   //Los envio por el RF
      }                                                    
 }                                                         

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

  
