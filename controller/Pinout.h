#ifndef PINOUT_H
#define PINOUT_H

typedef struct _peripheralLocation{
char i2cAddress;
char pinNumber;
_peripheralLocation(char i2c, char pin) : i2cAddress(i2c), pinNumber(pin){};
} PeripheralLocation;

static PeripheralLocation pressureLocation[3] = {PeripheralLocation(0x48,0), PeripheralLocation(0x48,1), PeripheralLocation(0x48,2)};
static PeripheralLocation thermistor1Location[3] = {PeripheralLocation(0,0), PeripheralLocation(0,0), PeripheralLocation(0,0)}; 
static PeripheralLocation thermistor2Location[3] = {PeripheralLocation(0,0), PeripheralLocation(0,0), PeripheralLocation(0,0)};
static PeripheralLocation thermistor3Location[3] = {PeripheralLocation(0,0), PeripheralLocation(0,0), PeripheralLocation(0,0)};
static PeripheralLocation rtdLocation[3] = {PeripheralLocation(0,0), PeripheralLocation(0,0), PeripheralLocation(0,0)};
static PeripheralLocation* temperatureLocation[4] = {thermistor1Location, thermistor2Location, thermistor3Location, rtdLocation};
static PeripheralLocation flowLocation[3] = {PeripheralLocation(0,0), PeripheralLocation(0,0), PeripheralLocation(0,0)};
static PeripheralLocation pumpLocation[4] = {PeripheralLocation(0x15, 6), PeripheralLocation(0x15, 7), PeripheralLocation(0x15,8), PeripheralLocation(0x15, 9)};

#endif
