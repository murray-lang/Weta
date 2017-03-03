/**
 * @file weta.c
 * @brief Main implementation file of a Weta VM
 */
#include "weta.h"
#include "turtle/weta_turtle.h"
#include "WvmCodes.h"
#include "type_handlers/type_handlers.h"

#ifdef SUPPORT_STRING
#include <string.h>
#endif

#if defined(SUPPORT_FLOAT) || defined(SUPPORT_DOUBLE)
//#include <math.h>
#endif

#ifdef SUPPORT_STRING
static char* pszByteFormat  = DEFAULT_FORMAT_BYTE;
static char* pszIntFormat   = DEFAULT_FORMAT_INT;
static char* pszFloatFormat = DEFAULT_FORMAT_FLOAT;
#endif

// Functions implemented later in this file

void init_registers(Registers* regs);
void init_states(WetaStates* states);
#ifdef SUPPORT_CRICKET
void init_comm_transfer(Weta* pWeta);
void comm_loop(Weta* pWeta);
#endif
bool exec_prepare(Weta* pWeta);
void exec_cleanup(Weta* pWeta);
void exec_loop(Weta* pWeta);
void exec_loop_body(Weta* pWeta);
//void exec_turtle(Weta* pWeta);
#ifdef SUPPORT_GPIO
void clear_run_button(Weta* pWeta);
#endif

PSTORE weta_open_store(Weta* pWeta, WetaStorage storage);
void   weta_close_store(Weta* pWeta);
void   weta_rewind(Weta* pWeta);

bool with_current_type(Weta* pWeta);
#ifdef SUPPORT_QUERY
WetaCodePtr verify_json_codes(const char* jsonArray);
#endif

//*******************************************************************************
void WETAFUNCATTR
weta_init(Weta* pWeta, Hardware* pHardware, uint16_t flags)
{

	hw_init_specific(pHardware, flags);

    DEBUGMSG("hw_init_specific() returned\r\n");
	if(pWeta->store)
	{
		weta_store_close(pWeta->store);
		pWeta->store = 0;
	}

	//pWeta->store = weta_store_init_flash(pHardware->flash, pHardware->flashLength);

	pWeta->hal          = pHardware;
	pWeta->startAddress = 0;
    if (pHardware->sports.n_ports == 0)
    {
        pWeta->sport = pWeta->debugsport = NULL;
    }
    else if (pHardware->sports.n_ports == 1)
    {
        pWeta->sport = &pHardware->sports.ports[0];
        pWeta->debugsport = pWeta->sport;
    }
    else
    {
        pWeta->sport = &pHardware->sports.ports[WETA_SPORT];
        pWeta->debugsport = &pHardware->sports.ports[WETA_DEBUG_SPORT];
    }
	pWeta->stack = NULL;	// This will be allocated in weta_reset()

	DEBUGMSG("weta_init()\r\n");

	weta_reset(pWeta);

}

//*******************************************************************************
void WETAFUNCATTR
init_registers(Registers* regs)
{
	regs->pc               = 0;
	regs->opCode           = OP_INVALID;
	regs->withCode         = OP_WITHINT16; // Gogo board default
	regs->repcountLocation = ~0;
	regs->blockDepthMask   = 0;
	regs->blocksExecuted   = 0;
}

//*******************************************************************************
void WETAFUNCATTR
init_states(WetaStates* states)
{
	states->runRequest   = STOPPED;
	states->machineState = WETA_READY;
#ifdef SUPPORT_CRICKET
	states->commState    = COMM_IDLE;
#endif
	states->waitingCmd   = false;
}

#ifdef COOPERATIVE_RTOS

void WETAFUNCATTR
start_wait(Weta *pWeta, uint16_t millis)
{
    pWeta->regs.waitStart = hw_time_now();
    pWeta->regs.waitMillis = millis;
    pWeta->states.machineState = WETA_WAITING;
}

void WETAFUNCATTR
reset_wait(Weta *pWeta)
{
    pWeta->regs.waitStart = 0;
    pWeta->regs.waitMillis = 0;
}

bool WETAFUNCATTR
test_wait_expired(Weta *pWeta)
{
    WetaTimestamp now = hw_time_now();
    return (now - pWeta->regs.waitStart >= pWeta->regs.waitMillis * MILLIS_PER_TICK);
}

#endif

PSTORE WETAFUNCATTR
weta_open_store(Weta* pWeta, WetaStorage storage)
{
    // Clean up any existing storage now
    weta_close_store(pWeta);

    if (storage == STORAGE_FLASH)
    {
        return weta_store_open_flash(pWeta->hal->flash);
    }
    else if (storage == STORAGE_RAM)
    {
        return weta_store_open_ram();
    }
    return 0;
}

void WETAFUNCATTR
weta_close_store(Weta* pWeta)
{
    if(pWeta->store)
    {
        weta_store_close(pWeta->store);
        pWeta->store = 0;
    }
}

//*******************************************************************************
void WETAFUNCATTR
weta_rewind(Weta* pWeta)
{
#ifdef SUPPORT_MOTORS
    hw_motor_all_off(&pWeta->hal->motors);
#endif
#ifdef SUPPORT_STEPPERS
    hw_stepper_all_off(&pWeta->hal->steppers);
#endif
	init_registers(&pWeta->regs);
	init_states(&pWeta->states);
	if (pWeta->stack)
		weta_stack_release(pWeta->stack);
	pWeta->stack = weta_stack_allocate(/*WETA_STACK_SIZE*/);
}
//*******************************************************************************
void WETAFUNCATTR
weta_reset(Weta* pWeta)
{
	weta_rewind(pWeta);
    weta_close_store(pWeta);

#ifdef SUPPORT_STRING
	pWeta->formats.pszByteFormat  = pszByteFormat;
	pWeta->formats.pszIntFormat   = pszIntFormat;
	pWeta->formats.pszFloatFormat = pszFloatFormat;
#endif
    hw_serial_flush(pWeta->sport);
#ifdef SUPPORT_GPIO
    clear_run_button(pWeta);
#endif
}

//*******************************************************************************
bool WETAFUNCATTR
weta_start(Weta* pWeta, PSTORE store, WetaCodePtr address)
{
        // If new code is supplied then use it...
    if (store)
    {
        weta_reset(pWeta);
        pWeta->store = store;
        pWeta->startAddress = address;
    }
    else if (pWeta->store)  //...else if there is already code then restart...
	{
		weta_rewind(pWeta);
	}
    else    //...else assume a boot/reboot and get any code from flash
    {
        pWeta->store = weta_open_store(pWeta, STORAGE_FLASH);
        pWeta->startAddress = 0;
    }
    if (!pWeta->store)
    {
        DEBUGMSG("weta_start() failed because storage is not available.");
        weta_reset(pWeta);
        return false;
    }
	if (!exec_prepare(pWeta))
    {
        weta_reset(pWeta);
        return false;
    }
    pWeta->states.machineState = WETA_RUN;
    pWeta->states.runRequest = RUNNING;

    return true;
}

//*******************************************************************************
void WETAFUNCATTR
weta_debug(Weta* pWeta, const char * msg)
{
	hw_serial_write(pWeta->debugsport, (uint8_t*)msg, strlen(msg));
}

#ifdef SUPPORT_GPIO
void WETAFUNCATTR
clear_run_button(Weta* pWeta)
{
#ifdef SUPPORT_DEBOUNCE

    Gpio* gpio = &pWeta->hal->gpio;
    GpioPin* run = &gpio->pins[GPIO_INDEX_RUN];
    hw_gpio_clear_debounce(run);
#endif
}
#endif

#ifdef SUPPORT_DEBOUNCE
void WETAFUNCATTR
weta_debounce(Weta* pWeta)
{
    Gpio* gpio = &pWeta->hal->gpio;
	hw_gpio_debounce(gpio);
    GpioPin* run = &gpio->pins[GPIO_INDEX_RUN];
	if(run->debounce->debounced)
    {
        if (run->debounce->state)
        {
			clear_run_button(pWeta);
            weta_start(pWeta, NULL, 0);
        }
    }
}
#endif


//*******************************************************************************
void WETAFUNCATTR
weta_loop(Weta* pWeta)
{
	//pWeta->states.runRequest = RUNNING; // Spoof press of 'run' button
	bool abort = false;
	while (!abort)
	{
		weta_loop_body(pWeta);
	}
}

//*******************************************************************************
void WETAFUNCATTR
weta_loop_body(Weta* pWeta)
{
    static int count = 0;
    //count++;
    if (count == 10)
    {
        hw_motor_select(&pWeta->hal->motors, 3);
        hw_motor_power(&pWeta->hal->motors, 255);
        hw_motor_on(&pWeta->hal->motors, true);
    }

#ifdef SUPPORT_DEBOUNCE
	weta_debounce(pWeta);
#endif

	switch (pWeta->states.machineState)
	{
#ifdef COOPERATIVE_RTOS
    case WETA_WAITING:
        if (test_wait_expired(pWeta))
        {
            reset_wait(pWeta);
            pWeta->states.machineState = WETA_RUN;
            exec_loop_body(pWeta);
        }
        break;
#endif
	case WETA_CONFIG:
			// Do nothing while the machine is being reconfigured
		break;

	case WETA_READY:
#ifdef SUPPORT_CRICKET
        //DEBUGMSG( "hw_serial_available()\r\n");
		if (hw_serial_available(pWeta->sport))
		{
			pWeta->states.machineState = WETA_COMM;
			init_comm_transfer(pWeta);
			DEBUGMSG( "Weta state change WETA_READY->WETA_COMM\r\n");
		}
		else
#endif
        if (pWeta->states.runRequest == RUNNING)
		{
			//Serial.println("setMachineState(RUN)");

			DEBUGMSG( "Weta state change WETA_READY->RUN\r\n");
            if (!exec_prepare(pWeta))
            {
                DEBUGMSG( "exec_prepare() failed.\r\n");
                init_states(&pWeta->states);
            }
            else
            {
                pWeta->states.machineState = WETA_RUN;
            }
		}
		else
		{
            //DEBUGMSG( ".");
			//DEBUGMSG( "Weta machine state:WETA_READY\r\n");
		}
		break;
#ifdef SUPPORT_CRICKET
	case WETA_COMM:
		comm_loop(pWeta);
            // If comms have finished, and the machine hasn't started as a
            // result, then go into idle/ready
		if (   pWeta->states.commState == COMM_FINISHED
            && pWeta->states.machineState != WETA_RUN)
			pWeta->states.machineState = WETA_READY;
		break;
#endif
	case WETA_RUN:
		//init_registers(&pWeta->regs);
		//weta_stack_initStack(pWeta->stack);
        ////DEBUGMSG( "About to call exec_loop()\r\n");
		exec_loop_body(pWeta);
		//_motors.off();
		//pWeta->states.machineState = WETA_READY;
		//pWeta->states.runRequest = STOPPED;
		break;
#ifdef	SUPPORT_STEPPERS
	case STEPPER:
		if (hw_stepper_tick(&pWeta->hal->steppers))
			pWeta->states.machineState = WETA_RUN;
		break;
#endif
	}
}

#ifdef SUPPORT_CRICKET
static uint8_t	   cmd = 0;
static uint8_t	   remainingChars = 0;
static PSTORE      cricket_store = 0;
static WetaCodePtr start_address = 0;
static WetaCodePtr byte_count = 0;
static bool        byte_count_written = false;

void WETAFUNCATTR
init_comm_transfer(Weta* pWeta)
{
	cmd = 0;
	remainingChars = 0;
	start_address = 0;
	byte_count = 0;
	byte_count_written = false;

	pWeta->states.waitingCmd = true; 		// set the waiting flag
	pWeta->states.commState  = COMM_STARTED;
}

//*******************************************************************************
void WETAFUNCATTR
comm_loop(Weta* pWeta)
{
	//DEBUGMSG("comm_loop()\r\n");
	if (pWeta->states.commState != COMM_FINISHED)
	{
        //DEBUGMSG("pWeta->states.commState != COMM_FINISHED\r\n");
		if (!hw_serial_available(pWeta->sport))
		{
			//hw_time_waitus(10);
            //if (remainingChars > 0)
			    //DEBUGMSG("No data available. Waiting...\r\n");
			return;
		}
        //DEBUGMSG("hw_serial_read_byte()\r\n");
        uint8_t temp = hw_serial_read_byte(pWeta->sport);
        //DEBUGMSG("hw_serial_read_byte() returned %0X.\r\n", temp);
            // Debugging
        /*
        if (hw_serial_available(pWeta->sport))
            DEBUGMSG("Still more bytes available!\r\n");
        else
            DEBUGMSG("Just read last available byte!\r\n");
	    */
		//DEBUGMSG("->%02x\r\n", temp);
			// The Cricket host expects all characters to be echoed, because
			// this was done by the infrared device used for communication
			// with the Cricket robot.
		hw_serial_write_byte(pWeta->sport, temp);
		//DEBUGMSG("hw_serial_write_byte(%0X) completed.\r\n", temp);
		if(pWeta->states.waitingCmd)
		{
			cmd = temp;
				// test received command and set number of bytes to read
			switch(cmd)
			{
			case cmdCricketCheck:
				remainingChars = 1;	// remaining character received should be 0 (not '0')
				pWeta->states.waitingCmd = false;
				break;

			case cmdSetPointer:
			case cmdReadBytes:
			case cmdWriteBytes:
				remainingChars = 2;
				pWeta->states.waitingCmd = false;
			break;

			case cmdRun:
				DEBUGMSG( "Run command received\r\n");
				pWeta->states.runRequest = RUNNING;
				pWeta->states.commState  = COMM_FINISHED;
			break;

			default:
				DEBUGMSG("Unexpected data. Bail out.\n");
				pWeta->states.commState = COMM_FINISHED;
				break;
			}
		}
		else
		{
				// receive bytes corresponding to each command
			//pWeta->states.waitingCmd = false;
			switch(cmd)
			{
			case cmdCricketCheck:
				if (remainingChars == 1)
				{
					hw_serial_write_byte(pWeta->sport, cmdCricketCheckACK);
                    //DEBUGMSG("hw_serial_write_byte(%0X) completed.\r\n", cmdCricketCheckACK);
					//sprintf(msg, "->%02x\r\n", cmdCricketCheckACK);
					//DEBUGMSG( msg);
				}

				pWeta->states.commState = COMM_FINISHED;
				break;


			case cmdSetPointer:
				if (remainingChars == 2)
				{
					remainingChars--;
					start_address = (WetaCodePtr)(temp << 8);
				}
				else
				{
					remainingChars--;
					start_address |= (WetaCodePtr)(temp & 0xFF);
					pWeta->startAddress = start_address;
					pWeta->states.commState = COMM_FINISHED;

					//sprintf(msg, "Start address is %d\r\n", start_address);
					//weta_debug(pWeta, msg);
				}
				break;

			case cmdWriteBytes:
				if (remainingChars == 2)
				{
					remainingChars--;
					byte_count = (WetaCodePtr)temp << 8;
				}
				else if (remainingChars == 1)
				{
					remainingChars--;
					byte_count |= (WetaCodePtr)(temp & 0xFF);
				}
				else
				{
                    cricket_store = weta_open_store(pWeta, STORAGE_FLASH);
                    if (!cricket_store)
                    {
                        DEBUGMSG("Unable to open flash storage.\n");
                        pWeta->states.commState = COMM_FINISHED;
                        break;
                    }
					if (!byte_count_written)
					{
						//sprintf(msg, "Writing byte count(%d)\r\n", byte_count);
						//weta_debug(pWeta, msg);
							// This writes the byte count at the beginning of the data

						if (weta_store_start_write(cricket_store, start_address, byte_count))
						{
							//pWeta->regs.pc = 0;
                            DEBUGMSG("weta_store_start_write(%d, %d) success.\n", start_address, byte_count);
							byte_count_written = true;
						}
						else
						{
                            DEBUGMSG("weta_store_start_write() failed.\n");
                            weta_store_close(cricket_store);
							pWeta->states.commState = COMM_FINISHED;
							break;
						}

						byte_count_written = true;
							// Reuse the start_address variable as a counter
						//start_address = 0;
					}
					//sprintf(msg, "Writing [%d] = %02x\r\n", start_address, temp);
					//weta_debug(pWeta, msg);
					if (weta_store_write_byte(cricket_store, temp) == 1)
					{
                        //DEBUGMSG("weta_store_write_byte() succeeded.\n");
						hw_serial_write_byte(pWeta->sport, ~temp);
                        //DEBUGMSG("hw_serial_write_byte(%0X) succeeded.\n", ~temp);
						//start_address++;
						byte_count--;
						if (byte_count == 0)
						{
                            DEBUGMSG("byte_count == 0.\n");
							weta_store_flush(cricket_store);
							pWeta->states.commState = COMM_FINISHED;
                            weta_start(pWeta, cricket_store, start_address);
						}
					}
					else
					{
                        DEBUGMSG("weta_store_write_byte() failed.\n");
                        weta_store_close(cricket_store);
						pWeta->states.commState = COMM_FINISHED;
					}
				}
				break;

			case cmdReadBytes:
				if (remainingChars == 2)
				{
					remainingChars--;
					byte_count = (unsigned int)temp << 8;
				}
				else if (remainingChars == 1)
				{
					remainingChars--;
					byte_count |= temp & 0xFF;
                    cricket_store = weta_open_store(pWeta, STORAGE_FLASH);
                    if (!cricket_store)
                    {
                        DEBUGMSG("Unable to open flash storage.\n");
                        pWeta->states.commState = COMM_FINISHED;
                        break;
                    }

					if (weta_store_start_read(cricket_store, start_address))
					{
							// reuse start_address variable as a counter
						start_address = 0;
						while (byte_count > 0)
						{
							if (weta_store_read_byte(cricket_store, start_address, &temp) == 1)
							{
								hw_serial_write_byte(pWeta->sport, temp);
								byte_count--;
								start_address++;
							}
							else
							{
								hw_serial_write_byte(pWeta->sport, 0xFF);
								break;
							}
						}
					}
                    weta_store_close(cricket_store);
					pWeta->states.commState = COMM_FINISHED;
				}
				break;

			default:
				pWeta->states.commState = COMM_FINISHED;
				break;
			}
		}
	}
}
#endif // SUPPORT_CRICKET
//*******************************************************************************
bool WETAFUNCATTR
exec_prepare(Weta* pWeta)
{
	init_registers(&pWeta->regs);
	weta_stack_initStack(pWeta->stack);
	//WetaCodePtr bytesRead = 0;
	pWeta->regs.pc = 0;

	if (!weta_store_start_read(pWeta->store, pWeta->startAddress))
	{
        DEBUGMSG("weta_store_start_read(%d) failed\r\n", pWeta->startAddress);
		pWeta->states.runRequest = STOPPED;
		return false;
	}
    else
    {
        DEBUGMSG("weta_store_start_read(%d) succeeded.\r\n", pWeta->startAddress);
    }
	return true;
}

//*******************************************************************************
void WETAFUNCATTR
exec_cleanup(Weta* pWeta)
{

}

//*******************************************************************************
void WETAFUNCATTR
exec_loop(Weta* pWeta)
{
	DEBUGMSG( "exec_loop()\r\n");

	while (pWeta->states.runRequest == RUNNING)
	{
		exec_loop_body(pWeta);
	}
	pWeta->states.machineState = WETA_READY;
	pWeta->states.runRequest = STOPPED;
}
//*******************************************************************************
void WETAFUNCATTR
exec_loop_body(Weta* pWeta)
{
	//char szMsg[64];

	if (pWeta->states.runRequest == RUNNING)
	{
		//WetaCodePtr loc = pWeta->regs.pc;
		if (weta_store_read_byte(pWeta->store,pWeta->regs.pc, &pWeta->regs.opCode) == 0)
		{
            DEBUGMSG("Unable to read byte from code store.\n");
			weta_reset(pWeta);
			return;
		}
        DEBUGMSG("weta_store_read_byte(%d) = %d\r\n", pWeta->regs.pc, pWeta->regs.opCode);
		pWeta->regs.pc++;

		//sprintf(szMsg, "weta_store_read_byte(%d) = %d\r\n", loc, pWeta->regs.opCode);
		//weta_debug(pWeta, szMsg);
		switch (pWeta->regs.opCode)
		{
/*
#ifdef INCLUDE_LIB_IO
			case OP_IO:
				_ioLib.interpret();
				break;
#endif
#ifdef INCLUDE_LIB_MATH
			case OP_MATH:
				_mathLib.interpret();
				break;
#endif
#ifdef INCLUDE_LIB_COMM
			case OP_COMM:
				_commLib.interpret();
				break;
#endif
*/
			case OP_BYTE:
			case OP_INT8:
			case OP_SPAN:
				{
					uint8_t value;
					//WetaCodePtr loc = pWeta->regs.pc; // Debugging
					if (weta_store_read_byte(pWeta->store, pWeta->regs.pc, &value) == sizeof(value))
					{
						//sprintf(szMsg, "OP_BYTE @ %d = %d\r\n", loc, value);
						//weta_debug(pWeta, szMsg);
						weta_stack_pushUint8(pWeta->stack, value);
						pWeta->regs.pc++;
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;


			case OP_SHORT:
			case OP_UINT16:
				{
					uint16_t value;
					if (weta_store_read_uint16(pWeta->store, pWeta->regs.pc, &value) == sizeof(value))
					{
						//sprintf(szMsg, "OP_SHORT (%d)\r\n", value);
						//weta_debug(pWeta, szMsg);
						weta_stack_pushUint16(pWeta->stack, value);
						pWeta->regs.pc += sizeof(value);
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;

			case OP_GLOBAL:
				{
					WetaStackPtr value;
					if (weta_store_read_stackptr(pWeta->store, pWeta->regs.pc, &value) == sizeof(value))
					{
						weta_stack_pushStackPtr(pWeta->stack, value);
						pWeta->regs.pc += sizeof(value);
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;

			case OP_INT32:
			case OP_UINT32:
				{
					uint32_t value;
					if (weta_store_read_uint32(pWeta->store, pWeta->regs.pc, &value) == sizeof(value))
					{
#ifdef SUPPORT_32BIT
						weta_stack_pushUint32(pWeta->stack, value);
#else
						weta_stack_pushUint16(pWeta->stack, (value >> 16) & 0xFFFF);	// High 16 bits
						weta_stack_pushUint16(pWeta->stack, value & 0xFFFF);	// Low 16 bits
#endif
						pWeta->regs.pc += sizeof(value);
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;

#ifdef SUPPORT_FLOAT
			case OP_FLOAT:
				{
					float value;
					if (weta_store_read_float(pWeta->store, pWeta->regs.pc, &value) == sizeof(value))
					{
						weta_stack_pushFloat(pWeta->stack, value);
						pWeta->regs.pc += sizeof(value);
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;
#endif
#ifdef SUPPORT_DOUBLE
			case OP_DOUBLE:
				{
					double value;
					if (weta_store_read_double(pWeta->store, pWeta->regs.pc, &value) == sizeof(value))
					{
						weta_stack_pushDouble(pWeta->stack, value);
						pWeta->regs.pc += sizeof(value);
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;
#endif
			case OP_BOOL:
				{
					uint8_t value;
					if (weta_store_read_byte(pWeta->store, pWeta->regs.pc, &value) == 1)
					{
						weta_stack_pushUint8(pWeta->stack, (uint8_t)!!value);
						pWeta->regs.pc++;
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;

			case OP_CPTR:
				{
					WetaCodePtr value;
					if (weta_store_read_codePtr(pWeta->store, pWeta->regs.pc, &value) == sizeof(value))
					{
						weta_stack_pushCodePtr(pWeta->stack, value);
						pWeta->regs.pc += sizeof(value);
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;

#ifdef SUPPORT_STRING
			case OP_STRING:
				{
						// ***KLUDGE WARNING***
						// Borrowing unused (hopefully) stack space as a buffer!
						// (To avoid having to allocate another buffer.
					uint8_t* psz = (uint8_t*)weta_stack_getTopAddress(pWeta->stack);

					WetaCodePtr bytesRead = weta_store_read_string(pWeta->store, pWeta->regs.pc, psz, 255);
					if (bytesRead > 0)
					{
						weta_stack_pushString(pWeta->stack, psz);
						pWeta->regs.pc += bytesRead;
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;

			case OP_ASCII:
				{
					uint8_t* psz;
					weta_stack_topString(pWeta->stack, &psz);
					uint8_t ascii = psz[0];
					weta_stack_popString(pWeta->stack);
					weta_stack_pushUint8(pWeta->stack, ascii);
				}
				break;

			case OP_STRLEN:
				{
					uint8_t* psz;
					weta_stack_topString(pWeta->stack, &psz);
					uint8_t len = strlen((char*)psz);
					weta_stack_popString(pWeta->stack);
					weta_stack_pushUint8(pWeta->stack, len);
				}
				break;
#endif
			case OP_BEEP:
				DEBUGMSG( "OP_BEEP\r\n");
				hw_buzzer_beep(pWeta->hal);
				break;

			case OP_LEDON:
                DEBUGMSG("OP_LEDON\n");
#ifdef GPIO_INDEX_LED
                weta_led(pWeta->hal, true);
#endif
				break;

			case OP_LEDOFF:
                DEBUGMSG("OP_LEDOFF\n");
#ifdef GPIO_INDEX_LED
                weta_led(pWeta->hal, false);
				break;
#endif

			case OP_WITHINT8:
			case OP_WITHUINT8:
			case OP_WITHINT16:
			case OP_WITHUINT16:
			case OP_WITHBOOL:
			case OP_WITHPTR:
#ifdef SUPPORT_32BIT
			case OP_WITHINT32:
			case OP_WITHUINT32:
#endif
#ifdef SUPPORT_FLOAT
			case OP_WITHFLOAT:
#endif
#ifdef SUPPORT_DOUBLE
			case OP_WITHDOUBLE:
#endif
#ifdef SUPPORT_STRING
			case OP_WITHSTRING:
#endif
				pWeta->regs.withCode = pWeta->regs.opCode;
				break;

			case OP_LOCAL:
				{
					WetaStackPtr varOffset;
					if (weta_store_read_uint16(pWeta->store, pWeta->regs.pc, (uint16_t*)&varOffset) == sizeof(uint16_t))
					{
						weta_stack_pushStackPtr(
							pWeta->stack,
							(WetaStackPtr)pWeta->regs.localFrame.top + varOffset);
						pWeta->regs.pc += sizeof(uint16_t);
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;

			case OP_PARAM:
				{
					WetaStackPtr varOffset;
					if (weta_store_read_uint16(pWeta->store, pWeta->regs.pc, (uint16_t*)&varOffset) == sizeof(uint16_t))
					{
						//sprintf(szMsg, "OP_PARAM (%d)\r\n", varOffset);
						//weta_debug(pWeta, szMsg);
							// We have an index into the parameters, which were put on
							// the stack in reverse order before the function call.
							// Calculate the absolute stack offset using the local
							// stack frame offset.
							// Also, the total size of the arguments was pushed last,
							// so we need to step past that too.
							// TODO: Check against the total argument size.
						weta_stack_pushStackPtr(
							pWeta->stack,
							getArgsLocation(&pWeta->regs)
							- sizeof(uint8_t) 	// Size of args
							- varOffset		// Offset to the required param
						);
						pWeta->regs.pc += sizeof(uint16_t);
					}
					else
					{
						weta_reset(pWeta);
					}
				}
				break;

			case OP_BLOCK:
				{
					descendBlock(&pWeta->regs);	// Shift the flag to the next block bit
					uint16_t blockLength;
						// Push address of next instruction (following the block
						// length data)
					weta_stack_pushCodePtr(
						pWeta->stack,
						(WetaCodePtr)pWeta->regs.pc + sizeof(uint16_t));
					weta_store_read_uint16(pWeta->store, pWeta->regs.pc, &blockLength);
						// Step past the block (tests there will bring execution back)
					pWeta->regs.pc += blockLength;

				}
				break;

			case OP_EOB:
				setBlockExecuted(&pWeta->regs);	// Set the bit to indicate that this block has executed
				break;

			case OP_DO:
				{
					// OP_DO is like OP_BLOCK except that execution falls through to
					// the top of the block of code unconditionally rather than jump
					// to the end where some condition is tested.
					// Need to:
					//  - Push the address that the following OP_BLOCK code would
					//    push if it had been allowed to execute.
					//  - Step past the:
					//	  - The OP_BLOCK code (uint8_t)
					//    - The block size (uint16_t)
					// After going through the code block it should jump back to
					// the beginning of the OP_BLOCK and loop as usual.
					descendBlock(&pWeta->regs);	// Shift the flag to the next block bit (normally done by OP_BLOCK)
					WetaCodePtr startOfBlock =
						(WetaCodePtr)pWeta->regs.pc + sizeof(uint8_t) +sizeof(uint16_t);
					weta_stack_pushCodePtr(pWeta->stack, startOfBlock);
					pWeta->regs.pc = startOfBlock;
				}
				break;

			case OP_WAITUNTIL:
				{
					bool condition = false;
						// If the block has executed then (and only then) a
						// boolean test result has been pushed. Get it.
					if (hasBlockExecuted(&pWeta->regs))
						weta_stack_popUint8(pWeta->stack, (uint8_t*)&condition);

						// Get the address of the start of the block
					WetaCodePtr blockAddr;
					weta_stack_popCodePtr(pWeta->stack, &blockAddr);
					if (condition)
					{
							// The condition has tested true. Leave.
						ascendBlock(&pWeta->regs);
					}
					else
					{
							// Not true yet. Try again.
						weta_stack_pushCodePtr(pWeta->stack, blockAddr);
						pWeta->regs.pc = blockAddr; // Back to start of block
						if (hasBlockExecuted(&pWeta->regs))
							hw_time_waitms(10); // Prevent a tight loop
					}
				}
				break;

			case OP_WHILE:
				{
					bool  condition;
					WetaCodePtr blockAddr;
					weta_stack_popUint8(pWeta->stack, (uint8_t*)&condition);
					//_stack.pop(blockAddr);
					if (condition) // if true then go to the block start address
					{
						weta_stack_topCodePtr(pWeta->stack, &blockAddr);
						pWeta->regs.pc = blockAddr;
					}
					else
					{
						weta_stack_popCodePtr(pWeta->stack, &blockAddr); // Throw it away
						ascendBlock(&pWeta->regs);	// Finished with this block now
					}
				}
				break;

			case OP_REPEAT:
				{
					WetaCodePtr blockAddr;
					uint16_t max;
					weta_stack_popCodePtr(pWeta->stack, &blockAddr);
					weta_stack_popUint16(pWeta->stack, &max);
					uint16_t repcount;
					if (!hasBlockExecuted(&pWeta->regs)) // First time around?
					{
						repcount = 1;
						weta_stack_pushUint16(pWeta->stack, repcount);
							// point to the counter we just pushed
						WetaStackPtr slot = weta_stack_getTop(pWeta->stack);
							// Save outer loop's repcount pointer
						weta_stack_pushStackPtr(pWeta->stack, pWeta->regs.repcountLocation);
							// Set it to ours
						pWeta->regs.repcountLocation = slot;
					}
					else
					{
						weta_stack_getUint16(pWeta->stack, pWeta->regs.repcountLocation, &repcount); // Get counter value
						repcount++;
					}
					if (repcount <= max)
					{
						weta_stack_setUint16(pWeta->stack, pWeta->regs.repcountLocation, repcount);
						weta_stack_pushUint16(pWeta->stack, max);
						weta_stack_pushCodePtr(pWeta->stack, blockAddr);
						pWeta->regs.pc = blockAddr; // Back to start of block
					}
					else
					{
							// Restore the outer loop's repcount pointer
						weta_stack_popStackPtr(pWeta->stack, &pWeta->regs.repcountLocation);
						weta_stack_popUint16(pWeta->stack, &repcount);	// Dispose of counter
						ascendBlock(&pWeta->regs);	// Finished with this block now
					}
				}
				break;

			case OP_REPCOUNT:
				{
					uint16_t repcount;
					weta_stack_getUint16(pWeta->stack, pWeta->regs.repcountLocation, &repcount);
					weta_stack_pushUint16(pWeta->stack, repcount);
				}
				break;

			case OP_FOR:
				{
						// The counter variable has already been set to the from
						// value, so the from value isn't on the stack.
					WetaCodePtr  blockAddr;
					int16_t      step, to, from;
					WetaStackPtr counterOff;
					int16_t      counterVal;
					weta_stack_popCodePtr(pWeta->stack, &blockAddr);
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&step);
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&to);
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&from);
					weta_stack_popStackPtr(pWeta->stack, &counterOff);
					weta_stack_getUint16(pWeta->stack, counterOff, (uint16_t*)&counterVal);
					bool keepGoing;
						// See if this is the first time around
					if (!hasBlockExecuted(&pWeta->regs))
					{
						counterVal = from;
						keepGoing = (step > 0) ? (counterVal <= to) : (counterVal >= to);
					}
					else
					{
						// If step > 0 then assume from < to otherwise assume from > to
						keepGoing = (step > 0) ? (counterVal < to) : (counterVal > to);
						counterVal += step;
					}
					if (keepGoing)
					{
						weta_stack_setUint16(pWeta->stack, counterOff, (uint16_t)counterVal);
						pWeta->regs.pc = blockAddr; // reiterate
						weta_stack_pushStackPtr(pWeta->stack, counterOff);	// Var offset
						weta_stack_pushUint16(pWeta->stack, (uint16_t)from);	// to
						weta_stack_pushUint16(pWeta->stack, (uint16_t)to);	// to
						weta_stack_pushUint16(pWeta->stack, (uint16_t)step);	// step
						weta_stack_pushCodePtr(pWeta->stack, blockAddr);
					}
					else
					{
						ascendBlock(&pWeta->regs);
					}
				}
				break;


			case OP_IF:
				{
						// If it's the first time through then check the
						// condition
					if (!hasBlockExecuted(&pWeta->regs))
					{
						WetaCodePtr blockAddr;
						bool     condition;
						weta_stack_popCodePtr(pWeta->stack, &blockAddr);  // Block initial address
						weta_stack_popUint8(pWeta->stack, (uint8_t*)&condition); // argument to test
						if (condition)
						{
							pWeta->regs.pc = blockAddr;	// Go to start of block
						}
						else
						{
							ascendBlock(&pWeta->regs);
						}
					}
					else

					{
						ascendBlock(&pWeta->regs);
					}
				}
				break;

				// IFELSE starts with address of THEN and ELSE lists (and
				// CONDITIONAL) on the stack. CONDITIONAL is tested and
				// appropriate list is run.
			case OP_IFELSE:
				{
					WetaCodePtr elseBlock;
					weta_stack_popCodePtr(pWeta->stack, &elseBlock);  // ELSE block start
						// Note that descendBlock() will have been called twice
						// the first time around (once for each block as it was skipped).
					ascendBlock(&pWeta->regs); // Remove the else block flag...
						// ...and use the then block flag for both purposes
					if (!hasBlockExecuted(&pWeta->regs))
					{
						WetaCodePtr thenBlock;
						bool     condition;
						weta_stack_popCodePtr(pWeta->stack, &thenBlock); // THEN block start
						weta_stack_popUint8(pWeta->stack, (uint8_t*)&condition); 	  // argument to test
						if (condition)
						{
							pWeta->regs.pc = thenBlock; // Go to then block
								// The ELSE address will get pushed again when
								// execution falls into the ELSE block after
								// exiting the THEN block.
								// Another else block will be descended into
								// as well, and ascended away again above.
								// The eob code will be encountered at the end
								// of the then block, and that will set the
								// executed flag.
						}
						else
						{
							pWeta->regs.pc = elseBlock;	  // Go to else block
							weta_stack_pushCodePtr(pWeta->stack, (WetaCodePtr)0); // Push fake ELSE to balance
								// Borrow the then block flag and set it now as
								// executed since it won't actually be set
								// otherwise.
							setBlockExecuted(&pWeta->regs);
								// Descend the else block now, as
								// this also won't be done in the block's code.
							descendBlock(&pWeta->regs);
						}
					}
					else
					{
						//weta_stack_popCodePtr(_stack, &elseBlock);  // dispose of unrequired address
						ascendBlock(&pWeta->regs); // ie. the then block
					}
				}
				break;

			case OP_PUSH:
				{
					uint8_t amount;
					weta_stack_popUint8(pWeta->stack, &amount);
					weta_stack_pushn(pWeta->stack, (WetaStackPtr)amount);
				}
				break;

			case OP_POP:
				{
					uint8_t amount;
					weta_stack_popUint8(pWeta->stack, &amount);
					weta_stack_popn(pWeta->stack, (WetaStackPtr)amount);
				}
				break;

			case OP_CHKPOINT:
					// Save the stack context (top etc.) This is necessary
					// because checkpoints can be recursive.
				weta_stack_pushStackState(pWeta->stack, &pWeta->regs.checkPoint);
					//i.e. OF the stack, not from the stack
				weta_stack_getStackState(pWeta->stack, &pWeta->regs.checkPoint);
				break;

			case OP_ROLLBACK:
					// Set stack to where it was at the checkpoint (throw away
					// anything that was put onto the stack since then).
				weta_stack_setStackState(pWeta->stack, &pWeta->regs.checkPoint);
					// Now the state that was saved can be restored
				weta_stack_popStackState(pWeta->stack, &pWeta->regs.checkPoint);
				break;

			case OP_CALL:
				{
					WetaCodePtr procAddr;
					weta_stack_popCodePtr(pWeta->stack, &procAddr);	// Get the function location
						// Save the current code location for the return
					weta_stack_pushCodePtr(pWeta->stack, (WetaCodePtr)pWeta->regs.pc);
					pWeta->regs.pc = procAddr;  // Now jump to the function
				}
				break;

			case OP_BEGIN:
				pushRegisters(pWeta);	// Save state of the caller

					// Block (e.g. if, while, for etc.) states (ie. executed)
					// have been saved byt pushRegisters() above. Reset for this procedure.
				pWeta->regs.blockDepthMask   = 0;
				pWeta->regs.blocksExecuted   = 0;
				//weta_stack_getStackState(_stack, &pWeta->regs.localFrame); // = weta_stack_getTop(_stack);

				break;

			case OP_ENTER:
					// Save the caller's local frame then get the frame for the
					// current procedure. MUST be called immediately after
					// OP_BEGIN if there are EITHER call arguments or local
					// variables.
				weta_stack_pushStackState(pWeta->stack, &pWeta->regs.localFrame);
					// i.e. OF the stack, not from the stack
				weta_stack_getStackState(pWeta->stack, &pWeta->regs.localFrame);
				break;

			case OP_LEAVE:
					// Unwind the stack to the beginning of the local frame.
					// Call immediately before OP_RETURN if OP_ENTER was called
					// for the current procedure.
				weta_stack_setStackState(pWeta->stack, &pWeta->regs.localFrame);
				weta_stack_popStackState(pWeta->stack, &pWeta->regs.localFrame);
				break;

			case OP_RETURN:
				{
						//Unwind the stack to the beginning of the local frame
					//weta_stack_setStackState(_stack, &pWeta->regs.localFrame);
					popRegisters(pWeta);

					WetaCodePtr returnAddr;
					weta_stack_popCodePtr(pWeta->stack, &returnAddr);	// Get the return address
					//_stack.pop(pWeta->regs.argsLoc);	// Restore the param location for the calling function
					pWeta->regs.pc = returnAddr;
				}
				break;

			case OP_EXIT:
				weta_rewind(pWeta);
				break;

			case OP_LOOP:
				{
					WetaCodePtr blockAddr;
					weta_stack_topCodePtr(pWeta->stack, &blockAddr);
						// Go back to the top of the block unconditionally
					pWeta->regs.pc = blockAddr;

				}
				break;


			case OP_WAIT:
				{
					DEBUGMSG( "OP_WAIT\r\n");
					uint16_t millis;
					weta_stack_popUint16(pWeta->stack, &millis);
#ifdef COOPERATIVE_RTOS
                    start_wait(pWeta, millis);
#else
					hw_time_waitms(millis);
#endif
				}
				break;

			case OP_TIMER:

				weta_stack_pushUint16(
                    pWeta->stack,
                    (uint16_t)((hw_time_now()- pWeta->timerStart)/MILLIS_PER_TICK));
				break;

			case OP_RESETT:
				pWeta->timerStart = hw_time_now();
				break;

			case OP_RANDOM:
				weta_stack_pushUint16(pWeta->stack, (uint16_t)((32767 * rand()) / RAND_MAX));
				break;

			case OP_RANDOMXY:
				{
					int16_t x;
					int16_t y;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&y);
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&x);
					 	// Nasty hack.
						// Assumes y > x.
						// Use the difference as the range, then add x to shift
						// the result back into the desired range.
						// Note that though the result is cast as unsigned,
						// when it is popped it can still be interpreted as signed.
					weta_stack_pushUint16(
						pWeta->stack,
						(uint16_t)((((y-x) * rand()) / RAND_MAX) + x)
					);
				}
				break;
// Turtle commands
			case OP_FORWARD:
				{
#ifdef	SUPPORT_STEPPERS
					pWeta->states.machineState = STEPPER;
#endif
					int16_t mm;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&mm);
					//os_sprintf(szMsg, "OP_FORWARD %d mm\r\n", mm);
					//weta_debug(pWeta, szMsg);
					weta_turtle_forward(pWeta->hal, mm);
				}
				break;

			case OP_BACKWARD:
				{
#ifdef	SUPPORT_STEPPERS
					pWeta->states.machineState = STEPPER;
#endif
					int16_t mm;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&mm);
					//os_sprintf(szMsg, "OP_BACKWARD %d mm\r\n", mm);
					//weta_debug(pWeta, szMsg);
					weta_turtle_backward(pWeta->hal, mm);
				}
				break;

			case OP_LEFT:
				{
#ifdef	SUPPORT_STEPPERS
					pWeta->states.machineState = STEPPER;
#endif
					int16_t degrees;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&degrees);
					weta_turtle_left(pWeta->hal, degrees);
				}
				break;

			case OP_RIGHT:
				{
#ifdef	SUPPORT_STEPPERS
					pWeta->states.machineState = STEPPER;
#endif
					int16_t degrees;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&degrees);
					weta_turtle_right(pWeta->hal, degrees);
				}
				break;

			case OP_PENUP:
				{
					weta_turtle_penup();
				}
				break;

			case OP_PENDOWN:
				{
					weta_turtle_pendown();
				}
				break;

#ifdef SUPPORT_MOTORS
			case OP_MOTORS:
				{
					DEBUGMSG( "OP_MOTORS\r\n");
					uint8_t selected;
					weta_stack_popUint8(pWeta->stack, &selected);
					weta_motor_select(pWeta->hal, selected);
				}
				break;

			case OP_THISWAY:
				DEBUGMSG( "OP_THISWAY\r\n");
				weta_motor_dir(pWeta->hal, MOTOR_THIS_WAY);
				break;


			case OP_THATWAY:
				DEBUGMSG( "OP_THATWAY\r\n");
                weta_motor_dir(pWeta->hal, MOTOR_THAT_WAY);
				break;

			case OP_RD:
				DEBUGMSG( "OP_RD\r\n");
                weta_motor_rd(pWeta->hal);
				break;

			case OP_SETPOWER:
				{
					DEBUGMSG( "OP_SETPOWER\r\n");
					uint8_t power;
					weta_stack_popUint8(pWeta->stack, &power);
					//sprintf(szMsg, "OP_SETPOWER to %d\r\n", power);
					//weta_debug(pWeta, szMsg);
					weta_motor_power(pWeta->hal, power);
				}
				break;


			case OP_BRAKE:
				DEBUGMSG( "OP_BRAKE\r\n");
				weta_motor_brake(pWeta->hal, true);
				break;

			case OP_ON:
				DEBUGMSG( "OP_ON\r\n");
				weta_motor_on(pWeta->hal, true);
				break;

			case OP_ONFOR:
				{
					DEBUGMSG( "OP_ONFOR\r\n");
					uint16_t tenths;
					weta_stack_popUint16(pWeta->stack, &tenths);
                    weta_motor_on(pWeta->hal, true);
					hw_time_waitms(tenths * 100);
                    weta_motor_on(pWeta->hal, false);
				}
				break;

			case OP_OFF:
				DEBUGMSG( "OP_OFF\r\n");
                weta_motor_on(pWeta->hal, false);
				break;
#endif
#ifdef SUPPORT_SERVOS
			case OP_SERVOS:
				{
					uint8_t selected;
					weta_stack_popUint8(pWeta->stack, &selected);
					//sprintf(szMsg, "OP_SERVOS (%d)\r\n", selected);
					//weta_debug(pWeta, szMsg);
					weta_servo_select(pWeta->hal, selected);
				}
				break;

			case OP_SETSVH:
				{
					int16_t heading;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&heading);
					//sprintf(szMsg, "OP_SETSVH (%d)\r\n", heading);
					//weta_debug(pWeta, szMsg);
					weta_servo_pos(pWeta->hal, heading);
				}
				break;

			case OP_SVR:
			case OP_SVL:
				break;	// TO DO!
#endif
#ifdef SUPPORT_ADC
			case OP_SENSOR1:
			case OP_SENSOR2:
				{
					int16_t value = 0;
					weta_adc_get(pWeta->hal, pWeta->regs.opCode - OP_SENSOR1, &value);
					//sprintf(szMsg, "OP_SENSOR(%d) = %d\r\n", pWeta->regs.opCode - OP_SENSOR1, value);
					//weta_debug(pWeta, szMsg);
					weta_stack_pushUint16(
						pWeta->stack,
						(uint16_t)value
					);
				}
				break;

			case OP_SENSOR3:
			case OP_SENSOR4:
			case OP_SENSOR5:
			case OP_SENSOR6:
			case OP_SENSOR7:
			case OP_SENSOR8:
				{
					int16_t value = 0;
					weta_adc_get(pWeta->hal, pWeta->regs.opCode - OP_SENSOR3 + 2, &value);
					//sprintf(szMsg, "OP_SENSOR(%d) = %d\r\n", pWeta->regs.opCode - OP_SENSOR3 + 2, value);
					//weta_stack_pushUint16(pWeta->stack, (uint16_t)value);
				}
				break;

			case OP_AIN:
				{
					//Serial.print("ain ");
					uint8_t input;
					weta_stack_popUint8(pWeta->stack, &input);
					int16_t value = 0;
					weta_adc_get(pWeta->hal, input, &value);
					weta_stack_pushUint16(pWeta->stack,(uint16_t)value);
				}
				break;
#endif
#ifdef SUPPORT_DAC
			case OP_AOUT:
				{
					uint8_t output;
					uint8_t value;
					weta_stack_popUint8(pWeta->stack, &output);
					weta_stack_popUint8(pWeta->stack, &value);
					weta_dac_set(pWeta->hal, output, (uint16_t)value);
				}
				break;
#endif
#ifdef SUPPORT_GPIO
			case OP_SWITCH1:
			case OP_SWITCH2:
				{
					bool value = false;
#ifdef GPIO_INDEX_SWITCH1
                    weta_digital_in(
						pWeta->hal,
						pWeta->regs.opCode - (uint8_t)OP_SWITCH1 + (uint8_t)GPIO_INDEX_SWITCH1,
						&value
					);
#endif
					//sprintf(szMsg, "OP_SWITCH(%d) = %d\r\n", pWeta->regs.opCode - OP_SWITCH1 + SWITCH1, value);
					//weta_debug(pWeta, szMsg);
					weta_stack_pushUint8(pWeta->stack, (uint8_t)value);
				}
				break;

			case OP_SWITCH3:
			case OP_SWITCH4:
			case OP_SWITCH5:
			case OP_SWITCH6:
			case OP_SWITCH7:
			case OP_SWITCH8:
				{
					bool value = false;
#ifdef GPIO_INDEX_SWITCH3
                hw_gpio_get_i(
						&pWeta->hal->gpio,
						pWeta->regs.opCode - (uint8_t)OP_SWITCH3 + (uint8_t)GPIO_INDEX_SWITCH3,
						&value
					);
					//sprintf(szMsg, "OP_SWITCH(%d) = %d\r\n", pWeta->regs.opCode - OP_SWITCH3 + SWITCH3, value);
					//weta_debug(pWeta, szMsg);
#endif
					weta_stack_pushUint8(pWeta->stack, (uint8_t)value);
				}
				break;
            case OP_DIN:
				{
					uint8_t input;
					weta_stack_popUint8(pWeta->stack, &input);
					bool value = false;
                    weta_digital_in(pWeta->hal, input, &value);
					weta_stack_pushUint8(pWeta->stack,(uint8_t)value);
				}
				break;

			case OP_DOUT:
				{
					uint8_t output;
					uint8_t value;
					weta_stack_popUint8(pWeta->stack, &output);
					weta_stack_popUint8(pWeta->stack, &value);
                    weta_digital_out(pWeta->hal, output, value);
				}
				break;
#endif
			case OP_NOT:
				{
					uint8_t rhs;
					weta_stack_popUint8(pWeta->stack, &rhs);
					weta_stack_pushUint8(pWeta->stack, (uint8_t)!rhs);
				}
				break;

			case OP_AND:
				{
					uint8_t rhs, lhs;
					weta_stack_popUint8(pWeta->stack, &rhs);
					weta_stack_popUint8(pWeta->stack, &lhs);
					weta_stack_pushUint8(pWeta->stack, (uint8_t)(!!lhs && !!rhs));
				}
				break;

			case OP_OR:
				{
					uint8_t rhs, lhs;
					weta_stack_popUint8(pWeta->stack, &rhs);
					weta_stack_popUint8(pWeta->stack, &lhs);
					weta_stack_pushUint8(pWeta->stack, (uint8_t)(!!lhs || !!rhs));
				}
				break;

			case OP_XOR:
				{
					uint8_t rhs, lhs;
					weta_stack_popUint8(pWeta->stack, &rhs);
					weta_stack_popUint8(pWeta->stack, &lhs);
						// No logical XOR operator in C (only bitwise)
					weta_stack_pushUint8(
						pWeta->stack,
						(uint8_t)((!!lhs && !rhs) || (!lhs && !!rhs))
					);
				}
				break;
    		case OP_BTOS:
				{
					int8_t value;
					weta_stack_popUint8(pWeta->stack, (uint8_t*)&value);
					weta_stack_pushUint16(pWeta->stack,(uint16_t)value);
				}
				break;
			case OP_UBTOS:
				{
					uint8_t value;
					weta_stack_popUint8(pWeta->stack, &value);
					weta_stack_pushUint16(pWeta->stack,(uint16_t)value);
				}
				break;
			case OP_STOB:	// and OP_USTOB
				{
					uint16_t value;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&value);
					weta_stack_pushUint8(pWeta->stack, (uint8_t)value);

				}
				break;
#ifdef SUPPORT_32BIT
			case OP_BTOI:
				{
					int8_t value;
					weta_stack_popUint8(pWeta->stack, (uint8_t*)&value);
					weta_stack_pushUint32(pWeta->stack,(uint32_t)value);
				}
				break;
			case OP_UBTOI:
				{
					uint8_t value;
					weta_stack_popUint8(pWeta->stack, &value);
					weta_stack_pushUint32(pWeta->stack,(uint32_t)value);
				}
				break;
			case OP_STOI:
				{
					int16_t value;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&value);
					weta_stack_pushUint32(pWeta->stack,(uint32_t)value);
				}
				break;
			case OP_USTOI:
				{
					uint16_t value;
					weta_stack_popUint16(pWeta->stack, &value);
					weta_stack_pushUint32(pWeta->stack,(uint32_t)value);
				}
				break;
			case OP_ITOB: // and OP_UITOB
				{
					int32_t value;
					weta_stack_popUint32(pWeta->stack, (uint32_t*)&value);
					weta_stack_pushUint8(pWeta->stack, (uint8_t)value);
				}
				break;
			case OP_ITOS: //and OP_UITOS

				{
					int32_t value;
					weta_stack_popUint32(pWeta->stack, (uint32_t*)&value);
					weta_stack_pushUint16(pWeta->stack, (uint16_t)value);
				}
				break;
#endif
#ifdef SUPPORT_FLOAT
			case OP_BTOF:
				{
					int8_t value;
					weta_stack_popUint8(pWeta->stack, (uint8_t*)&value);
					weta_stack_pushFloat(pWeta->stack, (float)value);
				}
				break;
			case OP_UBTOF:
				{
					uint8_t value;
					weta_stack_popUint8(pWeta->stack, &value);
					weta_stack_pushFloat(pWeta->stack, (float)value);
				}
				break;
			case OP_STOF:
				{
					int16_t value;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&value);
					weta_stack_pushFloat(pWeta->stack, (float)value);
				}
				break;
			case OP_USTOF:
				{
					uint16_t value;
					weta_stack_popUint16(pWeta->stack, &value);
					weta_stack_pushFloat(pWeta->stack, (float)value);
				}
				break;
			case OP_FTOB:
				{
					float value;
					weta_stack_popFloat(pWeta->stack, &value);
					weta_stack_pushUint8(pWeta->stack, (uint8_t)value);
				}
				break;
			case OP_FTOS:
				{
					float value;
					weta_stack_popFloat(pWeta->stack, &value);
					weta_stack_pushUint16(pWeta->stack, (uint16_t)value);
				}
				break;
#endif
#ifdef SUPPORT_DOUBLE
			case OP_BTOD:
				{
					int8_t value;
					weta_stack_popUint8(pWeta->stack, (uint8_t*)&value);
					weta_stack_pushDouble(pWeta->stack, (double)value);
				}
				break;
			case OP_UBTOD:
				{
					uint8_t value;
					weta_stack_popUint8(pWeta->stack, &value);
					weta_stack_pushDouble(pWeta->stack, (double)value);
				}
				break;
			case OP_STOD:
				{
					int16_t value;
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&value);
					weta_stack_pushDouble(pWeta->stack, (double)value);
				}
				break;
			case OP_USTOD:
				{
					uint16_t value;
					weta_stack_popUint8(pWeta->stack, (uint8_t*)&value);
					weta_stack_pushDouble(pWeta->stack, (double)value);
				}
				break;
			case OP_DTOB:
				{
					double value;
					weta_stack_popDouble(pWeta->stack, &value);
					weta_stack_pushUint8(pWeta->stack, (uint8_t)value);
				}
				break;
			case OP_DTOS:
				{
					double value;
					weta_stack_popDouble(pWeta->stack, &value);
					weta_stack_pushUint16(pWeta->stack, (uint16_t)value);
				}
				break;
#endif
#if defined(SUPPORT_DOUBLE) && defined(SUPPORT_32BIT)
			case OP_ITOD:
				{
					int32_t value;
					weta_stack_popUint32(pWeta->stack, (uint32_t*)&value);
					weta_stack_pushDouble(pWeta->stack, (double)value);
				}
				break;
			case OP_UITOD:
				{
					uint32_t value;
					weta_stack_popUint32(pWeta->stack, &value);
					weta_stack_pushDouble(pWeta->stack, (double)value);
				}
				break;
			case OP_DTOI:
				{
					double value;
					weta_stack_popDouble(pWeta->stack, &value);
					weta_stack_pushUint32(pWeta->stack, (uint32_t)value);
				}
				break;
#endif
#if defined(SUPPORT_DOUBLE) && defined(SUPPORT_FLOAT)
			case OP_FTOD:
				{
					float value;
					weta_stack_popFloat(pWeta->stack, &value);
					weta_stack_pushDouble(pWeta->stack, (double)value);
				}
				break;
			case OP_DTOF:
				{
					double value;
					weta_stack_popDouble(pWeta->stack, &value);
					weta_stack_pushFloat(pWeta->stack, (float)value);
				}
				break;
#endif
#if defined(SUPPORT_FLOAT) && defined(SUPPORT_32BIT)
			case OP_ITOF:
				{
					int32_t value;
					weta_stack_popUint32(pWeta->stack, (uint32_t*)&value);
					weta_stack_pushFloat(pWeta->stack, (float)value);
				}
				break;
			case OP_UITOF:
				{
					uint32_t value;
					weta_stack_popUint32(pWeta->stack, &value);
					weta_stack_pushFloat(pWeta->stack, (float)value);
				}
				break;
			case OP_FTOI:
				{
					float value;
					weta_stack_popFloat(pWeta->stack, &value);
					weta_stack_pushUint32(pWeta->stack, (uint32_t)value);
				}
				break;
#endif
			case OP_NEWRX:
				weta_stack_pushUint8(pWeta->stack, (uint8_t)weta_serial_available(pWeta->hal, 0));
				break;

			case OP_NEWRXN:
				{
					uint8_t port;
					weta_stack_popUint8(pWeta->stack, &port);
                    weta_stack_pushUint8(
                        pWeta->stack,
                        (uint8_t)weta_serial_available(pWeta->hal, port)
                    );
				}
				break;

			case OP_RXN:
				{
					//DEBUGMSG( "OP_RXN\r\n");
					uint8_t  port;
					uint16_t rxBuffLocation;
					uint8_t  rxNumBytes;
					int16_t timeout;
					weta_stack_popUint8(pWeta->stack, &port);
					weta_stack_popUint16(pWeta->stack, &rxBuffLocation);
					weta_stack_popUint8(pWeta->stack, &rxNumBytes);
					weta_stack_popUint16(pWeta->stack, (uint16_t*)&timeout);

                    uint8_t* buffAddr = (uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation);

                    uint8_t bytesRead = weta_serial_read(
                        pWeta->hal,
                        port,
                        buffAddr,  //(uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation),
                        rxNumBytes,
                        timeout
                    );
                    weta_stack_pushUint8(pWeta->stack, bytesRead);
                    /*
                    sprintf(szMsg, "hw_serial_read(%d, %d, %d, %d ) = %d\r\n",
                            port,
                            (uint8_t)*buffAddr,
                            rxNumBytes,
                            timeout,
                            bytesRead);
                    weta_debug(pWeta, szMsg);
						*/
					}
				break;

			case OP_TXN:
				{
					//DEBUGMSG( "OP_TXN\r\n");
					uint8_t  port;
					uint16_t txBuffLocation;
					uint8_t  txNumBytes;
					weta_stack_popUint8(pWeta->stack, &port);
					weta_stack_popUint16(pWeta->stack, &txBuffLocation);
					weta_stack_popUint8(pWeta->stack, &txNumBytes);
                    uint8_t* buffAddr = (uint8_t*)weta_stack_getStackAddress(
                        pWeta->stack,
                        txBuffLocation
                    );
                    //uint8_t rc = 42;

                    uint8_t bytesWritten = weta_serial_write(
                        pWeta->hal,
                        port,
                        buffAddr,  //(uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation),
                        txNumBytes
                    );
                    weta_stack_pushUint8(pWeta->stack, bytesWritten);
                    /*
                    sprintf(szMsg, "hw_serial_write(%d, %d, %d) = %d\r\n",
                            port,
                            (uint8_t)*buffAddr,
                            txNumBytes,
                            bytesWritten
                    );
                    weta_debug(pWeta, szMsg);
                    */
				}
				break;

			case OP_I2CSTART:
				//DEBUGMSG( "OP_I2CSTART\r\n");
				weta_i2c_start(pWeta->hal);
				break;

			case OP_I2CSTOP:
				//DEBUGMSG( "OP_I2CSTOP\r\n");
				weta_i2c_stop(pWeta->hal);
				break;

			case OP_I2CWRITE:
				{
					//DEBUGMSG( "OP_I2CWRITE\r\n");
					uint8_t  slaveAddr;
					uint32_t registerAddr = 0;
					uint8_t  registerAddrWidth;
					uint16_t txBuffLocation;
					uint8_t  txNumBytes;
					weta_stack_popUint8(pWeta->stack, &slaveAddr);
					weta_stack_popUint8(pWeta->stack, &registerAddrWidth);
					if (registerAddrWidth == 1)
						weta_stack_popUint8(pWeta->stack, (uint8_t*)&registerAddr);
					else if (registerAddrWidth == 2)
						weta_stack_popUint16(pWeta->stack, (uint16_t*)&registerAddr);
#ifdef SUPPORT_32BIT
					else if (registerAddrWidth == 3 || registerAddrWidth == 4)
						weta_stack_popUint32(pWeta->stack, &registerAddr);
#endif

					weta_stack_popUint16(pWeta->stack, &txBuffLocation);
					weta_stack_popUint8(pWeta->stack, &txNumBytes);

					uint8_t* buffAddr = (uint8_t*)weta_stack_getStackAddress(pWeta->stack, txBuffLocation);

					//uint8_t rc =
					weta_i2c_write(
                        pWeta->hal,
						slaveAddr,
						registerAddr,
						registerAddrWidth,
						buffAddr,   //(uint8_t*)weta_stack_getStackAddress(pWeta->stack, txBuffLocation),
						(uint32_t)txNumBytes
					);
					/*
					sprintf(szMsg, "hw_i2c_write(%d, %d, %d, %d, %d) = %d\r\n",
							slaveAddr,
							registerAddr,
							registerAddrWidth,
							(uint8_t)*buffAddr,
							txNumBytes,
							rc);
					weta_debug(pWeta, szMsg);
					*/
				}
				break;

			case OP_I2CREAD:
				{
					DEBUGMSG( "OP_I2CREAD\r\n");
					uint8_t  slaveAddr;
					uint32_t registerAddr = 0;
					uint8_t  registerAddrWidth;
					uint16_t rxBuffLocation;
					uint8_t  rxNumBytes;
					weta_stack_popUint8(pWeta->stack, &slaveAddr);
					weta_stack_popUint8(pWeta->stack, &registerAddrWidth);
						// A register address width of 0 means that no register
						// address is popped off the stack
					if (registerAddrWidth == 1)
						weta_stack_popUint8(pWeta->stack, (uint8_t*)&registerAddr);
					else if (registerAddrWidth == 2)
						weta_stack_popUint16(pWeta->stack, (uint16_t*)&registerAddr);
#ifdef SUPPORT_32BIT
					else if (registerAddrWidth == 4)
						weta_stack_popUint32(pWeta->stack, &registerAddr);
#endif
					weta_stack_popUint16(pWeta->stack, &rxBuffLocation);
					weta_stack_popUint8(pWeta->stack, &rxNumBytes);

					uint8_t* buffAddr = (uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation);
					//uint8_t rc = 42;

					//uint8_t rc =
					weta_i2c_read(
                        pWeta->hal,
						slaveAddr,
						registerAddr,
						registerAddrWidth,
						buffAddr,  //(uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation),
						(uint32_t)rxNumBytes
					);
					/*
					sprintf(szMsg, "hw_i2c_read(%d, %d, %d, %d, %d) = %d\r\n",
							slaveAddr,
							registerAddr,
							registerAddrWidth,
							(uint8_t)*buffAddr,
							rxNumBytes,
							rc);
					weta_debug(pWeta, szMsg);
					*/
				}
				break;
#ifdef SUPPORT_32BIT
			case OP_I2CERR:
				{
					//Serial.println("---i2cerr---");
						// Atmel TWID API doesn't make error codes available.
						// Just return no error.
					weta_stack_pushUint32(pWeta->stack, 0);
				}
				break;
#endif

			case OP_RECORD:
			case OP_RECALL:
			case OP_RESETDP:
			case OP_SETDP:
			case OP_ERASE:

					// TODO!!!
				break;

			default:
					// All of the type specific codes are dealt with here
				if (!with_current_type(pWeta))
				{
					//beep();	// Just an indication for now.
					//Serial.print("unrecognised opcode: ");
					//Serial.println(pWeta->regs.opCode);
				}
				break;

		}
	}
}

WetaStackPtr WETAFUNCATTR
getArgsLocation(Registers* pRegs)
{
	return pRegs->localFrame.top
			- sizeof(uint8_t) * 2	//Block states
			//- sizeof(STACKSTATE)	// Saved checkpoint from outer context
			- sizeof(STACKSTATE)	// Saved localFrame from outer context
			- sizeof(WetaCodePtr);	// Return address
}

void WETAFUNCATTR
descendBlock(Registers* pRegs)
{
	if (pRegs->blockDepthMask == 0)
		pRegs->blockDepthMask = 1;
	else
		pRegs->blockDepthMask <<= 1;
}

void WETAFUNCATTR
ascendBlock(Registers* pRegs)
{
	pRegs->blocksExecuted &= ~pRegs->blockDepthMask; // Clear the executed flag
	pRegs->blockDepthMask >>= 1; // Back to the next block outwards
}

void WETAFUNCATTR
setBlockExecuted(Registers* pRegs)
{
	pRegs->blocksExecuted |= pRegs->blockDepthMask;
}

bool WETAFUNCATTR
hasBlockExecuted(Registers* pRegs)
{
	return (pRegs->blocksExecuted & pRegs->blockDepthMask) == pRegs->blockDepthMask;
}

void WETAFUNCATTR
pushRegisters(Weta* pWeta)
{
	weta_stack_pushUint8(pWeta->stack, pWeta->regs.blockDepthMask);
	weta_stack_pushUint8(pWeta->stack, pWeta->regs.blocksExecuted);
}

void WETAFUNCATTR
popRegisters(Weta* pWeta)
{
	weta_stack_popUint8(pWeta->stack, &pWeta->regs.blocksExecuted);
	weta_stack_popUint8(pWeta->stack, &pWeta->regs.blockDepthMask);
}

bool WETAFUNCATTR
with_current_type(Weta* pWeta)
{
	switch (pWeta->regs.withCode)
	{
	case OP_WITHINT8:	return with_int8(pWeta);
	case OP_WITHUINT8:	return with_uint8(pWeta);
	case OP_WITHINT16:	return with_int16(pWeta);
	case OP_WITHUINT16:	return with_uint16(pWeta);
	case OP_WITHBOOL:	return with_bool(pWeta);
	case OP_WITHPTR:	return with_stackptr(pWeta);
#ifdef SUPPORT_32BIT
	case OP_WITHINT32:	return with_int32(pWeta);
	case OP_WITHUINT32:	return with_uint32(pWeta);
#endif
#ifdef SUPPORT_FLOAT
	case OP_WITHFLOAT:	return with_float(pWeta);
#endif
#ifdef SUPPORT_DOUBLE
	case OP_WITHDOUBLE:	return with_double(pWeta);
#endif
#ifdef SUPPORT_STRING
	case OP_WITHSTRING:	return with_string(pWeta);
#endif

	}
	return false;
}

WetaCodePtr WETAFUNCATTR
verify_json_codes(const char* jsonArray)
{
	WetaCodePtr numNums = 0;
	char * cursor = strchr(jsonArray, '[');
	if (!cursor)
		return 0;	// No array, so no codes.

	cursor++;	// Step past the '['
	char* endptr = cursor;
	while (*endptr != ']')
	{
			// No need to know the number, just that one is there.
		strtol(cursor, &endptr, 0);
		if (endptr == cursor)
			break;
		cursor = endptr;
		cursor++;	// step past comma
		numNums++;	// increment count
	}
	return numNums;
}

#ifdef SUPPORT_JSON
bool WETAFUNCATTR
weta_program_json(Weta* pWeta, WetaStorage storage, WetaCodePtr address, const char* jsonArray)
{
    //pWeta->states.machineState = WETA_CONFIG;

    DEBUGMSG("weta_program_json()\n\r");
    DEBUGMSG(jsonArray);
	WetaCodePtr numCodes = verify_json_codes(jsonArray);
	DEBUGMSG("\n\rnumCodes = %d\n\r", numCodes);
	if (numCodes == 0)
		return false;
        // Initialise storage of the given type
    PSTORE store = weta_open_store(pWeta, storage);
    if (!store)
        return false;

	//pWeta->startAddress = address;
	DEBUGMSG("address = %d\n\r", address);

		// Now that we have the number of codes, we can start the write
		// operation.
	if (!weta_store_start_write(store, address, numCodes))
	{
		DEBUGMSG("weta_store_start_write() failed\r\n");
		return false;
	}

		// Now that we know that there is at least one code, we can parse the
		// the JSON array string and program each code as it is parsed.
		// Find the start of the JSON number array
	char * cursor = strchr(jsonArray, '[');
	cursor++;	// Step past the '['
	char* endptr = cursor;
	long nextCode;
    bool error = false;
    uint8_t written = 0;    // debugging
	while (*endptr != ']')
	{
		nextCode = strtol(cursor, &endptr, 0);
		if (endptr == cursor)
        {
            DEBUGMSG("Could not interpret a number  from %s\n\r", cursor);
            error = true;
            break;
        }
        DEBUGMSG("%ld\n\r", nextCode);
		if (!weta_store_write_byte(store, (uint8_t)nextCode))
		{
			DEBUGMSG("weta_store_write_byte() failed\r\n");
            error = true;
			break;
		}
        written++;
		cursor = endptr;
		cursor++;	// step past comma
	}
    DEBUGMSG("Finished write loop with %d codes written\r\n", written);
    if (error)
    {
        weta_store_close(store);
        return false;
    }
	else if (!weta_store_flush(store))
	{
		weta_store_close(store);
		return false;
	}
	return weta_start(pWeta, store, address);
}
#endif

