#ifndef __HW_I2C_H__
#define __HW_I2C_H__

#include <weta_platform.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _Hardware;

extern void hw_i2c_init(struct _Hardware* hw,uint16_t flags);

extern bool hw_i2c_start(struct _Hardware* hw);
extern void hw_i2c_stop(struct _Hardware* hw);

extern uint8_t hw_i2c_write(
    struct _Hardware* hw,
		uint8_t slave_address,
		uint32_t internal_address,
		uint8_t isize,
		uint8_t *data,
		uint32_t n_bytes);

extern uint8_t hw_i2c_read(
    struct _Hardware* hw,
		uint8_t slave_address,
		uint32_t internal_address,
		uint8_t isize,
		uint8_t *data,
		uint32_t n_bytes);

#ifdef __cplusplus
}
#endif

#endif // __HW_I2C_H__