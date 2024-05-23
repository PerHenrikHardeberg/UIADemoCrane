#define CUSTOM // Define custom variable is used
#include "INPUT_SLAVE.h"


// 0x70 is i2c multiplexer
// Encoders:
#include <AS5600.h>

// Define three as5600 objects
AS5600 stpA;
AS5600 stpB;
AS5600 stpC;


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


// ADDED 12.03.2024 TOF Sensor VL53lXX
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

///////////////////////////////////////////////////////////////////////////////////////
/// MOVING AVARAGE FILTER_TEST: 
#include "MovingAverageFloat.h"
MovingAverageFloat <32> Floatfilter; // For Laser sensor

MovingAverageFloat <32> Floatfilter2; // For IMU ANGLE Value
////////////////////////////////////////////////////////////////////////////////////////////

//INITIATES  SENSORS
float radA = 0.0;
float radB = 0.0;
float radC = 0.0;
float xRot = 0.0;
float yRot = 0.0;
float TelescopeDistance_mm = 0.0;

// Encoder Variables
//I2C ports on multiplex board
int I2CA=1; 
int I2CB=2;
int I2CC=3;


float newradA;
float newradB;
float newradC;

float initA;
float initB;
float initC;

float dradA=0,dradB=0,dradC=0;
float drevA=0,drevB=0,drevC=0;

/////////////////////////////////////////////////////////////
//////IMU
bool sensorCalibration = 0;
const int MPU_addr1 = 0x68;
float xa, ya, za, roll, pitch;
////// IMU
//////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(9600); // For printing
  Wire.begin(); // Begin i2c
  
  EASYCAT.BufferIn.Cust.mpuCalibrated = sensorCalibration;

////////////////////////////////////////////////////////////////////
///IMU
  Wire.beginTransmission(MPU_addr1);                 //begin, send the slave adress (in this case 68)
  Wire.write(0x6B);                                  //make the reset (place a 0 into the 6B register)
  Wire.write(0);
  Wire.endTransmission(true);                        //end the transmission
////IMU
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
///////// EVERYTHING FROM SETUP TOF
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test.");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power
  Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));

  // start continuous ranging
  lox.startRangeContinuous();
//////////////////////////////////////////// EVERYTHING FROM SETUP TOF
///////////////////////////////////////////////////////////////////////////////


  //////////////////////////
  ////////////////////////
  //Encoders//////
  //Encoder1:
  Wire.begin();
  TCA9548(I2CA);
  stpA.begin(4);
  stpA.setDirection(AS5600_CLOCK_WISE);
  int b = stpA.isConnected();
  initA=stpA.getCumulativePosition()*AS5600_RAW_TO_RADIANS;
  radA=0;
  delay(20);

  //Encoder2:
  TCA9548(I2CB);
  stpB.begin(4);
  stpB.setDirection(AS5600_CLOCK_WISE);
  int c = stpB.isConnected();
  initB=stpB.getCumulativePosition()*AS5600_RAW_TO_RADIANS;
  radB=0;
  delay(20);

  //Encoder3:
  TCA9548(I2CC);
  stpC.begin(4);
  stpC.setDirection(AS5600_CLOCK_WISE);
  int d = stpC.isConnected();
  initC=stpC.getCumulativePosition()*AS5600_RAW_TO_RADIANS;
  radC=0;
  delay(20);
  delay(1000);

  

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



void Application(){
  encoders();
  IMUWithSIMPLE();
  TOFSensor();

  EASYCAT.BufferIn.Cust.encoder1Rad = radA;
  EASYCAT.BufferIn.Cust.encoder2Rad = radB;
  EASYCAT.BufferIn.Cust.encoder3Rad = radC;
  EASYCAT.BufferIn.Cust.xRotDeg = xRot;
  EASYCAT.BufferIn.Cust.yRotDeg = yRot;
  EASYCAT.BufferIn.Cust.TelescopeDistance_mm = TelescopeDistance_mm;
  EASYCAT.BufferIn.Cust.mpuCalibrated = sensorCalibration;
}


void IMUWithSIMPLE(){
  Wire.beginTransmission(MPU_addr1);
  Wire.write(0x3B);  //send starting register address, accelerometer high byte
  Wire.endTransmission(false); //restart for read
  Wire.requestFrom(MPU_addr1, 6); //get six bytes accelerometer data
  int t = Wire.read();
  xa = (t << 8) | Wire.read();
  t = Wire.read();
  ya = (t << 8) | Wire.read();
  t = Wire.read();
  za = (t << 8) | Wire.read();

// formula from https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing
  roll = atan2(ya , za) * 180.0 / PI;
  pitch = atan2(-xa , sqrt(ya * ya + za * za)) * 180.0 / PI; //account for roll already applied
  xRot = roll;
  yRot = Floatfilter2.add(pitch); // AVARAGE FILTER 

}

void TOFSensor(){
  if (lox.isRangeComplete()) {
    float unFiltered = lox.readRange();
    
  // MOVING AVARAGE FILTER
 float AvarageFiltered = Floatfilter.add(unFiltered);
  TelescopeDistance_mm = AvarageFiltered;
  }
}



void encoders(){
   static uint32_t lastTime = 0;
   
   if (millis() - lastTime >= 5)
  {
    TCA9548(I2CA);
    newradA = stpA.getCumulativePosition()*AS5600_RAW_TO_RADIANS-initA;
    dradA=newradA-radA;
    radA=newradA;
    
    TCA9548(I2CB);
    newradB=stpB.getCumulativePosition()*AS5600_RAW_TO_RADIANS-initB;
    dradB=newradB-radB;
    radB=newradB;
    
    TCA9548(I2CC);
    newradC=stpC.getCumulativePosition()*AS5600_RAW_TO_RADIANS-initC;
    dradC = newradC-radC;
    radC=newradC;
  }
}


void TCA9548(int bus)
{
    Wire.beginTransmission(0x70);
    Wire.write(1 << bus);
    Wire.endTransmission();
}


void DiagPrint(){
  Serial.print("EncoderValue: ");
  Serial.print(radA);
  Serial.print(radB);
  Serial.print(radC);
  Serial.print(TelescopeDistance_mm);
  Serial.print(xRot);
  Serial.print(yRot);
  Serial.println();
}



