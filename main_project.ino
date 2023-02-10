/****************************************
Se declara en primera instancia las inclusiones 
de los demás sensores: 
- AHT10 
- NH3 
- CO2 
*****************************************/
#include "SENSOR_CO2.h"
#include "SENSOR_NH3.h"
#include "SENSOR_AHT10.h"

#include "LoRaWAN.h" // Libreria de LoraWan
#include <Wire.h>
#define BANDA EU868 // Define la banda de frecuencia utilizada 868 MHz 

#define SN 0x0B  /* SN Identificador del despositivo, para saber quien envia*/
#define tiempo_minutos 1 /* Número de minutos que se envia los datos */
#define tiempo_envio (tiempo_minutos *60 *1000) /* Conversión de los números de minutos */

// Para mayor exactitud se hace una media de muestras para que sea más correcto el valor
#define CANT_MUESTRAS 25 




/*
NH3 es una estructura de datos en C++ que representa una molécula de amoníaco. 
Los valores que se le pasan a la estructura (A1, -0.30865, 0.4923, 1.23, 10.00) 
son los parámetros necesarios para calcular las propiedades termodinámicas de la molécula de amoníaco. 
A1 es el coeficiente para el estado gaseoso, -0.30865 es el entalpía de combustión, 
0.4923 es el calor específico, 1.23 es la densidad
y 10.00 es la presión de vapor.
*/

NH3 NH3(A1, -0.30865, 0.4923, 1.23, 10.00);

// Declaraciones de variables y clases

CO2 CO2(A0);
AHT10_Clase aht;
float temperatura = 0x00, humedad = 0x00;


/*
Estos valores son identificadores únicos utilizados en el protocolo LoRaWAN (Long Range Wide Area Network), 
que es un estándar de comunicación inalámbrica para IoT. 
El "appEui" es el identificador único de la aplicación, 
El "appKey" es la clave de la aplicación y 
El "devEui" es el identificador único del dispositivo. 
Estos valores son necesarios para establecer una conexión segura y autenticada entre el dispositivo y la red LoRaWAN.
*/

  const char *appEui  = "70B3D57ED0041D2A";
  const char *appKey  = "472A6E045ED9791019ABBAE4451B10D9";
  const char *devEui  = "00F91B72C6AE42ED";
  /**/


/*
 Esta función inicializa el dispositivo de sensores con el siguiente proceso:
 Inicializa la comunicación serial para fines de depuración a una velocidad de 115200 baudios.
 Inicializa los sensores y el sensor AHT10 para medir la temperatura y la humedad. 
 Si se encuentra este último sensor inicializado correctamente, se imprime un mensaje en el puerto serial. Si no se encuentra, se imprime un mensaje de error.
 Inicializa las comunicaciones LoRaWAN.
*/

void setup() {
	
  Serial.begin(115200);


  NH3.begin();
  CO2.begin();
  
  if (aht.begin(AHT10_Low) == true or aht.begin(AHT10_High) == true)
  {
    Serial.println ("AHT10: OK" );
  }
  else
  {
    Serial.println("Error");
  }

  connectLoRaWAN();
}
/*
 La función loop inicializa y muestra los datos de los sensores de amoníaco,
 dióxido de carbono, temperatura y humedad. Los datos se muestran a través
 del puerto serie y se envían mediante LoRaWAN. Luego, la función espera un tiempo 
 determinado antes de repetir el proceso.
*/

void loop() {

  LecturaNH3();
  LecturaCO2();
  LecturaAHT();
  EnviarLorawan();
  delay(tiempo_envio);

}
/*
 La función "LecturaNH3" lee los valores de concentración de amoníaco (NH3) y 
 los almacena en una estructura de datos "NH3.dataNH3.ppm". 
 La cantidad de muestras que se tomarán se puede variar con la constante "CANT_MUESTRAS". 
 Se suman los valores leídos y se divide el total por el número de muestras para obtener una media.
*/
void LecturaNH3() {
  NH3.dataNH3.ppm = 0;
  float aux = 0;
  for (uint32_t con = 0; con < CANT_MUESTRAS; con++) {
	  
    aux = NH3.leer();
	
    NH3.dataNH3.ppm = NH3.dataNH3.ppm + aux;
  }
  NH3.dataNH3.ppm = NH3.dataNH3.ppm / CANT_MUESTRAS;
}
/*
 La función "LecturaCO2" es similar, pero lee los valores de concentración de dióxido de carbono (CO2) y 
 los almacena en una estructura de datos "CO2.dataCO2.volts". También se divide el total de los valores 
 leídos por el número de muestras para obtener una media. Además, se llama a una función "CO2.porcentaje" 
 para calcular la concentración de CO2 en términos de porcentaje.
*/
void LecturaCO2() {
  CO2.dataCO2.volts = 0;
  float aux = 0;
  for (uint32_t con = 0; con < CANT_MUESTRAS; con++) {
    aux = CO2.leer();
    CO2.dataCO2.volts = CO2.dataCO2.volts + aux;
  }
  
  CO2.dataCO2.volts = CO2.dataCO2.volts / CANT_MUESTRAS;
  CO2.dataCO2.percent = CO2.porcentaje(CO2.dataCO2.volts, CO2.dataCO2.curve);
}

/*
   La función LecturaAHT se encarga de leer la humedad y la temperatura del sensor AHT10.
*/

  void LecturaAHT()
  {
	temperatura = aht.ObtenerTemperatura();
	humedad = aht.ObtenerHumedad();
  }

/*
 la siguiente función "connectLoRaWAN"inicializa un objeto LoRaWAN y se conecta a una red LoRaWAN. 
 La función hace las siguientes acciones:
 Inicializa el objeto LoRaWAN usando la banda especificada.
 Agrega un canal LoRaWAN con frecuencia 868300000 Hz y configuración de canal 0 y anchura de banda de 6.
 Realiza una conexión OTAA (Over-The-Air Activation) a la red LoRaWAN usando los valores especificados para appEui, appKey y devEui como identificadores de la aplicación y el dispositivo.

*/
void connectLoRaWAN () {
  LoRaWAN.begin(BANDA);
  LoRaWAN.addChannel(1, 868300000, 0, 6);
  LoRaWAN.joinOTAA(appEui, appKey, devEui);
}

/**
 La funciíon "f2b". Una unión es un tipo de datos que permite almacenar diferentes tipos de datos en la misma posición de memoria. 
 En este caso, la unión "f2b" contiene dos campos:
 -> "fbyte", que es un tipo "float" (número de punto flotante con precisión simple).
 -> "bbyte", que es un tipo "byte" (arreglo de tamaño 3 de enteros sin signo de 8 bits).
 Esta unión permite acceder y manipular los mismos bytes de memoria como un número de punto flotante o como un arreglo de bytes.
 Esto puede ser útil para la conversión de tipos de datos, la manipulación de bytes en la transmisión de datos, entre otros usos. 
 Sin embargo, tenga en cuenta que la unión puede ser un concepto difícil de usar y que puede haber limitaciones y 
 consideraciones de plataforma que deben tenerse en cuenta.
*/


union f2b {
  float fbyte;
  byte bbyte[3];
};


/**

La función "EnviarLorawan"convierte varios valores de medición (NH3, CO2, temperatura y humedad) a tipos "float", 
y luego los almacena en "uniones" "f2b". 
Luego, la función utiliza la biblioteca "LoRaWAN" para enviar los datos convertidos a través de una red LoRaWAN.

En detalle, la función hace lo siguiente:

 Declara 4 variables de unión "f2b": "aux_n", "aux_c", "aux_t" y "aux_h".
 Asigna a cada unión un valor float que representa una medición (NH3.dataNH3.ppm, CO2.dataCO2.percent, temperatura y humedad).
 Inicia un paquete de transmisión con "LoRaWAN.beginPacket()".
 Escribe en el paquete el ID de dispositivo SN con "LoRaWAN.write(SN)".
 Escribe en el paquete los bytes de cada unión con "LoRaWAN.write(n.bbyte[0])", "LoRaWAN.write(n.bbyte[1])", etc.
 Finaliza y envía el paquete con "LoRaWAN.endPacket()".

*/


void EnviarLorawan() {
  f2b aux_n, aux_c, aux_t, aux_h, aux_p, aux_a;
  aux_n.fbyte = (float)NH3.dataNH3.ppm;
  aux_c.fbyte = CO2.dataCO2.percent;
  aux_t.fbyte = temperatura;
  aux_h.fbyte = humedad;
  LoRaWAN.beginPacket();
  LoRaWAN.write(SN);
  // ENVIAMOS NH3
  LoRaWAN.write(aux_n.bbyte[0]);  LoRaWAN.write(aux_n.bbyte[1]);   LoRaWAN.write(aux_n.bbyte[2]);  LoRaWAN.write(aux_n.bbyte[3]);
  // ENVIAMOS CO2
  LoRaWAN.write(aux_c.bbyte[0]);  LoRaWAN.write(aux_c.bbyte[1]);   LoRaWAN.write(aux_c.bbyte[2]);   LoRaWAN.write(aux_c.bbyte[3]);
  // ENVIAMOS TEMPERATURA
  LoRaWAN.write(aux_t.bbyte[0]);  LoRaWAN.write(aux_t.bbyte[1]);   LoRaWAN.write(aux_t.bbyte[2]);   LoRaWAN.write(aux_t.bbyte[3]);
  // ENVIAMOS HUMEDAD
  LoRaWAN.write(aux_h.bbyte[0]);  LoRaWAN.write(aux_h.bbyte[1]);   LoRaWAN.write(aux_h.bbyte[2]);   LoRaWAN.write(aux_h.bbyte[3]);
  
  LoRaWAN.endPacket();
}
