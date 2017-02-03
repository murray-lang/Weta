#ifndef __WETA_H__
#define __WETA_H__
/**
 * @file weta.h
 * @author Murray Lang
 * @brief 
 */
 
 #include <weta_platform.h>
 #include "./hw/hw.h"
 #include "weta_stack.h"
 #include "weta_store.h"

/**
 * @brief Machine states of the VM as a whole
 */
typedef enum { CONFIG, READY, COMM, RUN, STEPPER } eMachineState;

/**
 * @brief Communications states
 */
typedef enum  { COMM_IDLE, COMM_STARTED, COMM_FINISHED, COMM_TIMEOUT } eCommState;

/**
 * @brief Run button states
 */
typedef enum  { STOPPED, RUNNING } eRunRequest;

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

/**
 * @brief Collection of states for the VM
 */
typedef struct _WetaStates
{
	eMachineState    machineState    : 3;
	eCommState       commState	     : 2;
	eRunRequest      runRequest      : 1;
	bool             waitingCmd      : 1;
	unsigned int     unused          : 1;
} WetaStates;

/**
 * @brief Serial communications parameters
 */
 /*
typedef struct _SerialParams
{
	uint8_t	stopbits	: 2;
	eParity parity		: 2;
	uint8_t databits	: 4;
} SerialParams;
*/
/**
 * @brief VM program context record
 */
typedef struct _Registers
{
	WetaCodePtr	pc;			    	/**< program counter */
	uint8_t 	opCode;		    	/**< Current instruction */
	uint8_t		withCode;	    	/**< Sets current data type for stack operations */
	STACKSTATE  localFrame;			/**< Saved onto the stack by procedures */
	STACKSTATE  checkPoint;			/**< Used to cleanup args for a function call */ 
	WetaStackPtr repcountLocation;	/**< Saved onto the stack by blocks */
	uint8_t		blockDepthMask;		/**< One bit set to indicate block depth */
	uint8_t     blocksExecuted;		/**< Bit set when block is run the first time */
} Registers;

typedef struct
{
	char* pszByteFormat;
	char* pszIntFormat;
	char* pszFloatFormat;
} StringFormats;

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
	uint32_t	  timerStart;
	SerialPort*   sport;		// i.e. default copied from hal
	SerialPort*   debugsport;
	StringFormats formats;
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

extern void weta_start(Weta* pWeta);

extern void weta_debounce(Weta* pWeta);

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

#ifdef SUPPORT_JSON
/**
 * @brief Store the program codes in the given JSON array at the current start address
 * @param jsonArray
 * @return
 */
extern bool weta_program_json(Weta* pWeta, WetaCodePtr address, const char* jsonArray);
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

extern bool weta_query(Weta* weta, WetaQuery q, char * json, uint16_t length);

#endif

#endif //__WETA_H__