#include <Adafruit_PWMServoDriver.h>;
#define CUSTOM // Define custom variable is used
#include "OUTPUT_SLAVE.h"



#include "EasyCAT.h"                // EasyCAT library
#include <SPI.h>                    // SPI library - The EasyCAT shield is connected to the SPI bus
EasyCAT EASYCAT;                    // EasyCAT istantiation 
#include <Wire.h>                   // I2C library
//Global Copied from IO Script EasyCat
unsigned long Millis, PreviousMillis = 0;
unsigned char EcatState;
unsigned char N_Cycles = 0;
bool Blink = true;
UWORD Raw;



Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40); 

//INITIATES  WITH SERVO IN MIDDLE
int ServoPulseLength1 = 350;
int ServoPulseLength2 = 350;
int ServoPulseLength3 = 350;
int ServoPulseLength4 = 350;
int ServoPulseLength5 = 350;
int ServoPulseLength6 = 350;


// Servo Variables
int servoMin = 150; //(of 4096)  150 og 600 gives around 180 deg.
int servoMax = 600; //(of 4096)  
int servoFrequency = 50; //Hz




void setup() {
  Serial.begin(9600); // For printing
  Wire.begin(); // Begin i2c
  
  // Servo 
  pwm1.begin();
  pwm1.setOscillatorFrequency(27000000);
  pwm1.setPWMFreq(servoFrequency);


  // THIS NEEDS TO BE HERE TO ENABLE COMMUNICATION. Copied from IO script
  if (EASYCAT.Init() == true)                                     // Initialization
  {                                                               // succesfully completed
    Serial.print ("initialized");                                 //
  }                                                               //
  
  else                                                            // Initialization failed   
  {                                                               // The EasyCAT board was not recognized
    Serial.print ("initialization failed");                       //     
                                                                  // The most common reason is that the SPI 
                                                                  // chip select choosen on the board doesn't 
                                                                  // match the one choosen by the firmware
                                                                  
                                                                  // Stay in loop for ever
                                                                  // with the red led blinking fast
    while(1)                                                      //
    {                                                             //   
      digitalWrite (13, LOW);                                 // 
      delay(100);                                                 //   
      digitalWrite (13, HIGH);                                //  
      delay(100);                                                 // 
    }                                                             // 
  }    
  PreviousMillis = millis();  
}




void loop() {
  // put your main code here, to run repeatedly:
  EcatState = EASYCAT.MainTask();
  Application();
  DiagPrint();    
}

// 
float servoSafety(int ServoPulseLength){
  int  servoPulse;
  if (ServoPulseLength<= 250 & ServoPulseLength != 0)
  {
    servoPulse = 250;
  }
  else if (ServoPulseLength>= 450)
  {
    servoPulse = 450;}
  else if (ServoPulseLength== 0)
  {
    servoPulse = 350;}
  else
  {
    servoPulse = ServoPulseLength;
  }
  return servoPulse;
}


void Application(){
  ServoPulseLength1 = EASYCAT.BufferOut.Cust.ServoPulseLength1;
  ServoPulseLength2 = EASYCAT.BufferOut.Cust.ServoPulseLength2;
  ServoPulseLength3 = EASYCAT.BufferOut.Cust.ServoPulseLength3;
  ServoPulseLength4 = EASYCAT.BufferOut.Cust.ServoPulseLength4;
  ServoPulseLength5 = EASYCAT.BufferOut.Cust.ServoPulseLength5;
  ServoPulseLength6 = EASYCAT.BufferOut.Cust.ServoPulseLength6;

  ServoPulseLength1 = servoSafety(ServoPulseLength1);
  ServoPulseLength2 = servoSafety(ServoPulseLength2);
  ServoPulseLength3 = servoSafety(ServoPulseLength3);
  ServoPulseLength4 = servoSafety(ServoPulseLength4);
  ServoPulseLength5 = servoSafety(ServoPulseLength5);
  ServoPulseLength6 = servoSafety(ServoPulseLength6);

  pwm1.setPWM(1, 0, ServoPulseLength1);
  pwm1.setPWM(2, 0, ServoPulseLength2);
  pwm1.setPWM(3, 0, ServoPulseLength3);
  pwm1.setPWM(4, 0, ServoPulseLength4);
  pwm1.setPWM(5, 0, ServoPulseLength5);
  pwm1.setPWM(6, 0, ServoPulseLength6);
}




void DiagPrint(){
  Serial.print("     ServoValue:    ");
  Serial.print(ServoPulseLength1);
  Serial.print(ServoPulseLength2);
  Serial.print(ServoPulseLength3);
  Serial.print(ServoPulseLength4);
  Serial.print(ServoPulseLength5);
  Serial.println(ServoPulseLength6);

}



