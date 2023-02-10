/*!

Los derechos de autor del código de ejemplo del sensor c02 son para:

	Escrito por  C�novas Espinal, Antonio

*/


#include "Arduino.h"
#include "SENSOR_CO2.h"

/**
 analogPin: pin analógico en el microcontrolador donde se conecta el sensor de CO2
 Constructor de la clase CO2 que almacena el pin analógico en la estructura de datos "dataCO2".
 Esta función debe ser llamada en la declaración de la instancia para inicializar el pin del sensor.
*/
CO2::CO2(uint8_t analogPin) {
  this->dataCO2.pin = analogPin;
}
/**
 Inicializa el pin del sensor de CO2 configurándolo como una entrada en el microcontrolador.
 Esta función debe ser llamada después de la declaración de la instancia para preparar el sensor para la lectura.
*/
void CO2::begin() {
  pinMode(dataCO2.pin, INPUT);
}

/**
 Devuelve la tensión leída por el microcontrolador.
 Esta función permite realizar lecturas analógicas del sensor de CO2 y devolver los resultados en términos de tensión.
 El número de muestras y los intervalos entre ellas se definen en el archivo CO2.h. 
 Es necesario pasar el resultado de esta función a la función percentage para calcular la concentración de CO2 en el aire.
*/
float CO2::leer() {
   int i;
  float v = 0;

  for (i = 0; i < TIMES_DE_MUESTREO_LECTURA ; i++) {
    v += analogRead(dataCO2.pin);
    delay(INTERVALO_DE_MUESTREO_LECTURA );
  }
  v = (v / TIMES_DE_MUESTREO_LECTURA ) * 5 / 1024 ;
  return v;
}

/**
	Voltaj: voltaje de la señal del sensor de CO2
	curva de calibración para el sensor de CO2
	Devuelve el porcentaje de concentración de CO2 en el aire en formato entero, o -1 si el voltaje es mayor o igual al voltaje de punto cero.
    Calcula la concentración de CO2 en el aire utilizando el voltaje de la señal y la curva de calibración.
*/
int32_t CO2::porcentaje(float voltaj, float *curva) {
  if ((voltaj / DC_GAIN ) >= ZERO_POINT_VOLTAGE) {
    return -1;
  }
  else {
    return pow(10, ((voltaj / DC_GAIN) - curva[1]) / curva[2] + curva[0]);
  }
}
