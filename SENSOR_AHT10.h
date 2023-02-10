/*!

Los derechos de autor del código de ejemplo del sensor son:

	Autor: fmawic@gmail.com
	Escrito por Fran "fmawic" M. Alvarez Wic for Sensoryca.

*/
#ifndef AHT10_H
#define AHT10_H

#include <Arduino.h>
#include <stdint.h>

/**

Aquí definimos la clase ATH10
Con todos sus metodos

*/


typedef enum {
  AHT10_default = 0x38,
  AHT10_Low     = 0x38,
  AHT10_High    = 0x39,
} HUM_SENSOR_T;


class AHT10_Clase
{
  public:
    AHT10_Clase();

    float ObtenerHumedad(void);
    float ObtenerTemperatura(void);
    float ObtenerPuntoDeRocio(void);
    unsigned char LeerEstado(void);
    void Reset(void);
    bool begin(unsigned char _AHT10_dirc = AHT10_default);

  private:
    unsigned long LeerSensor(bool GetDataCmd);
    unsigned char AHT10_direc;

};

#endif
