#include "../hw_endian.h"

void WETAFUNCATTR
hton_int16(int16_t  val, uint8_t* bytes)
{
	hton_uint16((uint16_t)val, bytes);		
}

void WETAFUNCATTR
hton_uint16(uint16_t val, uint8_t* bytes)
{
	bytes[0] = (uint8_t)((val >> 8) & 0xFF);
	bytes[1] = (uint8_t)(val & 0xFF);
}

void WETAFUNCATTR
hton_int32(int32_t  val, uint8_t* bytes)
{
	hton_uint32((uint32_t)val, bytes);
}

void WETAFUNCATTR
hton_uint32(uint32_t val, uint8_t* bytes)
{
	bytes[0] = (uint8_t)((val >> 24) & 0xFF);
	bytes[1] = (uint8_t)((val >> 16) & 0xFF);
	bytes[2] = (uint8_t)((val >> 8) & 0xFF);
	bytes[3] = (uint8_t)(val & 0xFF);
}

void WETAFUNCATTR
hton_float(float val, uint8_t* bytes)
{
	union _fbytes
	{
		float f;
		uint8_t bytes[4];
	} fbytes;

	fbytes.f = val;
	bytes[0] = fbytes.bytes[3];
	bytes[1] = fbytes.bytes[2];
	bytes[2] = fbytes.bytes[1];
	bytes[3] = fbytes.bytes[0];
}

void WETAFUNCATTR
hton_double(double val, uint8_t* bytes)
{
	union _dbytes
	{
		double d;
		uint8_t bytes[8];
	} dbytes;

	dbytes.d = val;
	bytes[0] = dbytes.bytes[7];
	bytes[1] = dbytes.bytes[6];
	bytes[2] = dbytes.bytes[5];
	bytes[3] = dbytes.bytes[4];
	bytes[4] = dbytes.bytes[3];
	bytes[5] = dbytes.bytes[2];
	bytes[6] = dbytes.bytes[1];
	bytes[7] = dbytes.bytes[0];
}


int16_t WETAFUNCATTR
ntoh_int16(uint8_t* bytes)
{
	return (int16_t)ntoh_uint16(bytes);
}

uint16_t WETAFUNCATTR
ntoh_uint16(uint8_t* bytes)
{
	return (uint16_t)(bytes[0] << 8) + (bytes[1] & 0xFF);
}

int32_t WETAFUNCATTR
ntoh_int32(uint8_t* bytes)
{
	return (int32_t) ntoh_uint32(bytes); 
}

uint32_t WETAFUNCATTR
ntoh_uint32(uint8_t* bytes)
{
	return (uint32_t)(bytes[0] << 24) 
		+ (uint32_t)(bytes[1] << 16) 
		+ (uint32_t)(bytes[2] << 8) 
		+ (bytes[3] & 0xFF);
}

float WETAFUNCATTR
ntoh_float(uint8_t* bytes)
{
	union _fbytes
	{
		float f;
		uint8_t bytes[4];
	} fbytes;

	fbytes.bytes[3] = bytes[0];
	fbytes.bytes[2] = bytes[1];
	fbytes.bytes[1] = bytes[2];
	fbytes.bytes[0] = bytes[3];
	return fbytes.f;
}

double WETAFUNCATTR
ntoh_double(uint8_t* bytes)
{
	union _dbytes
	{
		double d;
		uint8_t bytes[8];
	} dbytes;

	dbytes.bytes[7] = bytes[0];
	dbytes.bytes[6] = bytes[1];
	dbytes.bytes[5] = bytes[2];
	dbytes.bytes[4] = bytes[3];
	dbytes.bytes[3] = bytes[4];
	dbytes.bytes[2] = bytes[5];
	dbytes.bytes[1] = bytes[6];
	dbytes.bytes[0] = bytes[7];
	return dbytes.d;	
}

