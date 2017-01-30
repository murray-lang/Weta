#ifndef __WETA_ENDIAN_H__
#define __WETA_ENDIAN_H__

#include <weta_platform.h>

extern void hton_int16(int16_t  val, uint8_t* bytes);
extern void hton_uint16(uint16_t val, uint8_t* bytes);
extern void hton_int32(int32_t  val, uint8_t* bytes);
extern void hton_uint32(uint32_t val, uint8_t* bytes);
extern void hton_float(float    val, uint8_t* bytes);
extern void hton_double(double   val, uint8_t* bytes);

extern int16_t  ntoh_int16(uint8_t* bytes);
extern uint16_t ntoh_uint16(uint8_t* bytes);
extern int32_t  ntoh_int32(uint8_t* bytes);
extern uint32_t ntoh_uint32(uint8_t* bytes);
extern float    ntoh_float(uint8_t* bytes);
extern double   ntoh_double(uint8_t* bytes);

#endif // __WETA_ENDIAN_H__
