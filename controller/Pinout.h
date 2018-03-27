#ifndef PINOUT_H
#define PINOUT_H

typedef struct _peripheralLocation{
char i2cAddress;
char pinNumber;
_peripheralLocation(char i2c, char pin) : i2cAddress(i2c), pinNumber(pin){};
} PeripheralLocation;

static PeripheralLocation pressureLocation[3] = {PeripheralLocation(0x48,0), PeripheralLocation(0x48,1), PeripheralLocation(0x48,2)};
static PeripheralLocation thermistor1Location[3] = {PeripheralLocation(0x48,4), PeripheralLocation(0x48,6), PeripheralLocation(0x48,5)}; 
static PeripheralLocation thermistor2Location[3] = {PeripheralLocation(0x48,7), PeripheralLocation(0x49,9%8), PeripheralLocation(0x49,8%8)};
static PeripheralLocation thermistor3Location[3] = {PeripheralLocation(0x49,10%8), PeripheralLocation(0x49,12%8), PeripheralLocation(0x49,11%8)};
static PeripheralLocation rtdLocation[3] = {PeripheralLocation(0x49,13%8), PeripheralLocation(0x49,15%8), PeripheralLocation(0x49,14%8)};
static PeripheralLocation* temperatureLocation[4] = {thermistor1Location, thermistor2Location, thermistor3Location, rtdLocation};
static PeripheralLocation flowLocation = PeripheralLocation(0x48,3);
static PeripheralLocation pumpLocation[4] = {PeripheralLocation(0x15, 6), PeripheralLocation(0x15, 7), PeripheralLocation(0x15,8), PeripheralLocation(0x15, 9)};
static PeripheralLocation heaterLocation[3] = {PeripheralLocation(0x15, 0),PeripheralLocation(0x15,1),PeripheralLocation(0x15, 2)};
static PeripheralLocation coolerLocation[3] = {PeripheralLocation(0x15, 3),PeripheralLocation(0x15,4),PeripheralLocation(0x15, 5)};

#endif
