/*!

Los derechos de autor del código de ejemplo del sensor AHT10 son para:

	Autor: fmawic@gmail.com
	Escrito por Fran "fmawic" M. Alvarez Wic for Sensoryca.

*/
#include "SENSOR_AHT10.h"
#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>
#include <math.h>


#define PUNTO_ROCIO 17.62f //  En este caso, 17.62f es el valor de la constante del punto de rocio
#define PRESION_BAROME 243.5f // 243.5f es el valor de la constante para calcular la presión barométrica.

// Datos de Array
///< Calibration command -> 0xE1 
///< Trigger reading command -> 0xAC
///< Soft reset command -> 0xBA
///< Status bit for calibrated -> 0x08

uint8_t eSensorCalibrateCmd[3] = {0xE1, 0x08, 0x00};
uint8_t eSensorNormalCmd[3]    = {0xA8, 0x00, 0x00};
uint8_t eSensorMeasureCmd[3]   = {0xAC, 0x33, 0x00};
uint8_t SignalResetCmd         = 0xBA;
bool    ObtenerHumedadCmd      = true;
bool    ObtenerTemperaturaCmd  = false;

uint8_t status;
uint8_t error_code;

/******************************************************************************
   Constructor
 ******************************************************************************/
AHT10_Clase::AHT10_Clase()
{
  status = true;
  error_code = false;
}


/*
 Esta función "begin" inicializa el sensor de humedad y temperatura AHT10 mediante el protocolo I2C. 
 Toma como parámetro la dirección del dispositivo en la red I2C. 
 La función espera 20 milisegundos para permitir que el sensor se encierre. 
 Luego, se establece la dirección del dispositivo, se inicializa la comunicación I2C y 
 se envía un comando de reinicio al dispositivo. Se espera otro 20 milisegundos para permitir que el dispositivo reinicie. 
 Luego se envía un comando de calibración al dispositivo y se espera 20 milisegundos. Finalmente, 
 se solicita un byte de información del dispositivo y se comprueba si el dispositivo está funcionando correctamente. 
 La función devuelve un valor booleano indicando si el dispositivo se inicializó correctamente.

*/

bool AHT10_Clase::begin(unsigned char _AHT10_dirc)
{
  uint8_t tmp = 0x00;
  delay (20); 
  AHT10_direc = _AHT10_dirc;
  Wire.begin();
  Wire.beginTransmission(AHT10_direc);
  Wire.write(SignalResetCmd);
  delay(20);
  Wire.write(eSensorCalibrateCmd, 3);
  delay(20);
  Wire.requestFrom(AHT10_direc, 1);
  tmp = Wire.read();
  Wire.endTransmission();
  
  if ((tmp & 0x68) == 0x08){
    return true;
  }
  else
  {
    return false;
  }

}


/*
 Obtiene la humedad actual del sensor.
 Y retorna la humedad relativa en %HR.
*/
float AHT10_Clase::ObtenerHumedad(void)
{
  float aux = LeerSensor(ObtenerHumedadCmd);
  if (aux == 0) {
    return 0;                       // Error
  }
  return aux * 100 / 1048576;
}

/*
 Obtiene la temperatura actual del sensor.
 Retorna la temperatura en grados Celsius.
*/
float AHT10_Clase::ObtenerTemperatura(void)
{
  float aux = LeerSensor(ObtenerTemperaturaCmd);
  return ((200 * aux) / 1048576) - 50;
}

/*
 Obtiene el punto de rocío actual basado en la humedad y temperatura actuales.
 Retorna el punto de rocío en grados Celsius.
*/
float AHT10_Clase::ObtenerPuntoDeRocio(void)
{
  float temperatura = ObtenerTemperatura();
  float humedad = ObtenerHumedad();

  float aux_0 = log(humedad / 100) + PUNTO_ROCIO * temperatura / (PRESION_BAROME + temperatura);
  float aux = PRESION_BAROME * aux_0 / (PUNTO_ROCIO - aux_0);

  return aux;
}


/****
 Esta función lee los datos del sensor AHT10 mediante el protocolo I2C. 
 Toma como parámetro un valor booleano que indica si se deben obtener los datos de la temperatura o de la humedad. 
 La función inicia una transmisión I2C con la dirección del dispositivo y envía un comando para medir los datos del sensor.
 Luego, se espera 100 milisegundos para permitir que el dispositivo complete la medición. 
 Se solicitan 6 bytes de información del dispositivo y se almacenan en un arreglo temporal. 
 Si se ha indicado obtener los datos de la temperatura, se combinan los bytes 1, 2 y 3 del arreglo temporal para obtener el valor de la temperatura. 
 En caso contrario, se combinan los bytes 3, 4 y 5 del arreglo temporal para obtener el valor de la humedad.
 Finalmente, la función devuelve el valor obtenido.
**/

unsigned long AHT10_Clase::LeerSensor(bool GetDataCmd)
{
  unsigned long aux, temp[6];

  Wire.beginTransmission(AHT10_direc);
  Wire.write(eSensorMeasureCmd, 3);
  Wire.endTransmission();
  delay(100);

  Wire.requestFrom(AHT10_direc, 6);

  for (unsigned char i = 0; Wire.available() > 0; i++)
  {
    temp[i] = Wire.read();
  }

  if (GetDataCmd)
  {
    aux = ((temp[1] << 16) | (temp[2] << 8) | temp[3]) >> 4;
  }
  else
  {
    aux = ((temp[3] & 0x0F) << 16) | (temp[4] << 8) | temp[5];
  }

  return aux;
}
/**
 Esta función se encarga de leer el estado del sensor AHT10 mediante el protocolo I2C. 
 Utiliza la librería Wire para comunicarse con el dispositivo a través del bus I2C. 
 La función solicita un byte de información del dispositivo a través de la dirección especificada en la variable AHT10_direc. 
 El byte leído se almacena en una variable temporal y se devuelve como resultado de la función.
 El byte leído contiene información sobre el estado del dispositivo, 
 por ejemplo si está listo para tomar una medición o si se encuentra en un estado de error.

*/

unsigned char AHT10_Clase::LeerEstado(void)
{
  unsigned char aux = 0;

  Wire.requestFrom(AHT10_direc, 1);
  aux = Wire.read();
  return aux;
}

/**

 Esta función se encarga de resetear el sensor AHT10 mediante el protocolo I2C. 
 Utiliza la librería Wire para comunicarse con el dispositivo a través del bus I2C. 
 La función inicia una transmisión a través del bus I2C utilizando la dirección especificada en la variable AHT10_direc. 
 Envía un comando de reset (SignalResetCmd) al dispositivo. 
 Luego finaliza la transmisión y agrega un retraso de 20 milisegundos para permitir que el dispositivo reinicie correctamente.
 Este comando es utilizado para reiniciar el sensor, se reinicia la configuración.


*/


void AHT10_Clase::Reset(void)
{
  Wire.beginTransmission(AHT10_direc);
  Wire.write(SignalResetCmd);
  Wire.endTransmission();
  delay(20);
}
