#include "../hw_i2c.h"
#include "hw_board.h"

void 
hw_i2c_init(uint16_t flags)
{
	flags = flags;
	stamp_i2c_init();
}

bool 
hw_i2c_start(void)
{
	// Do nothing. Atmel TWID API hides the start and stop.
	return true;
}

void 
hw_i2c_stop(void)
{
	// Do nothing. Atmel TWID API hides the start and stop.
}

uint8_t 
hw_i2c_write (
	uint8_t  slave_address, 
	uint32_t internal_address,
	uint8_t  isize, 
	uint8_t  *data,
	uint32_t n_bytes)
{
	return stamp_i2c_write(
		slave_address, 
		internal_address,
		isize, 
		data,
		n_bytes
		); // == 0;
}

uint8_t 
hw_i2c_read (	
	uint8_t  slave_address, 
	uint32_t internal_address,
	uint8_t  isize, 
	uint8_t  *data,
	uint32_t n_bytes)
{
	return stamp_i2c_read (	
		slave_address, 
		internal_address,
		isize, 
		data,
		n_bytes); // == 0;

}