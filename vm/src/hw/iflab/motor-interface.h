#ifndef _MOTOR_INTERFACE_H_
#define _MOTOR_INTERFACE_H_


typedef enum motor_states
{ 
  MOTOR_LEFT=0, 
	MOTOR_RIGHT, 
	MOTOR_STOP, 
//	MOTOR_BRAKE,
        MOTOR_SPEED
} motor_states;
/*
typedef union _motor_command {
	unsigned char data;
	struct {
		unsigned char state:2;
		unsigned char channel:2;
		unsigned char speed:4;
	} bits;
} motor_command;
 */
#endif
