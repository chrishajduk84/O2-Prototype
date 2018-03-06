#ifndef SENSORS_H
#define SENSORS_H

#include "O2-Cycler.h"
#include <math.h>

#define TEMP_1 0
#define TEMP_2 1
#define TEMP_3 2
#define RTD 3

typedef struct _cartridgeSensors{
  float pressure; //Absolute Gauge
  float temperature[4];
  float flow;
  float O2;
} CartridgeSensors;

class Sensors {
	unsigned int *pP_Abs, *pP_Gauge, *pTherm, *pFlow, *pTFlow, *pO2, *pO2Therm, *pHeaterCurrent;
  	CartridgeSensors csData;
  
	public:
	Sensors (int sensorIndex);
  	~Sensors();

	float getPressure();
	void printPressure();
	float getTemperature();
	void printTemperature();
	float getFlow();
	void printFlow();
	float getO2();
	void printO2();
  CartridgeSensors* getSensorData();
  void updateSensors();
};


#endif
