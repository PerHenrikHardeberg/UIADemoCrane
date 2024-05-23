#ifndef CUSTOM_PDO_NAME_H
#define CUSTOM_PDO_NAME_H

//-------------------------------------------------------------------//
//                                                                   //
//     This file has been created by the Easy Configurator tool      //
//                                                                   //
//     Easy Configurator project OUTPUT_SLAVE.prj
//                                                                   //
//-------------------------------------------------------------------//


#define CUST_BYTE_NUM_OUT	12
#define CUST_BYTE_NUM_IN	0
#define TOT_BYTE_NUM_ROUND_OUT	12
#define TOT_BYTE_NUM_ROUND_IN	0


typedef union												//---- output buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_OUT];
	struct
	{
		uint16_t    ServoPulseLength1;
		uint16_t    ServoPulseLength2;
		uint16_t    ServoPulseLength3;
		uint16_t    ServoPulseLength4;
		uint16_t    ServoPulseLength5;
		uint16_t    ServoPulseLength6;
	}Cust;
} PROCBUFFER_OUT;


typedef union												//---- input buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_IN];
	struct
	{
	}Cust;
} PROCBUFFER_IN;

#endif