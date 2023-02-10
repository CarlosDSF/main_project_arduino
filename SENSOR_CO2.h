/*!

Los derechos de autor del código de ejemplo del sensor c02 son para:

	Escrito por  C�novas Espinal, Antonio

*/

#ifndef CO2_H_
#define CO2_H_
#include <Arduino.h>

#define VCC_CO2 3.37
#define RESOLUTION 1024

#define DC_GAIN (8.5)   //definir la ganancia DC del amplificador
#define ZERO_POINT_VOLTAGE	(VCC_CO2/DC_GAIN) //definir la salida del sensor en voltios cuando la concentración de CO2 es de 400PPM.
#define REACTION_VOLTGAE	(0.030) //definir la disminución de voltaje del sensor cuando se mueve el sensor del aire a 1000ppm de CO2.
#define INTERVALO_DE_MUESTREO_LECTURA 	(50)    //definir el intervalo de muestras
#define TIMES_DE_MUESTREO_LECTURA	(5)     //definir el intervalo de tiempo (en milisegundos) entre cada muestra.

typedef struct dataCO2 {
  int percent;
	float volts;
	uint8_t pin;
	float  curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};
/*
se toman dos puntos de la curva.
con estos dos puntos, se forma una línea que es
"aproximadamente equivalente" a la curva original.
formato de datos: {x, y, pendiente}; punto 1: (lg400, 0.324), punto 2: (lg4000, 0.280)
pendiente = (voltaje de reacción) / (log400 - log1000)


*/
} data_CO2;

class CO2 {
  public:
    data_CO2 dataCO2;
    CO2(uint8_t analogPin);	
    void begin();
		int32_t porcentaje(float voltaj, float *curva);
		float leer();

};

#endif
