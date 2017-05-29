#include <SparkFunMAX31855k.h>
#include <SPI.h>

char pwm = 0;
unsigned long lastTime = 0;



#if defined(ARDUINO_AVR_UNO)

#define VALVE1 11 //3 Way
#define VALVE2 10 //2 Way
#define VALVE3 9 //
#define VALVE4 6

#define PUMPRELAY 8
#define HEATRELAY 7

#define SWITCHSTATE1 5
#define SWITCHSTATE2 4

#define CHIP_SELECT_PIN 10 // Using standard CS line (SS) for thermocouple reader
#define VCC 12 // SPI Reference
#define GND 13 // SPI Reference

#elif defined(ARDUINO_AVR_MEGA2560)
//Mega 2560 specific code
#define VALVE1 2
#define VALVE2 3
#define VALVE3 4
#define VALVE4 5

#define PUMPRELAY 6
#define HEATRELAY 7

#define SWITCHSTATE1 22
#define SWITCHSTATE2 23

#define CHIP_SELECT_PIN 53 // Using standard CS line (SS) for thermocouple reader
#define VCC 48 // SPI Reference
#define GND 49 // SPI Reference

#elif defined(ARDUINO_AVR_NANO)
#define VALVE1 3
#define VALVE2 -1
#define VALVE3 5
#define VALVE4 6
#define VALVE5 -1
#define VALVE6 9

#define PUMPRELAY 4
#define HEATRELAY_A 7
#define HEATRELAY_B 8

#define BUTTON1 16
#define BUTTON2 17
#define BUTTON3 18
#define BUTTON4 19
#define POWERBUTTON 2

#define LED1 10
#define LED2 11
#define POWERLED 1

#define CHIP_SELECT_PIN_A 14 // Using standard CS line (SS) for thermocouple reader
#define CHIP_SELECT_PIN_B 15
#define VCC 48 // SPI Reference
#define GND 49 // SPI Reference

#else
#error Unsupported hardware
#endif

int pinoutA[4] = {HEATRELAY_A, VALVE1, VALVE2, VALVE3};
int pinoutB[4] = {HEATRELAY_B, VALVE4, VALVE5, VALVE6};
int* pinoutArray[2] = {pinoutA, pinoutB};

//Instantiate the thermocouple class
SparkFunMAX31855k probeA(CHIP_SELECT_PIN_A, VCC, GND);
SparkFunMAX31855k probeB(CHIP_SELECT_PIN_B, VCC, GND);

int response = 0;
float currentTemp = 0;

bool POWERSTATE = false;
bool lastPowerButtonState = 0;
//bool lastButtonState = 0;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  pinMode(VALVE1, OUTPUT);
  pinMode(VALVE2, OUTPUT);
  pinMode(VALVE3, OUTPUT);
  pinMode(VALVE4, OUTPUT);
  pinMode(VALVE5, OUTPUT);
  pinMode(VALVE6, OUTPUT);
  pinMode(PUMPRELAY, OUTPUT);
  pinMode(HEATRELAY_A, OUTPUT);
  pinMode(HEATRELAY_B, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  pinMode(POWERBUTTON, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(POWERLED, OUTPUT);
  setPwmFrequency(VALVE1, 1); //Frequency with divisor = 1; 31250Hz
  setPwmFrequency(VALVE2, 1); //Frequency with divisor = 1; 31250Hz
  setPwmFrequency(VALVE3, 1); //Frequency with divisor = 1; 31250Hz
  setPwmFrequency(VALVE4, 1); //Frequency with divisor = 1; 31250Hz
  setPwmFrequency(VALVE5, 1); //Frequency with divisor = 1; 31250Hz
  setPwmFrequency(VALVE6, 1); //Frequency with divisor = 1; 31250Hz
  analogWrite(VALVE1,0);
  analogWrite(VALVE2,0);
  analogWrite(VALVE3,0);
  analogWrite(VALVE4,0);
  analogWrite(VALVE5,0);
  analogWrite(VALVE6,0);
 
  digitalWrite(PUMPRELAY, LOW);
  digitalWrite(HEATRELAY_A, LOW);
  digitalWrite(HEATRELAY_B, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(POWERLED, LOW);
  lastTime = millis();

  //Wait for thermocouple sensor to stabilize
  delay(750);
  //Read Cold Junction Temperature
  float temperature = probeA.readCJT();
  if (!isnan(temperature)) {
    Serial.print("CJT is (˚C): ");
    Serial.println(temperature);
  }

}

void loop(){

  //Read Sensor State
  //currentTemp = probeA.readTempC();
  
  // put your main code here, to run repeatedly:
  int response = Serial.readString().toInt();
  Serial.print("Response: ");
  Serial.println(response);
  //Serial.print("Temperature: ");
  //Serial.println(currentTemp);

  //if (response == 1) powerDevice(true);
  //if (response == 2) powerDevice(false);


  /*
   * Poll for button presses
   *
  if (digitalRead(BUTTON1) | digitalRead(BUTTON2) | digitalRead(BUTTON3) | digitalRead(BUTTON4) != lastButtonState){
    if (digitalRead(BUTTON1)){
      setState(1,0,pinoutArray);
      setState(1,1,pinoutArray);
    }
    else if (digitalRead(BUTTON2)){
      setState(2,0,pinoutArray);
      setState(2,1,pinoutArray);
    }
    else if (digitalRead(BUTTON3)){
      setState(3,0,pinoutArray);
      setState(3,1,pinoutArray);
    }
    else if (digitalRead(BUTTON4)){
      setState(4,0,pinoutArray);
      setState(4,1,pinoutArray);
    }
  }*/
  //Poll Power Button for presses
  if (digitalRead(POWERBUTTON) != lastPowerButtonState){
    lastPowerButtonState = digitalRead(POWERBUTTON);
    if(lastPowerButtonState){
      digitalWrite(LED1, !digitalRead(LED1));
      powerDevice(!POWERSTATE);
    }
  }

  //Wait for the thermocouple
  //delay(750);
}

void powerDevice(bool turnOn){
  if (turnOn == true && POWERSTATE == false){
    //Turn on device
    digitalWrite(POWERLED, HIGH);
    setState(1,0,pinoutArray);
    setState(1,1,pinoutArray);
    POWERSTATE = true;
  }
  else if (turnOn == false && POWERSTATE == true){
    //Turn off device
    digitalWrite(POWERLED, LOW);
    //setState(5,0,pinoutArray);
    //setState(5,1,pinoutArray);
    POWERSTATE = false;
    
  }
  
}


void setState(int state, int tube, int** peripheralArray){
    if (state == 1){
      //Pump for negative pressure (get as much air out as possible)
      //*PUMP ON
      //*HEATING OFF
      //*VALVE1 Closed
      //*VALVE2 Open (Not implemented yet)
      //*VALVE3 Closed (3-way valve - Open towards environment)
      startPump();
      stopHeating(peripheralArray[tube][0]);
      closeValve(peripheralArray[tube][1]);
      openValve(peripheralArray[tube][2]);
      closeValve(peripheralArray[tube][3]);
    }
    else if (state == 2){
      //Start Preheating
      //*PUMP ON
      //*HEATING ON
      //*VALVE1 Closed
      //*VALVE2 Closed (Not implemented yet)
      //*VALVE3 Open (3-way valve - Open towards balloon)
      startPump();
      startHeating(peripheralArray[tube][0]);
      closeValve(peripheralArray[tube][1]);
      closeValve(peripheralArray[tube][2]);
      openValve(peripheralArray[tube][3]);
    }
    else if (state == 3){
      //Releasing Oxygen:
      //*PUMP ON
      //*HEATING ON
      //*VALVE1 Closed
      //*VALVE2 Open (Not implemented yet)
      //*VALVE3 Open (3-way valve - Open towards balloon)
      startPump();
      stopHeating(peripheralArray[tube][0]);
      closeValve(peripheralArray[tube][1]);
      openValve(peripheralArray[tube][2]);
      openValve(peripheralArray[tube][3]);
    }
    else if (state == 4){
      //Regenerating Oxygen:
      //*PUMP ON
      //*HEATING OFF
      //*VALVE1 Open
      //*VALVE2 Open (Not implemented yet)
      //*VALVE3 Closed (3-way valve - Open towards environment)
      startPump();
      stopHeating(peripheralArray[tube][0]);
      openValve(peripheralArray[tube][1]);
      openValve(peripheralArray[tube][2]);
      closeValve(peripheralArray[tube][3]);
  }
  else if (state == 5){
      //Powered off state:
      //*PUMP OFF
      //*HEATING OFF
      //*VALVE1 Closed
      //*VALVE2 Closed (Not implemented yet)
      //*VALVE3 Closed (3-way valve - Open towards environment)
      stopPump();
      stopHeating(peripheralArray[tube][0]);
      closeValve(peripheralArray[tube][1]);
      closeValve(peripheralArray[tube][2]);
      closeValve(peripheralArray[tube][3]);
  }
  
}

void startHeating(int pinNum){
  digitalWrite(pinNum, HIGH);
}

void stopHeating(int pinNum){
  digitalWrite(pinNum, LOW);
}

void startPump(){
  digitalWrite(PUMPRELAY, HIGH);
}

void stopPump(){
  digitalWrite(PUMPRELAY, LOW);
}

void openValve(int pinNum){
  if (pinNum >= 0){
    analogWrite(pinNum, 255);
    lastTime = millis();
    while(millis() - lastTime < (unsigned long)1000*64);
    analogWrite(pinNum, 127);
  }
}

void closeValve(int pinNum){
  if (pinNum >= 0){
    analogWrite(pinNum, 0);
  }
}

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
#elif defined(ARDUINO_AVR_MEGA2560)
void setPwmFrequency(int pin, int divisor) {
  byte mode;
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 13 || pin == 4) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else if (pin == 12 || pin == 11) {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    } else if (pin == 10 || pin == 9) {
      TCCR2B = TCCR2B & 0b11111000 | mode;
    } else if (pin == 5 || pin == 3 || pin == 2) {
      TCCR3B = TCCR3B & 0b11111000 | mode;
    } else if (pin == 8 || pin == 7 || pin == 6) {
      TCCR4B = TCCR4B & 0b11111000 | mode;
    }
}
#endif
