#ifndef __MACHINE__H__
#define __MACHINE__H__

#define MACHINE_OK 0
#define MACHINE_ERROR 1

#define MOTOR_THIS_WAY 0
#define MOTOR_THAT_WAY (!MOTOR_THIS_WAY)

#define SERVO_LEFT   2
#define SERVO_RIGTH  1
#define REF_POSITION 180 //If the reference for the servo's Position in 180 

/* i2c */

/*
#include "atmel_twi.h"
#include "atmel_twid.h"
#include "async.h"
#include "SAM3N.h"
*/


typedef struct {
    char data_rx;
    char is_new;
} SerialP;

char machine_init(int flags);
void machine_print_state(void);

void beep(void);
void machine_wait(unsigned int delay); /* wait for 0.1 sec */

char motor_select(int motors);
void motor_reverse_direction(void);
void motor_thisway_thatway(int new_dir);
void motor_turn_on(void);
void motor_turn_off(void);
void motor_set_speed(int power);
void motor_set_brake(void);


void send_serial_data(unsigned char value);
void send_serial_iflab(unsigned char value);

void servo_set_position(char who , int position);
int c_Duty(int data);

unsigned char is_data_read(char who);
unsigned char read_serial_data(char who);

int machine_sensor_get(int who);
int machine_switch_get(unsigned char who);

/* infrared */
void             ir_send          (unsigned char val, int do_delay);
unsigned char    last_ir          (void);
int              new_ir_present   (void); /* 1 if IR received since last call to last_ir */

/* port */
void              bus_port_send                (unsigned char val);
unsigned char     bus_port_send_report_reply   (unsigned char val);

#endif
