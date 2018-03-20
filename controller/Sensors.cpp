#include "Sensors.h"
#include "OxygenController.h"
#include <wiringPiI2C.h>
#include <math.h>


//TEMP FOR DEBUG
#include <iostream>

#define ADC0 0x48
#define ADC1 0x49

char ADC_CHANNEL[8] = {0b10000000,
                    0b11000000,
                    0b10010000,
                    0b11010000,
                    0b10100000,
                    0b11100000,
                    0b10110000,
                    0b11110000};

#define ADC_POWERDOWN_BETWEEN 0b0000
#define ADC_IREF_OFF_AD_ON 0b0100
#define ADC_IREF_ON_AD_OFF 0b1000
#define ADC_IREF_ON_AD_ON 0b1100

#define IREF_TURNON 0b00001000

using namespace std;

Sensors::Sensors(int sensorIndex){
	//Connect to I2C peripherals
	fd0 = wiringPiI2CSetup(ADC0);	
    fd1 = wiringPiI2CSetup(ADC1);
    
    if (wiringPiI2CWrite(fd0, ADC_CHANNEL[0] | ADC_IREF_OFF_AD_ON) >= 0){ //-1 indicates an invalid file descriptor
	    //ADC0 - Verify Connection - check registers	
        //cout << wiringPiI2CReadReg16(fd0,0) << endl;
	    sensorStatus.adc0=true;
        cout << "[I2C] ADC0 detected" << endl;
    }
     
    if (wiringPiI2CWrite(fd1, ADC_CHANNEL[0] | ADC_IREF_OFF_AD_ON) >= 0){
	    //ADC1 - Verify Connection - check registers	
        //cout << wiringPiI2CReadReg16(fd1,0) << endl;
	    sensorStatus.adc1=true;
        cout << "[I2C] ADC1 detected" << endl;
    }
       
    //Assign pointers
	pPressure = &(pressureLocation[sensorIndex]);
	pTemperature1 = &(temperatureLocation[0][sensorIndex]);
	pTemperature2 = &(temperatureLocation[1][sensorIndex]);
	pTemperature3 = &(temperatureLocation[2][sensorIndex]);
	pRTD = &(rtdLocation[sensorIndex]);
	pFlow = &(flowLocation[sensorIndex]);
		
	//Oxygen determine via UART - Check for connection
	oxygenSensor.isConnected()?sensorStatus.oxygen=true:sensorStatus.oxygen=false;
		//List which sensors are available in DEBUG
	
	for (int i = 0; i < 4; i++){
  		csData.temperature[i] = 30;      //First value for rolling avg. [to prevent dividing by 0]
	}

}

Sensors::~Sensors(){}

float Sensors::getPressure(){
    int filehandle = pPressure->i2cAddress == 0x48?fd0:fd1;
    if (wiringPiI2CWrite(filehandle, ADC_CHANNEL[pPressure->pinNumber] | ADC_IREF_OFF_AD_ON) >= 0){
        unsigned int readings = wiringPiI2CReadReg16(filehandle,0);
        readings = ((readings & 0xFF) << 8) + ((readings & 0xFF00) >> 8); //Account for endianess
        float pressure = readings*30.0/0b111111111111;
        cout << "Press: " << pressure << endl;
        printf("%x",readings);
    } 
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
	char sendCommand[9] = {0x55,0xAA,0x7E,0x02,0x4F,0x43,0x94,0x0E,0x0D};
        csData.O2 = oxygenSensor.readLine(sendCommand,9);
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

