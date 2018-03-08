#include "Sensors.h"
#include "UART.h"

#include <wiringPiI2C.h>
#include <math.h>


//TEMP FOR DEBUG
#include <iostream>

#define ADC0 0b1111111
#define ADC1 0b1111111

#define ADC_VERSION_REG 0b00000000
#define ADC_VERSION 0b00000000

using namespace std;

Sensors::Sensors(int sensorIndex){
	//Connect to I2C peripherals
	wiringPiI2CSetup(1);	

	//ADC0 - Verify Connection - check registers	
	wiringPiI2CReadReg8(ADC0, ADC_VERSION_REG)==ADC_VERSION?sensorStatus.adc0=true:sensorStatus.adc0=false;

	//ADC1 - Verify Connection - check registers
	wiringPiI2CReadReg8(ADC1, ADC_VERSION_REG)==ADC_VERSION?sensorStatus.adc1=true:sensorStatus.adc1=false;

	//Assign pointers
	pPressure = &(pressureLocation[sensorIndex]);
	pTemperature1 = &(temperatureLocation[0][sensorIndex]);
	pTemperature2 = &(temperatureLocation[1][sensorIndex]);
	pTemperature3 = &(temperatureLocation[2][sensorIndex]);
	pRTD = &(rtdLocation[sensorIndex]);
	pFlow = &(flowLocation[sensorIndex]);
		
	//Oxygen determine via UART - Check for connection
	UART oxygenSensor;
	oxygenSensor.isConnected()?sensorStatus.oxygen=true:sensorStatus.oxygen=false;
	cout << sensorStatus.oxygen << endl;	
	
	for (int i = 0; i < 4; i++){
  		csData.temperature[i] = 30;      //First value for rolling avg. [to prevent dividing by 0]
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

