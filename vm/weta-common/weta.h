#ifndef __WETA_H__
#define __WETA_H__
/**
 * @file weta.h
 * @author Murray Lang
 * @brief 
 */
 
#include <weta_platform.h>
#include "hw/hw.h"
#include "weta_stack.h"
#include "store/weta_store.h"
#include "store/weta_store_flash.h"
#include "store/weta_store_ram.h"

/**
 * @brief Machine states of the VM as a whole
 */
typedef enum
{
    WETA_READY,
	WETA_CONFIG,
#ifdef COOPERATIVE_RTOS
    WETA_WAITING,
#endif
#ifdef SUPPORT_CRICKET
    WETA_COMM,
#endif
    WETA_RUN
#ifdef	SUPPORT_STEPPERS
    , STEPPER
#endif
} eMachineState;

#ifdef SUPPORT_CRICKET
/**
 * @brief Communications states
 */
typedef enum  { COMM_IDLE, COMM_STARTED, COMM_FINISHED, COMM_TIMEOUT } eCommState;

/**
 * @brief Cricket protocol commands
 */
typedef enum 
{
	cmdSetPointer		= 131,	//0x83
	cmdReadBytes		= 132,	//0x84
	cmdWriteBytes		= 133,	//0x85
	cmdRun				= 134,	//0x86
	cmdCricketCheck		= 135,	//0x87
	cmdCricketCheckACK	= 55	//0x37	
} eCricketCommands;
#endif
/**
 * @brief Run button states
 */
typedef enum  { STOPPED, RUNNING } eRunRequest;


/**
 * @brief Collection of states for the VM
 */
typedef struct _WetaStates
{
	eMachineState    machineState    : 3;
#ifdef SUPPORT_CRICKET
	eCommState       commState	     : 2;
#endif
	eRunRequest      runRequest      : 1;
	bool             waitingCmd      : 1;
	bool             configuring     : 1;
} WetaStates;



/**
 * @brief VM program context record
 */
typedef struct _Registers
{
	WetaCodePtr	    pc;			    	/**< program counter */
	uint8_t 	    opCode;		    	/**< Current instruction */
	uint8_t		    withCode;	    	/**< Sets current data type for stack operations */
	STACKSTATE      localFrame;			/**< Saved onto the stack by procedures */
	STACKSTATE      checkPoint;			/**< Used to cleanup args for a function call */
	WetaStackPtr    repcountLocation;	/**< Saved onto the stack by blocks */
	uint8_t         blockDepthMask;		/**< One bit set to indicate block depth */
	uint8_t         blocksExecuted;		/**< Bit set when block is run the first time */
#ifdef COOPERATIVE_RTOS
    WetaTimestamp   waitStart;          /**< Time that the last OP_WAIT occurred */
    uint16_t        waitMillis;         /**< Number of milliseconds to wait */
#endif
} Registers;

#ifdef SUPPORT_STRING
typedef struct
{
	char* pszByteFormat;
	char* pszIntFormat;
	char* pszFloatFormat;
} StringFormats;
#endif
/**
 * @brief Encapsulates a complete Weta VM
 */
typedef struct _Weta
{
	Hardware*     hal;
	WetaCodePtr	  startAddress;
	PSTACK		  stack;
	WetaStates	  states;
	Registers	  regs;
	PSTORE	      store;
	WetaTimestamp	  timerStart;   // Cricket logo timer
	SerialPort*   sport;		// i.e. default copied from hal
	SerialPort*   debugsport;
#ifdef SUPPORT_STRING
	StringFormats formats;
#endif
} Weta;

/**
 * @brief Send message to the debug output
 * @param pWeta
 * @param msg
 */
void weta_debug(Weta* pWeta, const char * msg);

/**
 * @brief Initialise a Weta VM
 * @param pWeta Pointer to an uninitialised Weta structure
 */
extern void weta_init(Weta* pWeta, Hardware* pHardware, uint16_t flags);

/**
 * @brief Reset a Weta object that might already be initialised
 * @param pWeta Pointer to a Weta structure
 */
extern void weta_reset(Weta* pWeta);

extern bool weta_start(Weta* pWeta, PSTORE store, WetaCodePtr address);

#ifdef SUPPORT_DEBOUNCE
extern void weta_debounce(Weta* pWeta);
#endif
/**
 * @brief Run a Weta VM
 * @param pWeta Initialised Weta structure identifying the VM
 */
extern void weta_loop(Weta* pWeta);

extern void weta_loop_body(Weta* pWeta);

// Utilities for internal use
extern WetaStackPtr getArgsLocation(Registers* pRegs);
extern void descendBlock(Registers* pRegs);
extern void ascendBlock(Registers* pRegs);
extern void setBlockExecuted(Registers* pRegs);
extern bool hasBlockExecuted(Registers* pRegs);
extern void pushRegisters(Weta* pWeta);
extern void popRegisters(Weta* pWeta);
extern bool with_current_type(Weta* pWeta);

#ifdef SUPPORT_JSON
/**
 * @brief Store the program codes in the given JSON array at the current start address
 * @param jsonArray
 * @return
 */
extern bool weta_program_json(Weta* pWeta, WetaStorage storage, WetaCodePtr address, const char* jsonArray);
#endif

#ifdef SUPPORT_QUERY
typedef enum
{
    QUERY_NONE     = 0x00,
    QUERY_DIGITAL  = 0x01,
    QUERY_ANALOG   = 0x02,
    QUERY_SERIAL   = 0x04,
    QUERY_MOTORS   = 0x08,
    QUERY_STEPPERS = 0x10,
    QUERY_SERVOS   = 0x20,
    QUERY_PWM      = 0x40,
    QUERY_ALL      = 0xFF
} WetaQuery;

extern bool weta_query_status(Weta* weta, WetaQuery q, char * json, uint16_t length);
extern bool weta_query_config(Weta* weta, char * json, uint16_t length);
#endif

#endif //__WETA_H__
