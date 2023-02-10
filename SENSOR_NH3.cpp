/*!

Los derechos de autor del código de ejemplo del sensor NH3 son para:

	Escrito por  C�novas Espinal, Antonio

*/


#include "Arduino.h"
#include "SENSOR_NH3.h"


NH3::NH3(uint8_t analogPin, float m, float b, float R0, float Rl) {
  this->dataNH3.pin = analogPin;
	this->dataNH3.m = m;
	this->dataNH3.b = b;
	this->dataNH3.R0 = R0;
	this->dataNH3.Rl = Rl;
}

/*
La función begin() simplemente establece el pin de entrada analógica en modo
 INPUT para que el microcontrolador pueda realizar lecturas.
*/
void NH3::begin() {
	pinMode(dataNH3.pin, INPUT);
}

/*
La función leer() realiza la lectura analógica en el pin y usa la información para calcular la concentración de amoníaco
 en términos de partes por millón (ppm). Esto se hace a través de un cálculo de resistencia, 
 en el que se usan los valores de tensión leídos para calcular la resistencia del gas (RS_gas), 
 luego se usa el valor de la resistencia para calcular una relación (ratio) con respecto a una resistencia de referencia (R0). 
Finalmente, se usa una fórmula para convertir la relación en ppm. La función devuelve el resultado en forma de double.

*/
double NH3::leer() {
  int sensorValue = analogRead(dataNH3.pin);
	float sensor_volt = (float)sensorValue * (VCC / RESOLUTION);
	float RS_gas = ((VCC * dataNH3.Rl) / sensor_volt) - dataNH3.Rl;
	float ratio = RS_gas / dataNH3.R0;
	double ppm_log = (log10(ratio) - dataNH3.b) / dataNH3.m; 
  double ppm = pow(10, ppm_log);
	return ppm;
}
