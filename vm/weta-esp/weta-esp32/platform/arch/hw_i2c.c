#include <hw_i2c.h>
#include <hw.h>

void WETAFUNCATTR 
hw_i2c_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;
	//stamp_i2c_init();
}

bool WETAFUNCATTR 
hw_i2c_start(struct _Hardware* hw)
{
	// Do nothing. Atmel TWID API hides the start and stop.
	return true;
}

void WETAFUNCATTR 
hw_i2c_stop(struct _Hardware* hw)
{
	// Do nothing. Atmel TWID API hides the start and stop.
}

uint8_t WETAFUNCATTR 
hw_i2c_write (
	struct _Hardware* hw,
	uint8_t  slave_address, 
	uint32_t internal_address,
	uint8_t  isize, 
	uint8_t  *data,
	uint32_t n_bytes)
{
	return false;
}

uint8_t WETAFUNCATTR 
hw_i2c_read (
	struct _Hardware* hw,
	uint8_t  slave_address, 
	uint32_t internal_address,
	uint8_t  isize, 
	uint8_t  *data,
	uint32_t n_bytes)
{
	return false;
}
