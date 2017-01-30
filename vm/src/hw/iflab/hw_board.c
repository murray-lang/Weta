#include "hw_board.h"
#include "ch.h"
#include "hal.h"
#include <pal_lld.h>
#include "test.h"
#include "chprintf.h"
#include "atmel_adc.h"
#include "atmel_pwm.h"
#include "atmel_pio.h"

/* i2c libraries */
#include "atmel_twi.h"
#include "atmel_twid.h"
#include "async.h"
#include <string.h>
#include "flashd.h"
//#include <io.h>
//#include <machine.h>
//#include <vm.h>




#define PWM_FREQUENCY          400000
#define MAX_DUTY_CYCLE         100
#define MIN_DUTY_CYCLE         0

#define MAX_SER_DCYCLE         4000
#define MIN_SER_DCYCLE         0

#define PWM_CH0                0
#define PWM_CH1                1
#define PWM_CH2                2
#define PWM_CH3                3

#define PIN_DIR_CH0            2
#define PIN_DIR_CH1            5

#define ADC_CH1                0
#define ADC_CH2                1
#define ADC_CH3                2
#define ADC_CH4                3

/* Moved to main_weta.c (for now)
#define SWITCH1                6
#define SWITCH2                7
#define SWITCH3                8
#define SWITCH4                11

#define LED1                   12
#define LED2                   15
#define LED3                   1
#define LED4                   0
*/
/* i2c */
#define TWCK					400000
#define BOARD_MCK               48000000


Twid twid;


int16_t adc_values[1] = { 0 } ;
volatile unsigned char conversionDone = 0 ;



static void 
serve_adc_interrupt(void)
{
    uint32_t status;
    status = ADC_GetStatus(ADC);
    if ( (status & ADC_ISR_RXBUFF) == ADC_ISR_RXBUFF )
    {
        conversionDone = 1;
        ADC_ReadBuffer( ADC, adc_values, 1 ) ;  //Lee solo un valor
    }
}

CH_IRQ_HANDLER(ADC_IRQHandler)
{
    CH_IRQ_PROLOGUE();
    serve_adc_interrupt();
    CH_IRQ_EPILOGUE();
}





int Sin[] = {512, 579, 645, 708, 768, 824, 874, 918, 955, 985, 1007, 1020, 1023, 1020, 1007, 985, 955, 918, 874, 824, 768, 708, 645, 579, 512, 445, 379, 316, 256, 200, 150, 106, 69, 39, 17, 4, 0, 4, 17, 39, 69, 106, 150, 200, 256, 316, 379, 445};

/*
void Delay (unsigned long a)
{

  volatile unsigned long d;

  d=a;
  while (--d!=0);
}
*/

void 
stamp_buzzer_init(void) /*Initialize PWM buzzer */
{
}

void 
stamp_buzzer_value(unsigned int val) 
{
	val = val; // Try to prevent warning
}

void 
stamp_buzzer_beep(void) 
{
	int a,i;
	for(a=0,i=0; i<4000; i++)
  {
  	
		stamp_buzzer_value(Sin[a++]);
		a%=47;
		//Delay(10);
		//cyg_thread_delay(10);
	}
}


static const Pin motor_pins[] = {
    PIN_PWM_CH0,
    PIN_PWM_CH1
};

static const Pin adc_pins[] = {
    PIN_ADC_CH1,     // PA17 as defined in atmel_pio.h
    PIN_ADC_CH2,     // PA18 as defined in atmel_pio.h
    PIN_ADC_CH3,     // PA19 as defined in atmel_pio.h
    PIN_ADC_CH4      // PA20 as defined in atmel_pio.h
};


static const Pin servo_pins [] = {
    PIN_PWM_CH2,
    PIN_PWM_CH3
};

/* i2c */
static const Pin i2c_pins[] = {
	PIN_TWD0,
	PIN_TWCK0
};


void 
stamp_dcmotor_init(void)  /*Initialize PWM*/
{
    PIO_Configure(motor_pins, PIO_LISTSIZE(motor_pins));
    PWM_Initialize(); /* Enable PWMC peripheral clock */
    PWMC_ConfigureClocks(PWM_FREQUENCY, 0 , BOARD_MCK );
    /* Configure Channel 0*/ 
    PWMC_ConfigureChannel(PWM_CH0, PWM_CMR_CPRE_CLKA, PWM_CMR_CALG, PWM_CMR_CPOL);
    PWMC_SetPeriod(PWM_CH0, MAX_DUTY_CYCLE);
    PWMC_SetDutyCycle(PWM_CH0, MIN_DUTY_CYCLE);
    PWMC_EnableChannel(PWM_CH0);
    /* Configure PWMC channel 1 */
    PWMC_ConfigureChannel(PWM_CH1, PWM_CMR_CPRE_CLKA, PWM_CMR_CALG, PWM_CMR_CPOL);
    PWMC_SetPeriod(PWM_CH1, MAX_DUTY_CYCLE);
    PWMC_SetDutyCycle(PWM_CH1, MIN_DUTY_CYCLE);
    PWMC_EnableChannel(PWM_CH1);
    /*Set direction pins as outputs */
    palSetPadMode(IOPORT1, PIN_DIR_CH0, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(IOPORT1, PIN_DIR_CH1, PAL_MODE_OUTPUT_PUSHPULL);
}

void 
stamp_control_motor(uint8_t idmotor, motor_states state, uint8_t value)
{
    switch (state) {
    case MOTOR_STOP:
      PWMC_SetDutyCycle(idmotor, MIN_DUTY_CYCLE);
      break;
    case MOTOR_LEFT:     /*Change direction, same speed */
      if(idmotor == 1){
        palSetPad(IOPORT1, PIN_DIR_CH1);
      }
      else{                
        palSetPad(IOPORT1, PIN_DIR_CH0);
      }
      break;
    case MOTOR_RIGHT:
      if(idmotor == 1){
        palClearPad(IOPORT1, PIN_DIR_CH1);
      }
      else{                
        palClearPad(IOPORT1, PIN_DIR_CH0);
      }
     break;
    case MOTOR_SPEED:
       PWMC_SetDutyCycle(idmotor, value);
     break;
    }
}


void 
stamp_init_adc(void)     /*Initialize ADC*/
{
  /* ADC configuration*/
  PIO_Configure(adc_pins, PIO_LISTSIZE(adc_pins));
  ADC_Initialize( ADC);
  /* startup = 15:    640 periods of ADCClock
   * for prescal = 11
   *     prescal: ADCClock = MCK / ( (PRESCAL+1) * 2 ) => 48MHz / ((11+1)*2) = 2MHz
   *     ADC clock = 2 MHz
   */
  ADC_cfgFrequency( ADC, 15, 11 ); //
  ADC_check( ADC, BOARD_MCK ); // Board Clock 48000000
//  ADC_EnableChannel(ADC, ADC_CHANNEL_4);
  nvicEnableVector(ADC_IRQn, CORTEX_PRIORITY_MASK(SAM3_ADC_PRIORITY)); /* Enable ADC interrupt */
//  ADC_StartConversion(ADC); /* Start conversion */
//  ADC_ReadBuffer(ADC,adc_values,1);
  ADC_EnableIt(ADC,ADC_IER_RXBUFF); /* Enable PDC channel interrupt */
}

bool 
stamp_adc(uint8_t channel, uint16_t* value)   // return adc value 
{
	if (channel > 3)
		return false;
		
	int status ;
	//channel--;
	ADC_EnableChannel(ADC, channel);   /*channel= (ADC_CHANNEL_0 ..  ADC_CHANNEL_15) */
	ADC_StartConversion(ADC);          /* Start conversion */
	ADC_ReadBuffer(ADC,adc_values,1);

	status = ADC_GetStatus( ADC );
	if ( (status & ADC_ISR_EOC4) == ADC_ISR_EOC4 ) /* if conversion is done*/
		ADC_StartConversion( ADC ) ;

	//    while ( !conversionDone ) {};
	if ( conversionDone )
	{
		conversionDone = 0 ;
	}
	*value = adc_values[0]*100/1023;
	return true;
}
/*
void 
stamp_gpio_init(void)
{
    palSetPadMode(IOPORT1, LED1, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(IOPORT1, LED2, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(IOPORT2, LED3, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(IOPORT2, LED4, PAL_MODE_OUTPUT_PUSHPULL);
	
	palSetPadMode(IOPORT1, SWITCH1, PAL_MODE_INPUT);
	palSetPadMode(IOPORT1, SWITCH2, PAL_MODE_INPUT);
	palSetPadMode(IOPORT1, SWITCH3, PAL_MODE_INPUT);
	palSetPadMode(IOPORT1, SWITCH4, PAL_MODE_INPUT);
}
*/
void 
stamp_gpio_init_pin(uint32_t port, uint32_t pin, uint32_t mode)
{
	palSetPadMode((ioportid_t)port, pin, mode);
}

bool 
stamp_get_gpio(uint32_t port, uint32_t pin, bool* value)
{
	*value = palReadPad((ioportid_t)port, pin);
	return true;
}

bool 
stamp_set_gpio(uint32_t port, uint32_t pin, bool value)
{
	if (value)
	{
		palSetPad((ioportid_t)port, pin);
		
	}
	else
	{
		palClearPad((ioportid_t)port, pin);
	}

	return true;
}

/*
bool 
stamp_get_gpio(uint8_t digital_input, bool* value)   // return gpio value 
{
    switch (digital_input) 
	{
    case 1:
		*value = palReadPad(IOPORT1, SWITCH1);
		break;
    case 2:
		*value = palReadPad(IOPORT1, SWITCH2);
		break;
    case 3:
		*value = palReadPad(IOPORT1, SWITCH3);
		break;
    case 4:
		*value = palReadPad(IOPORT1, SWITCH4);
		break;
	default:
		return false;
    }
 return true;
}

bool 
stamp_set_gpio(uint8_t digital_output, bool value)   // set gpio
{
	switch (digital_output) 
	{
    case 1:
		if (value)
		{
			palSetPad(IOPORT1, LED1);
		}
      else
			palClearPad(IOPORT1, LED1);
      break;
    case 2:
		if (value)
		{
			palSetPad(IOPORT1, LED2);
		}
		else
			palClearPad(IOPORT1, LED2);
		break;
    case 3:
		if (value)
		{
			palSetPad(IOPORT2, LED3);
		}
		else
			palClearPad(IOPORT2, LED3);
		break;
    case 4:
		if (value)
		{
			palSetPad(IOPORT2, LED4);
		}
		else
			palClearPad(IOPORT2, LED4);
		break;
    case 5:
		if (value)
		{
			palSetPad(IOPORT2, 11);
		}
		else
			palClearPad(IOPORT2, 11);
		break;
	default:
		return false;
	}
 	return true;
} 
*/

void stamp_servomotor_init()
{
    PIO_Configure(servo_pins, PIO_LISTSIZE(servo_pins));
    
    PWMC_ConfigureChannel(PWM_CH2, PWM_CMR_CPRE_CLKA, PWM_CMR_CALG , PWM_CMR_CPOL);
    PWMC_SetPeriod(PWM_CH2, MAX_SER_DCYCLE);
    PWMC_SetDutyCycle(PWM_CH2, MIN_SER_DCYCLE);
    PWMC_EnableChannel(PWM_CH2);
  
  	/* Configure PWMC channel 3 */
    PWMC_ConfigureChannel(PWM_CH3, PWM_CMR_CPRE_CLKA, PWM_CMR_CALG, PWM_CMR_CPOL);
    PWMC_SetPeriod(PWM_CH3, MAX_SER_DCYCLE);
    PWMC_SetDutyCycle(PWM_CH3, MIN_SER_DCYCLE);
    PWMC_EnableChannel(PWM_CH3); 
}


void stamp_servomotor_control(int idmotor, uint16_t value)
{   
    PWMC_SetDutyCycle((idmotor+2), value);    
}

/* i2c */
void stamp_i2c_init(){
	PIO_Configure(i2c_pins, PIO_LISTSIZE(i2c_pins));
	TWI0_Initialize();
	TWI_ConfigureMaster(TWI0, TWCK, BOARD_MCK);
	TWID_Initialize(&twid, TWI0);

	//memset(pData, 0, 64);
}

/* i2c */

uint8_t stamp_i2c_write (uint8_t slave_address, 
				uint8_t internal_address,
				uint8_t isize, 
				uint8_t *data,
				uint8_t n_bytes) {
	return TWID_Write(&twid, slave_address, internal_address, isize, data, n_bytes, 0);
}

uint8_t stamp_i2c_read (	uint8_t slave_address, 
				uint8_t internal_address,
				uint8_t isize, 
				uint8_t  *data,
				uint8_t n_bytes)  {
	//memset(data, 0, 64);
	return TWID_Read(&twid, slave_address, internal_address, isize, data, n_bytes, 0);
}



# if 0 
void write_programm_to_flash(void)
{
//TODO: Disable irqs
  uint32_t locknumber;
  unsigned char data[5];
  unsigned int status;
   data[0] = 0x32;
   data[1] = 0x32;
   data[2] = 0x32;
   data[3] = 0x33;
   data[4] = 0x32;

   locknumber = FLASHD_Unlock(FLASH_PROGRAM_ADDRESS_START, FLASH_PROGRAM_ADDRESS_END, 0, 0);
   FLASHD_Write(FLASH_PROGRAM_ADDRESS_START, &data, 6);           
   FLASHD_Lock(FLASH_PROGRAM_ADDRESS_START, FLASH_PROGRAM_ADDRESS_END, 0, 0);

/** \brief  Enable IRQ Interrupts

  This function enables IRQ interrupts by clearing the I-bit in the CPSR.
  Can only be executed in Privileged modes.
__attribute__( ( always_inline ) ) __STATIC_INLINE void __enable_irq(void)
{
  __ASM volatile ("cpsie i");
}
 */


/** \brief  Disable IRQ Interrupts

  This function disables IRQ interrupts by setting the I-bit in the CPSR.
  Can only be executed in Privileged modes.
__attribute__( ( always_inline ) ) __STATIC_INLINE void __disable_irq(void)
{
  __ASM volatile ("cpsid i");
}
 */


}


void read_programm_from_flash(void)
{
  unsigned char *data, count;
  for(count = 0; count < 6; count ++){
    *data = (unsigned char*)FLASH_PROGRAM_ADDRESS_START;
    chprintf((BaseChannel *)&SD2,"%d", *data);
    data++;
  }
    chprintf((BaseChannel *)&SD2,"\r\n" );
}
#endif

