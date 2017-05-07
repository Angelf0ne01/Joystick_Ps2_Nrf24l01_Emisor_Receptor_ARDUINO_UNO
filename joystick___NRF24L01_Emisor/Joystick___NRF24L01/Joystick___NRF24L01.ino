#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include <PS2X_lib.h>
PS2X ps2x;


char msg[16] = "Prometec.net.\n" ;                           // Array a transmitir
RF24 radio(9, 10);                       // Creamos un objeto radio del tipo RF2$
const uint64_t pipe = 0xE8E8F0F0E1LL;    // Usamos este canal

int flag;
byte type = 0;
int error;
int Botones[14];
int cambio_de_estado[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Botones_Send_N[14] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};

String Botones_Send[14] = {
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




///////////////////////////////////////////////////////////////////////////////////////////////////////
//CONFIGURACIONES
///////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);

  //                  clock,command,attention,data,pressure,rumble
  //                  data,comand cloack,attention,
  error == ps2x.config_gamepad(4, 6, 5, 7, true, true);
  if (error == 0)  Serial.println("Configurado");
  type = ps2x.readType();
  if ( type == 1)  Serial.println("Conectado.");

}

void loop()
{
  if (error == 1) {
    Serial.println("NO CONECTADO");
    return; //Salir si no encontramos mando
  }

  while (true) {
    ps2x.read_gamepad();          //Leemos el mando
    for (int x = 0; x < 15; x++) Botones[x] = 0;
    Lectura_de_datos();
    Envio_de_datos();
    

  }
  delay(500);
}

void Lectura_de_datos() {
  if (ps2x.Button(PSB_PAD_UP))   Botones[0] = 1; 
  if (ps2x.Button(PSB_PAD_DOWN)) Botones[1] = 1;
  if (ps2x.Button(PSB_PAD_LEFT)) Botones[2] = 1;
  if (ps2x.Button(PSB_PAD_RIGHT))Botones[3] = 1;

  if (ps2x.Button(PSB_RED))  Botones[4] = 1;
  if (ps2x.Button(PSB_BLUE)) Botones[5] = 1;
  if (ps2x.Button(PSB_PINK)) Botones[6] = 1;
  if (ps2x.Button(PSB_GREEN))Botones[7] = 1;

  if (ps2x.Button(PSB_L1)) Botones[8] = 1;
  if (ps2x.Button(PSB_L2)) Botones[9] = 1;
  if (ps2x.Button(PSB_R1)) Botones[10] = 1;
  if (ps2x.Button(PSB_R2)) Botones[11] = 1;

  if (ps2x.Button(PSB_START))  Botones[12] = 1;
  if (ps2x.Button(PSB_SELECT)) Botones[13] = 1;
}


void Envio_de_datos(){
  for(int x=0;x<14;x++){  
      if(Botones[x] == 1){                  //Pregunto que boton a sido apretado
        Serial.println(Botones_Send[x]);    //Escribo en pantalla el bonton que se ah apretado
        msg[0]=Botones_Send_N[x];           //
        radio.write(msg,1);                 //Envio el dato
        }
        
    for(int x=0;x<14;x++){
      if(cambio_de_estado[x]!= Botones[x]){  //Pregunto si se a apretado un nuevo estado o si se solto un boton
        msg[0]=15;                                  //El "15" significa que huebo un cambio de estado;
        flag=msg[0];
        Serial.println(flag);
        radio.write(msg,1);                         //envio el dato
    }
    cambio_de_estado[x]=Botones[x];          //Actualizo los valores de "cambio_de_estado" 
    }
  }
}

  
