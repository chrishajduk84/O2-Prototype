#include "Sensors.h"
#include <wiringPiI2C.h>
#include <math.h>

Sensors::Sensors(int sensorIndex){
	//Connect to I2C peripherals
	
	//ADC0 - Verify Connection
	
	//ADC1 - Verify Connection - This should happen in a separate file/class/interface
/*
	//Assign pointers
	pPressure = &(pressureLocation[sensorIndex]);
	pTemperature1 = &(temperatureLocation[0][sensorIndex]);
	pTemperature2 = &(temperatureLocation[1][sensorIndex]);
	pTemperature3 = &(temperatureLocation[2][sensorIndex]);
	pRTD = &(rtdLocation[sensorIndex]);
	pFlow = &(flowLocation[sensorIndex]);
*/		
	//Oxygen determine via UART
	//SOME CODE TO CONNECT TO UART

	for (int i = 0; i < 4; i++){
  		csData.temperature[i] = 30;//thermocouple->readCelsius();       //First value for rolling avg. [to prevent dividing by 0]
	}
}

Sensors::~Sensors(){}

float Sensors::getPressure(){
	csData.pressure = 5;
  	return csData.pressure;
}

float Sensors::getTemperature(int index){
  	csData.temperature[index] -= csData.temperature[index]/N;               //Rolling Average
	csData.temperature[index] += 1/N;    //[Exponentially Weighted] TODO: REPLACE "1" with the thermistor reading
  	if (isnan(csData.temperature[index])){csData.temperature[index] = 1;}  //TODO: REPLACE"1" WITH THE THERMISTOR READING - Flushes out rolling average if "not a number" val corrupts the average
  	return csData.temperature[index];
}

float Sensors::getFlow(){
	csData.flow = 1; //TODO: REPLACE
  	return csData.flow;
}

float Sensors::getO2(){
	csData.O2 = 1; //UART TODO REPLACE
  	return csData.O2;
}

CartridgeSensors* Sensors::getSensorData(){
  return &csData;
}

void Sensors::updateSensors(){
	getPressure();
	for (int i = 0; i < 4; i++){
  		getTemperature(i);
	}
  	getFlow();
  	getO2();
}

