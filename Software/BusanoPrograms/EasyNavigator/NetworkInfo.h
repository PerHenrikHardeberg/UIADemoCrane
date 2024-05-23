//******************************************************************************************* 
//*                                                                                         * 
//*     This file has been created by the Easy Navigator on Tue Apr 09 14:17:45 2024
//*                                                                                         * 
//*                    AB&T Tecnologie Informatiche - Ivrea Italy                           * 
//*                                                                                         * 
//*                    www.bausano.net           info@bausano.net                           * 
//*                                                                                         * 
//******************************************************************************************* 


#ifndef  NETWORK_H 
#define NETWORK_H 

#define _PACK_  __attribute__((packed))


// 1) ******* network size *******************************************************************

#define NUM_SLAVES 1                         // number of slaves

#define WKC 2                                // expected working counter


// 2) ******* slaves names *******************************************************************

const PROGMEM uint8_t NAME[NUM_SLAVES][64] =
{
   {"INPUT_SLAVE"}
};


// 3) ******* entries of the input PDOs - data from the slaves to the master *****************

struct  _PACK_
RxData
{
   struct _PACK_
   {
      float  encoder1Rad;
      float  encoder2Rad;
      float  encoder3Rad;
      float  TelescopeDistance_mm;
      float  xRotDeg;
      float  yRotDeg;
      uint8_t  mpuCalibrated;
   }
   _1_INPUT_SLAVE;

};


// 4) ******* entries of the output PDOs - data from the master to the slaves ****************

struct  _PACK_
TxData
{
};//


// 5) ******* SM configuration ***************************************************************

const PROGMEM uint8_t  SM[NUM_SLAVES][4][8] =
{
   {
      {0x00,0x01,0x00,0x20,0x00,0x19,0x12,0x00},
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
   }
};


// 6) ******* FMMU configuration *************************************************************

const PROGMEM uint8_t  FMMU[NUM_SLAVES][2][16] =
{
   {
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x01,0x01,0x00,0x12,0x00,0x07,0x00,0x00,0x19,0x00,0x00,0x00,0x00},
   }
};

#endif 
