#ifndef __BOARD_stamp__H__
#define __BOARD_stamp__H__

#define  __inline static inline

#include "motor-interface.h"
#include <stdint.h>
#include <stdbool.h>

//#include "ch.h"
//#include "hal.h"



void stamp_buzzer_init(void);
void stamp_buzzer_beep(void);
void stamp_buzzer_value(unsigned int val);

void stamp_dcmotor_init(void);
void stamp_control_motor(uint8_t idmotor, motor_states state, uint8_t value);

void stamp_servomotor_control(int idmotor, uint16_t value);
void stamp_servomotor_init(void);

void stamp_init_adc(void);
bool stamp_adc(uint8_t channel, uint16_t* value);

//void stamp_gpio_init(void);
void stamp_gpio_init_pin(uint32_t port, uint32_t pin, uint32_t mode);
bool stamp_get_gpio(uint32_t port, uint32_t pin, bool* value);
bool stamp_set_gpio(uint32_t port, uint32_t pin, bool value);

void read_programm_from_flash(void);
void write_programm_to_flash(void);

/* i2c */
void stamp_i2c_init(void);

uint8_t stamp_i2c_write (uint8_t slave_address, 
				uint8_t internal_address,
				uint8_t isize, 
				uint8_t *data,
				uint8_t n_bytes);

uint8_t stamp_i2c_read (	uint8_t slave_address, 
				uint8_t internal_address,
				uint8_t isize, 
				uint8_t  *data,
				uint8_t n_bytes);



#endif
