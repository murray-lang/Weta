#include "weta_stack.h"
#include "weta_defs.h"
#include <stdbool.h> // for 'bool'

//#include <ch.h>				// For debugging
//#include <hal.h>			// For debugging
//#include "./hw/hw_serial.h" // For debugging
//#include <stdio.h>          // For debugging (sprintf())
//#include <string.h>			// For debugging


// Private stack structure
struct _STACK
{
	uint8_t  stack[WETA_STACK_SIZE];		// The main stack grows upwards
	bool     available;
	WetaStackPtr top;
#ifdef SUPPORT_STRING
	WetaStackPtr stringTop;
#endif
};

static STACK stacks[WETA_MAX_STACKS];
/*
void weta_stack_dump(STACK* pstack)
{
	char szDump[256];
	sprintf(szDump, "Stack dump (Top = %d):\r\n", pstack->top);
	hw_serial_write(Serial2, (uint8_t*)szDump, strlen(szDump));
	
	WetaStackPtr i;
	for (i = 0; i < pstack->top; i++)
	{
		sprintf(szDump, "%02x\r\n", pstack->stack[i]);
		hw_serial_write(Serial2, (uint8_t*)szDump, strlen(szDump));
	}
	sprintf(szDump, "End of stack dump\r\n");
	hw_serial_write(Serial2, (uint8_t*)szDump, strlen(szDump));
}
*/
void WETAFUNCATTR 
weta_stack_initStack(STACK* pstack)
{
	pstack->available = true;
	pstack->top = 0;
#ifdef SUPPORT_STRING
	pstack->stringTop = WETA_STACK_SIZE;
#endif
	WetaStackPtr i;
	for (i = 0; i < WETA_STACK_SIZE; i++)
		pstack->stack[i] = 0;	
}

void WETAFUNCATTR 
weta_stack_init()
{
	int i;
	for (i = 0; i < WETA_MAX_STACKS; i ++)
		weta_stack_initStack((STACK*)&stacks[i]);
}

STACK* WETAFUNCATTR 
weta_stack_allocate(/*size_t size*/)
{
	int i;
	for (i = 0; i < WETA_MAX_STACKS; i ++)
	{
		if (stacks[i].available)
		{
			stacks[i].available = false;
			return &stacks[i];
		}
	}
	return 0;
}

void WETAFUNCATTR	  
weta_stack_release(STACK* pstack)
{
	WetaStackPtr i;
	for (i = 0; i < WETA_MAX_STACKS; i ++)
	{
		if (pstack == &stacks[i])
		{
			weta_stack_initStack(pstack);
			break;
		}
	}
}

void WETAFUNCATTR    
weta_stack_setStackState(STACK* pstack, const PSTACKSTATE pState)
{
	pstack->top       = pState->top;
#ifdef SUPPORT_STRING
	pstack->stringTop = pState->stringTop;
#endif	
}

void WETAFUNCATTR    
weta_stack_getStackState(STACK* pstack, PSTACKSTATE pState)
{
	pState->top       = pstack->top;
#ifdef SUPPORT_STRING
	pState->stringTop = pstack->stringTop;
#endif
}

void WETAFUNCATTR     
weta_stack_pushStackState(STACK* pstack, const PSTACKSTATE pState)
{
	weta_stack_pushStackPtr(pstack, pState->top);
#ifdef SUPPORT_STRING
	weta_stack_pushStackPtr(pstack, pState->stringTop);
#endif
}

void WETAFUNCATTR     
weta_stack_popStackState(STACK* pstack, PSTACKSTATE pState)
{
#ifdef SUPPORT_STRING	
	weta_stack_popStackPtr(pstack, &pState->stringTop);
#endif
	weta_stack_popStackPtr(pstack, &pState->top);
}

WetaStackPtr WETAFUNCATTR 
weta_stack_pushn(STACK* pstack, WetaStackPtr amount)
{
#ifdef STACK_CHECKED
	if (pstack->top > (WETA_STACK_SIZE - amount))
		return INVALID_STACKPTR;
#endif
	pstack->top += amount;
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_popn(STACK* pstack, WetaStackPtr amount)
{
#ifdef STACK_CHECKED
	if (pstack->top < amount)
		return ~0;
#endif
	pstack->top -= amount;
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_getTop(STACK* pstack)
{
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR
weta_stack_setTop(STACK* pstack, WetaStackPtr newTop)
{
#ifdef STACK_CHECKED
	if (newTop > WETA_STACK_SIZE || newTop < 0)
		return INVALID_STACKPTR;
#endif
	pstack->top = newTop;
	return newTop;
}

// Temporary for debugging
#ifdef SUPPORT_STRING
WetaStackPtr WETAFUNCATTR 
weta_stack_getStringTop(STACK* pstack)
{
	return pstack->stringTop;	
}
#endif

uint8_t* WETAFUNCATTR    
weta_stack_getStackAddress(STACK* pstack, WetaStackPtr loc)
{
	return &pstack->stack[loc];
}

uint8_t* WETAFUNCATTR    
weta_stack_getTopAddress(STACK* pstack)
{
	return &pstack->stack[pstack->top];
}

#ifdef FORCE_STACK_BYTE_ALIGNED

WetaStackPtr WETAFUNCATTR
weta_stack_pushBytes(STACK* pstack, uint8_t* bytes, uint8_t length)
{
#ifdef STACK_CHECKED
	if (pstack->top > WETA_STACK_SIZE - length)
		return INVALID_STACKPTR;
#endif
	WetaStackPtr whereItWent = pstack->top;
	//char szMsg[32];
	uint8_t i;
	for (i = 0; i < length; i++)
	{
		pstack->stack[whereItWent + i] = bytes[i];
		//sprintf(szMsg, "stack[%d] = %02x\r\n", whereItWent + i, bytes[i]);
		//hw_serial_write(Serial2, (uint8_t*)szMsg, strlen(szMsg));
	}
		
	pstack->top += length;
	return whereItWent;	
}

WetaStackPtr WETAFUNCATTR 
weta_stack_popBytes(STACK* pstack, uint8_t* bytes, uint8_t length)
{
#ifdef STACK_CHECKED
	if (pstack->top < length)
		return INVALID_STACKPTR;
#endif	
	pstack->top -= length;
	uint8_t i;
	for (i = 0; i < length; i++)
		bytes[i] = pstack->stack[pstack->top + i];
		
	return pstack->top;	
}

WetaStackPtr WETAFUNCATTR 
weta_stack_topBytes(STACK* pstack, uint8_t* bytes, uint8_t length)
{
	#ifdef STACK_CHECKED
	if (pstack->top < length)
		return INVALID_STACKPTR;
#endif
	WetaStackPtr from = pstack->top - length;
	uint8_t i;
	for (i = 0; i < length; i++)
		bytes[i] = pstack->stack[from + i];
		
	return from;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_getBytes(STACK* pstack, WetaStackPtr loc, uint8_t* bytes, uint8_t length)
{
#ifdef STACK_CHECKED
	if (loc + length > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
	uint8_t i;
	for (i = 0; i < length; i++)
		bytes[i] = pstack->stack[loc + i];
		
	return loc;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_setBytes(STACK* pstack, WetaStackPtr loc, uint8_t* bytes, uint8_t length)
{
	#ifdef STACK_CHECKED
	if (loc + length > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
	uint8_t i;
	for (i = 0; i < length; i++)
		pstack->stack[loc + i] = bytes[i];
	return loc;
}
#endif // FORCE_STACK_BYTE_ALIGNED


WetaStackPtr WETAFUNCATTR 
weta_stack_pushUint8(STACK* pstack, uint8_t val)
{
#ifdef STACK_CHECKED
	if (pstack->top > WETA_STACK_SIZE - 1)
		return INVALID_STACKPTR;
#endif
	WetaStackPtr whereItWent = pstack->top;	
	pstack->stack[whereItWent] = val;
	pstack->top++;
	
	return whereItWent;	
}

WetaStackPtr WETAFUNCATTR 
weta_stack_popUint8(STACK* pstack, uint8_t* pval)
{
#ifdef STACK_CHECKED
	if (pstack->top < 1)
		return INVALID_STACKPTR;
#endif	
	pstack->top--;
	*pval = pstack->stack[pstack->top];
	
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_topUint8(STACK* pstack, uint8_t* pval)
{
	WetaStackPtr pos = pstack->top - 1;
#ifdef STACK_CHECKED
	if (pos < 0)
		return INVALID_STACKPTR;	
#endif
	*pval = pstack->stack[pos];
	return pos;	
}

WetaStackPtr WETAFUNCATTR
weta_stack_getUint8(STACK* pstack, WetaStackPtr loc, uint8_t* pval)
{
#ifdef STACK_CHECKED
	if (loc + 1 > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
	*pval = pstack->stack[loc];
	return loc;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_setUint8(STACK* pstack, WetaStackPtr loc, uint8_t val)
{
#ifdef STACK_CHECKED
	if (loc + 1 > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif	
	pstack->stack[loc] = val;
	return loc;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_pushStackPtr(STACK* pstack, WetaStackPtr val)
{
#ifdef STACK_CHECKED
	if (pstack->top > WETA_STACK_SIZE - sizeof(WetaStackPtr))
		return INVALID_STACKPTR;
#endif
	WetaStackPtr whereItWent;
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (WetaStackPtr)];
		WetaStackPtr value;
	} asBytes;
	
	asBytes.value = val;
	whereItWent = weta_stack_pushBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
#else
	whereItWent = pstack->top;
	*((WetaStackPtr*)&pstack->stack[whereItWent]) = val;
	pstack->top += sizeof(WetaStackPtr);
#endif
	return whereItWent;
		
}

WetaStackPtr WETAFUNCATTR 
weta_stack_popStackPtr(STACK* pstack, WetaStackPtr* pval)
{
#ifdef STACK_CHECKED
	if (pstack->top < sizeof(WetaStackPtr))
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (WetaStackPtr)];
		WetaStackPtr value;
	} asBytes;
	weta_stack_popBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else	
	pstack->top -= sizeof(WetaStackPtr);
	*pval =*((WetaStackPtr*)&pstack->stack[pstack->top]);
#endif	
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_topStackPtr(STACK* pstack, WetaStackPtr* pval)
{
	WetaStackPtr pos = pstack->top - sizeof(WetaStackPtr);
#ifdef STACK_CHECKED
	if (pos < 0)
		return INVALID_STACKPTR;	
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (WetaStackPtr)];
		WetaStackPtr value;
	} asBytes;
	
	weta_stack_topBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((WetaStackPtr*)&pstack->stack[pos]);
#endif
	return pos;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_getStackPtr(STACK* pstack, WetaStackPtr loc, WetaStackPtr* pval)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(WetaStackPtr) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (WetaStackPtr)];
		WetaStackPtr value;
	} asBytes;
	
	weta_stack_getBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((WetaStackPtr*)&pstack->stack[loc]);
#endif	
	return loc;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_setStackPtr(STACK* pstack, WetaStackPtr loc, WetaStackPtr val)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(WetaStackPtr) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (WetaStackPtr)];
		WetaStackPtr value;
	} asBytes;
	
	asBytes.value = val;
	weta_stack_setBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
#else
	*((WetaStackPtr*)&pstack->stack[loc]) = val;
#endif	
	return loc;
}


WetaStackPtr WETAFUNCATTR 
weta_stack_pushUint16(STACK* pstack, uint16_t val)
{
#ifdef STACK_CHECKED
	if (pstack->top > WETA_STACK_SIZE - sizeof(uint16_t))
		return INVALID_STACKPTR;
#endif
	WetaStackPtr whereItWent;
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint16_t)];
		uint16_t value;
	} asBytes;
	
	asBytes.value = val;
	whereItWent = weta_stack_pushBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
#else
	whereItWent = pstack->top;	
	*((uint16_t*)&pstack->stack[whereItWent]) = val;
	pstack->top += sizeof(uint16_t);
#endif	
	return whereItWent;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_popUint16(STACK* pstack, uint16_t* pval)
{
#ifdef STACK_CHECKED
	if (pstack->top < sizeof(uint16_t))
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint16_t)];
		uint16_t value;
	} asBytes;
	weta_stack_popBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	pstack->top -= sizeof(uint16_t);
	*pval =*((uint16_t*)&pstack->stack[pstack->top]);
#endif	
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_topUint16(STACK* pstack, uint16_t* pval)
{
	WetaStackPtr pos = pstack->top - sizeof(uint16_t);
#ifdef STACK_CHECKED
	if (pos < 0)
		return INVALID_STACKPTR;	
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint16_t)];
		uint16_t value;
	} asBytes;
	
	weta_stack_topBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((uint16_t*)&pstack->stack[pos]);
#endif	
	return pos;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_getUint16(STACK* pstack, WetaStackPtr loc, uint16_t* pval)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(uint16_t) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint16_t)];
		uint16_t value;
	} asBytes;
	
	weta_stack_getBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((uint16_t*)&pstack->stack[loc]);
#endif	
	return loc;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_setUint16(STACK* pstack, WetaStackPtr loc, uint16_t val)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(uint16_t) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint16_t)];
		uint16_t value;
	} asBytes;
	
	asBytes.value = val;
	weta_stack_setBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
#else
	*((uint16_t*)&pstack->stack[loc]) = val;
#endif	
	return loc;
}

#ifdef SUPPORT_32BIT
WetaStackPtr WETAFUNCATTR 
weta_stack_pushUint32(STACK* pstack, uint32_t val)
{
#ifdef STACK_CHECKED
	if (pstack->top > WETA_STACK_SIZE - sizeof(uint32_t))
		return INVALID_STACKPTR;
#endif
	WetaStackPtr whereItWent;
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint32_t)];
		uint32_t value;
	} asBytes;
	
	asBytes.value = val;
	whereItWent = weta_stack_pushBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
#else
	whereItWent = pstack->top;	
	*((uint32_t*)&pstack->stack[whereItWent]) = val;
	pstack->top += sizeof(uint32_t);
#endif	
	return whereItWent;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_popUint32(STACK* pstack, uint32_t* pval)
{
#ifdef STACK_CHECKED
	if (pstack->top < sizeof(uint32_t))
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint32_t)];
		uint32_t value;
	} asBytes;
	weta_stack_popBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else	
	pstack->top -= sizeof(uint32_t);
	*pval =*((uint32_t*)&pstack->stack[pstack->top]);
#endif	
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_topUint32(STACK* pstack, uint32_t* pval)
{
	WetaStackPtr pos = pstack->top - sizeof(uint32_t);
#ifdef STACK_CHECKED
	if (pos < 0)
		return INVALID_STACKPTR;	
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint32_t)];
		uint32_t value;
	} asBytes;
	
	weta_stack_topBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((uint32_t*)&pstack->stack[pos]);
#endif	
	return pos;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_getUint32(STACK* pstack, WetaStackPtr loc, uint32_t* pval)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(uint32_t) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint32_t)];
		uint32_t value;
	} asBytes;
	
	weta_stack_getBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((uint32_t*)&pstack->stack[loc]);
#endif	
	return loc;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_setUint32(STACK* pstack, WetaStackPtr loc, uint32_t val)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(uint32_t) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (uint32_t)];
		uint32_t value;
	} asBytes;
	
	asBytes.value = val;
	weta_stack_setBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
#else
	*((uint32_t*)&pstack->stack[loc]) = val;
#endif
	return loc;
}

#endif

#ifdef SUPPORT_FLOAT
WetaStackPtr WETAFUNCATTR 
weta_stack_pushFloat(STACK* pstack, float val)
{
#ifdef STACK_CHECKED
	if (pstack->top > WETA_STACK_SIZE - sizeof(float))
		return INVALID_STACKPTR;
#endif
	WetaStackPtr whereItWent;
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (float)];
		float   value;
	} asBytes;
	
	asBytes.value = val;
	whereItWent = weta_stack_pushBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
#else
	whereItWent = pstack->top;	
	*((float*)&pstack->stack[whereItWent]) = val;
	pstack->top += sizeof(uint32_t);
#endif	
	return whereItWent;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_popFloat(STACK* pstack, float* pval)
{
#ifdef STACK_CHECKED
	if (pstack->top < sizeof(float))
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (float)];
		float value;
	} asBytes;
	weta_stack_popBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else	
	pstack->top -= sizeof(float);
	*pval =*((float*)&pstack->stack[pstack->top]);
#endif	
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_topFloat(STACK* pstack, float* pval)
{
	WetaStackPtr pos = pstack->top - sizeof(float);
#ifdef STACK_CHECKED
	if (pos < 0)
		return INVALID_STACKPTR;	
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (float)];
		float value;
	} asBytes;
	
	weta_stack_topBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((float*)&pstack->stack[pos]);
#endif	
	return pos;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_getFloat(STACK* pstack, WetaStackPtr loc, float* pval)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(float) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (float)];
		float value;
	} asBytes;
	
	weta_stack_getBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((float*)&pstack->stack[loc]);
#endif	
	return loc;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_setFloat(STACK* pstack, WetaStackPtr loc, float val)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(float) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif	
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (float)];
		float value;
	} asBytes;
	
	asBytes.value = val;
	weta_stack_setBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
#else
	*((float*)&pstack->stack[loc]) = val;
#endif	
	return loc;
}

#endif

#ifdef SUPPORT_DOUBLE
WetaStackPtr WETAFUNCATTR 
weta_stack_pushDouble(STACK* pstack, double val)
{
#ifdef STACK_CHECKED
	if (pstack->top > WETA_STACK_SIZE - sizeof(double))
		return INVALID_STACKPTR;
#endif
	WetaStackPtr whereItWent;
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (double)];
		double   value;
	} asBytes;
	
	asBytes.value = val;
	whereItWent = weta_stack_pushBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
#else
	whereItWent = pstack->top;	
	*((double*)&pstack->stack[whereItWent]) = val;
	pstack->top += sizeof(uint32_t);
#endif	
	return whereItWent;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_popDouble(STACK* pstack, double* pval)
{
#ifdef STACK_CHECKED
	if (pstack->top < sizeof(double))
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (double)];
		double value;
	} asBytes;
	weta_stack_popBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else	
	pstack->top -= sizeof(double);
	*pval =*((double*)&pstack->stack[pstack->top]);
#endif	
	return pstack->top;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_topDouble(STACK* pstack, double* pval)
{
	WetaStackPtr pos = pstack->top - sizeof(double);
#ifdef STACK_CHECKED
	if (pos < 0)
		return INVALID_STACKPTR;	
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (double)];
		double value;
	} asBytes;
	
	weta_stack_topBytes(pstack, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((double*)&pstack->stack[pos]);
#endif	
	return pos;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_getDouble(STACK* pstack, WetaStackPtr loc, double* pval)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(double) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (double)];
		double value;
	} asBytes;
	
	weta_stack_getBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
	*pval = asBytes.value;
#else
	*pval = *((double*)&pstack->stack[loc]);
#endif	
	return loc;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_setDouble(STACK* pstack, WetaStackPtr loc, double val)
{
#ifdef STACK_CHECKED
	if (loc + sizeof(float) > pstack->top || loc < 0)
		return INVALID_STACKPTR;
#endif
#ifdef FORCE_STACK_BYTE_ALIGNED
	union AsBytes
	{
		uint8_t bytes[sizeof (double)];
		double value;
	} asBytes;
	
	asBytes.value = val;
	weta_stack_setBytes(pstack, loc, asBytes.bytes, sizeof (asBytes.bytes));
#else	
	*((float*)&pstack->stack[loc]) = val;
#endif	
	return loc;
}

#endif

#ifdef SUPPORT_STRING

WetaStackPtr WETAFUNCATTR 
_weta_stack_pushChar(PSTACK pstack, uint8_t ch)
{
#ifdef STACK_CHECKED
	if (pstack->stringTop < 1)
		return INVALID_STACKPTR;
#endif	
	pstack->stringTop--;
	pstack->stack[pstack->stringTop] = ch;
	
	return pstack->stringTop;
}

WetaStackPtr WETAFUNCATTR 
_weta_stack_popChar(PSTACK pstack, uint8_t* ch)
{
#ifdef STACK_CHECKED
	if (pstack->stringTop > WETA_STACK_SIZE - 1)
		return INVALID_STACKPTR;
#endif	
	*ch = pstack->stack[pstack->stringTop];
	pstack->stringTop++;
	return pstack->stringTop;

}

WetaStackPtr WETAFUNCATTR 
_setChar(PSTACK pstack, WetaStackPtr loc, uint8_t ch)
{
#ifdef STACK_CHECKED
	if (loc > WETA_STACK_SIZE - 1)
		return INVALID_STACKPTR;
#endif
	pstack->stack[loc] = ch;	
	return loc;
}

WetaStackPtr WETAFUNCATTR 
_weta_stack_pushString(PSTACK pstack, uint8_t* psz)
{
		// Avoiding string.h for now
	uint16_t  length  = 0;	
	uint8_t * pszTemp = psz;
	while (*pszTemp++)
		length++;
#ifdef STACK_CHECKED
	if (pstack->stringTop < length)
		return INVALID_STACKPTR;
#endif
		// Push the terminating 0 regardless of length
	while (--pszTemp >= psz)
		_weta_stack_pushChar(pstack, *pszTemp);

	return pstack->stringTop; // (changed by _weta_stack_pushChar() )
}

WetaStackPtr WETAFUNCATTR 
_weta_stack_popString(PSTACK pstack)
{
	uint8_t next;
	WetaStackPtr pos;
	do
	{
		pos = _weta_stack_popChar(pstack, &next);
	}
	while (next != 0 && pos != INVALID_STACKPTR);
	return pstack->stringTop;
}

WetaStackPtr WETAFUNCATTR 
_weta_stack_topString(PSTACK pstack, uint8_t** ppsz)
{
	*ppsz = (uint8_t*)weta_stack_getStackAddress(pstack, pstack->stringTop);
	return pstack->stringTop;
}

WetaStackPtr WETAFUNCATTR 
_weta_stack_getString(PSTACK pstack, WetaStackPtr loc, uint8_t** ppsz)
{
	*ppsz = (uint8_t*)weta_stack_getStackAddress(pstack, loc);
	return loc;
}

WetaStackPtr WETAFUNCATTR 
_weta_stack_setString(PSTACK pstack, WetaStackPtr loc, uint8_t* psz)
{
	WetaStackPtr next = loc;
	do
	{
		_setChar(pstack, next++, *psz);
	}
	while (*psz++);

	return loc;
}


WetaStackPtr WETAFUNCATTR 
weta_stack_pushString(STACK* pstack, uint8_t* psz)
{
		// Push the string onto the downward growing stack first
	WetaStackPtr strLoc = _weta_stack_pushString((PSTACK)pstack, psz);
#ifdef STACK_CHECKED
	if (strLoc == INVALID_STACKPTR)
		return INVALID_STACKPTR;
#endif
		// Now push the string's offset onto the main stack
	return weta_stack_pushStackPtr(pstack, strLoc);

}

WetaStackPtr WETAFUNCATTR 
weta_stack_popString(STACK* pstack)
{
		// Simply pop from both stacks
	_weta_stack_popString(pstack);
	WetaStackPtr tmp;
	return weta_stack_popStackPtr(pstack, &tmp);
}

WetaStackPtr WETAFUNCATTR 
weta_stack_topString(STACK* pstack, uint8_t** ppsz)
{
		// ***NOTE***
		// In order to support the current scheme for returning strings from
		// functions, we have to use the pointer on the main stack to point
		// to the string rather than assume that it's at the top of the string
		// stack. (The string could actually be in zombie land beyond the top
		// of the string stack.)
		// Get the pointer from the mainstack
	WetaStackPtr strPtr;
	weta_stack_topStackPtr(pstack, &strPtr);
		// Get the string from the string stack
	_weta_stack_getString(pstack, strPtr, ppsz);
	return strPtr;
}

WetaStackPtr WETAFUNCATTR 
weta_stack_getString(STACK* pstack, WetaStackPtr loc, uint8_t** ppsz)
{
		// Get the location of the string in the string stack from the given
		// location in the main stack
	WetaStackPtr strLoc;
#ifdef STACK_CHECKED
	WetaStackPtr tmpLoc = weta_stack_getStackPtr(pstack, loc, &strLoc);
	if (tmpLoc == INVALID_STACKPTR)
		return INVALID_STACKPTR;
	tmpLoc = _weta_stack_getString((PSTACK)pstack, strLoc, ppsz);
	if (tmpLoc == INVALID_STACKPTR)
		return INVALID_STACKPTR;
#else
	weta_stack_getStackPtr(pstack, loc, &strLoc);
	_weta_stack_getString((PSTACK)pstack, strLoc, ppsz);
#endif
	return loc;
}

WetaStackPtr WETAFUNCATTR  
weta_stack_setString(STACK* pstack, WetaStackPtr loc, uint8_t* psz)
{
		// Get the location of the string in the string stack from the given
		// location in the main stack
	WetaStackPtr strLoc;
#ifdef STACK_CHECKED
	WetaStackPtr tmpLoc = weta_stack_getStackPtr(pstack, loc, &strLoc);
	if (tmpLoc == INVALID_STACKPTR)
		return INVALID_STACKPTR;
	tmpLoc = _weta_stack_setString((PSTACK)pstack, strLoc, psz);
	if (tmpLoc == INVALID_STACKPTR)
		return INVALID_STACKPTR;
#else
	weta_stack_getStackPtr(pstack, loc, &strLoc);
	_weta_stack_setString((PSTACK)pstack, strLoc, psz);
#endif
	return loc;
}

#endif

