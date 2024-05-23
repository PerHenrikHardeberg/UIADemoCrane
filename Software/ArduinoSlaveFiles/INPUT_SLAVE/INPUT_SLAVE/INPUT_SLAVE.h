#ifndef CUSTOM_PDO_NAME_H
#define CUSTOM_PDO_NAME_H

//-------------------------------------------------------------------//
//                                                                   //
//     This file has been created by the Easy Configurator tool      //
//                                                                   //
//     Easy Configurator project INPUT_SLAVE.prj
//                                                                   //
//-------------------------------------------------------------------//


#define CUST_BYTE_NUM_OUT	0
#define CUST_BYTE_NUM_IN	25
#define TOT_BYTE_NUM_ROUND_OUT	0
#define TOT_BYTE_NUM_ROUND_IN	28


typedef union												//---- output buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_OUT];
	struct
	{
	}Cust;
} PROCBUFFER_OUT;


typedef union												//---- input buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_IN];
	struct
	{
		float       encoder1Rad;
		float       encoder2Rad;
		float       encoder3Rad;
		float       TelescopeDistance_mm;
		float       xRotDeg;
		float       yRotDeg;
		uint8_t     mpuCalibrated;
	}Cust;
} PROCBUFFER_IN;

#endif