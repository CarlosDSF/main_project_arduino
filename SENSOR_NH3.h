/*!

Los derechos de autor del código de ejemplo del sensor NH3 son para:

	Escrito por  C�novas Espinal, Antonio

*/
#ifndef NH3_H_
#define NH3_H_
#include <Arduino.h>

#define VCC 3.307
#define RESOLUTION 1024

/*
Este código define una clase llamada "NH3" y un tipo de estructura llamado "data_NH3". 
La estructura "data_NH3" contiene los campos "m", "b", "pin", "R0", "Rl" y "ppm". La clase "NH3" 
tiene una variable miembro de estructura llamada "dataNH3", que es una instancia de "data_NH3".

La clase "NH3" tiene un constructor que toma cinco argumentos: "analogPin", "m", "b", "R0" y "Rl". 
Este constructor asigna los valores de estos argumentos a los campos correspondientes en "dataNH3".

La clase "NH3" tiene dos funciones públicas: "begin()" y "leer()". La función "begin()" 
configura el pin especificado como una entrada para lectura. La función "leer()" lee la tensión en el pin y 
realiza cálculos para calcular las partes por millón (ppm) de amoníaco. La función "leer()" 
devuelve un double que representa las ppm.;
*/
 


typedef struct dataNH3 {
  float m;
	float b;
	uint8_t pin;
	float R0;
	float Rl;
	double ppm;
} data_NH3;

class NH3 {
  public:
    data_NH3 dataNH3;
    NH3(uint8_t analogPin, float m, float b, float R0, float Rl);	
    void begin();
		double leer();
};

#endif
