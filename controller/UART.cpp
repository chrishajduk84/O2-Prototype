#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "UART.h"

using namespace std;

UART::UART(){
    int n;
    struct dirent **namelist;
    const char* sysdir = "/dev/";
    // Scan through /sys/class/tty - it contains all tty-devices in the system
    n = scandir(sysdir, &namelist, NULL, NULL);
    if (n < 0)
        perror("scandir");
    else {
        while (n--) {
            if (strcmp(namelist[n]->d_name,"..") && strcmp(namelist[n]->d_name,".")) {

                // Construct full absolute file path
                string devicedir = sysdir;
                devicedir += namelist[n]->d_name;
                if (devicedir.find("AMA") != string::npos){
                    portName = devicedir;
                }
            }
            free(namelist[n]);
        }
        free(namelist);
    }

    openDevice(portName.c_str());	
}

UART::~UART(){
	
}

void UART::openDevice(const char* device){
     struct termios port_config; //sets up termios configuration structure for the serial port
     serial_fd = open(device, O_RDWR, O_NOCTTY);// | O_NOCTTY | O_NDELAY); //opens serial port in device slot
     if(serial_fd == -1)
     { //-1 is the error message for failed open port
           fprintf(stdout, "[UART] Failed to open port\n");
     }

     if (tcgetattr(serial_fd, &port_config) < 0){
         cout << "[UART] Error getting port configuration" << endl;   
     }

     if (cfsetospeed(&port_config, B19200) < 0){
         cout << "[UART] Error setting output baud rate" << endl;
     }
     if (cfsetispeed(&port_config, B19200) < 0){
         cout << "[UART] Error setting input baud rate" << endl;
     }
     port_config.c_iflag &= ~(IXANY | IXON | IXOFF); //input flags (XON/XOFF software flow control, no NL to CR translation)
     //port_config.c_oflag = 0;
     //port_config.c_lflag &= ~(ECHO | ECHOE | ICANON | ISIG); //local flags (no line processing, echo off, echo newline off, canonical mode off, extended input processing off, signal chars off)
     port_config.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);
     port_config.c_cflag |= (CS8 | CLOCAL | CREAD | B19200); //Control flags (local connection, enable receivingt characters, force 8 bit input)

     port_config.c_cc[VMIN]  = 1;
     port_config.c_cc[VTIME] = 0;
     tcsetattr(serial_fd, TCSANOW, &port_config); //Sets the termios struct of the file handle fd from the options defined in options. TCSAFLUSH performs the change as soon as possible.
     cout << "[UART] Serial port opened on: " << device << endl;
}

bool UART::isConnected(){
     float oxygen;
     char sendCommand[9] = {0x55,0xAA,0x7E,0x02,0x4F,0x43,0x94,0x0E,0x0D};
     oxygen = readLine(sendCommand,9);
     cout << oxygen << endl;
     return oxygen > 20 && oxygen < 100;
}

void UART::writeLine(char* data, int len){
     write(serial_fd, data, len);
}

float UART::readLine(char* sendData, int sendLen){
     write(serial_fd, sendData, sendLen);
     unsigned char data[11];
     int len = readLine(data);
     return parseLine(data,len);


}

float UART::parseLine(unsigned char* data, int len){
     //TODO later: add checksum checking
     int result = 0;
     for (int i = 0; i < len; i++){
         if (data[i] == 0x4F && data[i+1] == 0x43){
	      int MSB = data[i+2];
	      int LSB = data[i+3];
              result = (MSB << 8) + LSB; 
              break; 
         }
     }
     return result/10.0;
}

int UART::readLine(unsigned char* buf){
     unsigned char data = 0;
     unsigned char lastData = 0;
     unsigned char c = 0;    

    
     //wait until the return character is detected - This is oxygen sensor specific
     int byteCount = 0;
     while (data != 0x0D){
          if (read(serial_fd,&c,1) > 0){
               buf[byteCount++] = c;
               data = c;
          }
     }   
     return byteCount;
}
