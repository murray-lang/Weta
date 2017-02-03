#include <machine.h>
#include <hw_board.h>
#include <vm.h>
#include <io.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "chprintf.h"

typedef struct {
    char selected;
    char is_on;
    char brake;
    char dir;
    char speed;
} MotorState;

typedef struct {
    char name;
    int position;
    int c_Duty;
} ServoState;

typedef struct {
    MotorState *motor;
    ServoState *servo;
    SerialP    *serial;
    char n_sports;
    char n_servos;
    char n_motors;
    int vm_flags;
} MachineState;

static MachineState state;

char machine_init(int flags)
{
    short int i;

    state.vm_flags = flags;
    state.n_motors = 2;
    state.n_servos = 2;
    state.n_sports = 1;
    
    assert(state.n_motors > 0);
    assert(state.n_servos > 0);        /* make sure that a machine was initialized */

    state.motor = malloc(sizeof(MotorState) * state.n_motors);
    state.servo = malloc(sizeof(ServoState) * state.n_servos);
    state.serial = malloc(sizeof(SerialP) * state.n_sports);

    if (!state.motor) {
        return (EXIT_FAILURE);
    }
    
    if (!state.servo) {
        return (EXIT_FAILURE);
    }
    
    if (!state.serial) {
        return (EXIT_FAILURE);
    }
    
    for (i = 0; i < state.n_motors; ++i) {
        state.motor[i].selected = 0;
        state.motor[i].is_on = 0;
        state.motor[i].speed = 0;
        state.motor[i].brake = 0; /* no active braking */
        state.motor[i].dir = MOTOR_THIS_WAY;
    }
    
    for (i = 0; i < state.n_servos; ++i) {
        state.servo[i].name = i+1;
        state.servo[i].position = 0;
        state.servo[i].c_Duty = 0;
    }
    
    for (i = 0; i < state.n_sports; ++i) {
        state.serial[i].data_rx = 0;
        state.serial[i].is_new =  0;
    }
    
    stamp_init_adc();
    stamp_dcmotor_init();
    stamp_buzzer_init();
    stamp_servomotor_init();
    stamp_gpio_init();

	/* i2c */
	stamp_i2c_init();

    return MACHINE_OK;
}

void machine_print_state(void)
{
    short int i;
//    chprintf((BaseChannel *)&SD2, " [machine_state  motors: \r\n");

    for (i = 0; i < state.n_motors; ++i) {
/*        chprintf((BaseChannel *)&SD2, "(#%d sel:%d on:%d speed:%d brake:%d dir:%s)\r\n",
                i, state.motor[i].selected, state.motor[i].is_on,
                state.motor[i].speed,
                state.motor[i].brake,
                state.motor[i].dir == MOTOR_THIS_WAY ? "this" : "that");
*/    }
    /* Servomotor */
    for (i = 0; i < state.n_servos; ++i) {
//        chprintf((BaseChannel *)&SD2, "(#%d position:%d )\r\n",
//                state.servo[i].name,state.servo[i].position);
    }
//    chprintf((BaseChannel *)&SD2, " ]\r\n");

}

/* Functions that do not depend on the machine type */

static void motor_update_hardware_state(void)
{
    short int i;
    for (i = 0; i < state.n_motors; ++i) {
        if (state.motor[i].selected) {
            if (!state.motor[i].is_on)
                stamp_control_motor(i, MOTOR_STOP, 0);
            else if(state.motor[i].brake)
            {
                stamp_control_motor(i,MOTOR_SPEED,0);
            }
            else {
              stamp_control_motor(i,
                                  (state.motor[i].dir ==
                                   MOTOR_THIS_WAY) ? MOTOR_RIGHT :
                                  MOTOR_LEFT, 0);
              stamp_control_motor(i,MOTOR_SPEED, state.motor[i].speed);
            }
        }
    }
}

char motor_select(int motors)
{
    short int i;
    int aux;

    for (i = 0, aux = motors; i < state.n_motors; ++i) {
        state.motor[i].selected = aux & 1;
        aux = aux >> 1;
    }
    return MACHINE_OK;
}

void motor_reverse_direction(void)
{
    short int i;

    for (i = 0; i < state.n_motors; ++i)
        if (state.motor[i].selected)
            state.motor[i].dir = !state.motor[i].dir;
    motor_update_hardware_state();
}

void motor_thisway_thatway(int new_dir)
{
    short int i;

    for (i = 0; i < state.n_motors; ++i)
        if (state.motor[i].selected)
            state.motor[i].dir = new_dir;
    motor_update_hardware_state();
}

static void motor_turn_onoff(int on_or_off)
{
    short int i;
    for (i = 0; i < state.n_motors; ++i)
        if (state.motor[i].selected)
        {
            state.motor[i].is_on = on_or_off;

            /* TODO: Is it OK to stop active braking when the motor is turned off? */
            state.motor[i].brake = 0;
        }
    motor_update_hardware_state();
}

/* range is from 0 up to 8 */
void motor_set_speed(int speed)
{
    int i;
    if (speed > 100)
       speed = 100;
    for (i = 0; i < state.n_motors; ++i)
        if (state.motor[i].selected){
            state.motor[i].speed = speed;
            state.motor[i].brake = 0; 
        }
    motor_update_hardware_state();
}

/* active braking */
void motor_set_brake(void)
{
    int i;

    for (i = 0; i < state.n_motors; ++i)
        if (state.motor[i].selected)
            state.motor[i].brake = 1;
    motor_update_hardware_state();
}

void motor_turn_on()
{
    motor_turn_onoff(1);
}

void motor_turn_off()
{
    motor_turn_onoff(0);
}


/* machine-dependant functions */
/* Please use #ifdef if you need specific code */


void beep(void)
{
  stamp_buzzer_beep();

}

void machine_wait(unsigned int delay)
{
    chThdSleepMilliseconds(delay*100);
}


/*
 * 0 = Port A
 * 1 = Port B
 */
int
machine_sensor_get(int who)
{
  return stamp_adc(who);
}
/*
 * 0 = Port A
 * 1 = Port B
 */
int
machine_switch_get(unsigned char who)
{
  return stamp_get_gpio(who);
}


/* IR */

static char new_ir_waiting;
static unsigned char last_ir_read;

void
ir_send(unsigned char val, int do_delay)
{
  if (do_delay)
    machine_wait(val);
}

unsigned char
last_ir (void)
{
  new_ir_waiting = 0;
  return last_ir_read;
}

int
new_ir_present (void) /* 1 if IR received since last call to last_ir */
{
  return new_ir_waiting;
}

/* BUS */

void
bus_port_send (unsigned char val)
{

}

/* sends value out Bus Port and reports reply */
unsigned char
bus_port_send_report_reply (unsigned char val)
{
  unsigned char r;

  r = 0;
  return r;
}

/* Functions for Servomotors */

static void servo_update_hardware_state(void)
{
    short int i;
    for (i = 0; i < state.n_servos; ++i) {
        stamp_servomotor_control(i,state.servo[i].c_Duty);
    }
}

void servo_set_position(char who , int position)
{
    if (who  == SERVO_RIGTH){
        state.servo[0].position = position;
        state.servo[0].c_Duty = c_Duty(10);
    }else{
        state.servo[1].position = position;
        state.servo[1].c_Duty = c_Duty(position);  
    };
    
    if (state.vm_flags & VM_VERBOSE)
    {
        machine_print_state();
    }

    servo_update_hardware_state();
}

int c_Duty(int data)
{   int result = (data)+220;
    return result;
}

void send_serial_data(unsigned char value)
{
    chprintf((BaseChannel *)&SD1,"%d;", value );
}

void send_serial_iflab(unsigned char value)
{
    chprintf((BaseChannel *)&SD2,"%d;", value );
}

unsigned char is_data_read(char who)
{
    state.serial[who].data_rx = who == 0 ? chIOGet(&SD1) : chIOGet(&SD2);
    state.serial[who].is_new = 1; 
    return state.serial[who].is_new;
}

unsigned char read_serial_data(char who)
{
    return state.serial[who].data_rx;
}


