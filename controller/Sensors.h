#ifndef SENSORS_H
#define SENSORS_H

#include "Pinout.h"
#include "UART.h"

#define TEMP_1 0
#define TEMP_2 1
#define TEMP_3 2
#define RTD 3

typedef struct _deviceStatus{
	bool adc0 = false;
	bool adc1 = false;
	bool oxygen = false;
	
} DeviceStatus;

typedef struct _cartridgeSensors{
  float pressure; //Absolute Gauge
  float temperature[4];
  float flow;
  float O2;
} CartridgeSensors;

class Sensors {
	DeviceStatus sensorStatus;
	PeripheralLocation *pPressure, *pTemperature1, *pTemperature2, *pTemperature3, *pRTD, *pFlow, *pO2;
  	CartridgeSensors csData;
	const static int N = 10; 
    int fd0, fd1;

	UART oxygenSensor;
	 
	public:
	Sensors (int sensorIndex);
  	~Sensors();

	float getPressure();
	void printPressure();
	float getTemperature(int index);
	void printTemperature(int index);
	float getFlow();
	void printFlow();
	float getO2();
	void printO2();
  	CartridgeSensors* getSensorData();
  	void updateSensors();
};


#endif
