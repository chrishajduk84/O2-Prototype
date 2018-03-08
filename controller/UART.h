#ifndef UART_H
#define UART_H

#include <string>

class UART{
	std::string portName;
	int serial_fd; 
	void openDevice(const char* device);
	float parseLine(unsigned char* data, int len);
	
public:
	UART();
	~UART();
	bool isConnected();
	void writeLine(char* data, int len);
	float readLine(char* sendData, int sendLen);
	int readLine(unsigned char* returnData);
};

#endif
