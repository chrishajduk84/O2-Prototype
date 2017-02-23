char pwm = 0;
unsigned long lastTime = 0;

#if defined(ARDUINO_AVR_UNO)

#define VALVE1 11 //3 Way
#define VALVE2 10 //2 Way
#define VALVE3 9
#define VALVE4 6

#define PUMPRELAY 8
#define HEATRELAY 7

#define SWITCHSTATE1 5
#define SWITCHSTATE2 4

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

#else
#error Unsupported hardware
#endif

int controlSource = 0; //By default, the switch controls the functionality

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(VALVE1, OUTPUT);
  pinMode(VALVE2, OUTPUT);
  pinMode(VALVE3, OUTPUT);
  pinMode(VALVE4, OUTPUT);
  pinMode(PUMPRELAY, OUTPUT);
  pinMode(HEATRELAY, OUTPUT);
  pinMode(SWITCHSTATE1, INPUT);
  pinMode(SWITCHSTATE2, INPUT);
  setPwmFrequency(VALVE1, 1); //Frequency with divisor = 1; 31250Hz
  setPwmFrequency(VALVE2, 1); //Frequency with divisor = 1; 31250Hz
  setPwmFrequency(VALVE3, 1); //Frequency with divisor = 1; 31250Hz
  setPwmFrequency(VALVE4, 1); //Frequency with divisor = 1; 31250Hz
  analogWrite(VALVE1,0);
  analogWrite(VALVE2,0);
  analogWrite(VALVE3,0);
  analogWrite(VALVE4,0);
  digitalWrite(PUMPRELAY, LOW);
  digitalWrite(HEATRELAY, LOW);
  lastTime = millis();

  if (digitalRead(SWITCHSTATE1) == LOW){
    controlSource = 1; //If the switch is in the triggered state, this signals computer control  
  }
}

void loop() {
  
  // put your main code here, to run repeatedly:
  int response = Serial.readString().toInt();
  Serial.print("1:");
  Serial.println(digitalRead(SWITCHSTATE1));
  Serial.print("2:");
  Serial.println(digitalRead(SWITCHSTATE2));
  Serial.print("ControlSource:");
  Serial.println(controlSource);
  if (response == 1 && !controlSource || digitalRead(SWITCHSTATE1) && controlSource){
    //Release Oxygen:
    //*PUMP ON
    //*HEATING ON
    //*VALVE1 Closed
    //*VALVE2 Open (3-way valve - Open towards Balloon)
    //*

    //Start Preheating
    startHeating();
    closeValve(VALVE1);
    closeValve(VALVE2);
    //Wait a few seconds to clear any air
    lastTime = millis();
    while(millis() - lastTime < (unsigned long)5000*64);
    
    openValve(VALVE1);
    startPump();
  }
  else if (response == 2 && !controlSource|| digitalRead(SWITCHSTATE2) && controlSource){
    //Regenerate Oxygen:
    //*PUMP ON
    //*HEATING OFF
    //*VALVE1 OPEN
    //*VALVE2 CLOSE
    stopHeating();
    startPump();
    openValve(VALVE2);
    closeValve(VALVE1);
  }
  else if (response == 3){
    //Turn off device
    stopHeating();
    stopPump();
    closeValve(VALVE1);
    closeValve(VALVE2);
  }
}

void startHeating(){
  digitalWrite(HEATRELAY, HIGH);
}

void stopHeating(){
  digitalWrite(HEATRELAY, LOW);
}

void startPump(){
  digitalWrite(PUMPRELAY, HIGH);
}

void stopPump(){
  digitalWrite(PUMPRELAY, LOW);
}

void openValve(int pinNum){
  analogWrite(pinNum, 255);
  lastTime = millis();
  while(millis() - lastTime < (unsigned long)1000*64);
  analogWrite(pinNum, 127);
}

void closeValve(int pinNum){
  analogWrite(pinNum, 0);
}

#if defined(ARDUINO_AVR_UNO)
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
