#ifndef __WETA_STACK_H__
#define __WETA_STACK_H__

#include "weta_platform.h"
#include <stddef.h>

struct _STACK;

typedef struct _STACK STACK, *PSTACK;

#define INVALID_STACKPTR ((WetaStackPtr)~0)

typedef struct _STACKSTATE
{
	WetaStackPtr top;
#ifdef SUPPORT_STRING	
	WetaStackPtr stringTop;
#endif	
} STACKSTATE, *PSTACKSTATE;

extern void		weta_stack_dump(STACK* pstack);
extern void 	weta_stack_init(void);

extern STACK*   weta_stack_allocate(void /*size_t size*/);
extern void	    weta_stack_release(STACK* pstack);

extern void     weta_stack_initStack(STACK* pstack);

extern uint8_t*	weta_stack_getStackAddress(STACK* pstack, WetaStackPtr loc);
extern uint8_t*	weta_stack_getTopAddress(STACK* pstack);

extern void     weta_stack_setStackState(STACK* pstack, const PSTACKSTATE pState);
extern void     weta_stack_getStackState(STACK* pstack, PSTACKSTATE pState);
extern void     weta_stack_pushStackState(STACK* pstack, const PSTACKSTATE pState);
extern void     weta_stack_popStackState(STACK* pstack, PSTACKSTATE pState);

#ifdef FORCE_STACK_BYTE_ALIGNED
extern WetaStackPtr weta_stack_pushBytes(STACK* pstack, uint8_t* bytes, uint8_t length);
extern WetaStackPtr weta_stack_popBytes(STACK* pstack, uint8_t* bytes, uint8_t length);
extern WetaStackPtr weta_stack_topBytes(STACK* pstack, uint8_t* bytes, uint8_t length);
extern WetaStackPtr weta_stack_getBytes(STACK* pstack, WetaStackPtr loc, uint8_t* bytes, uint8_t length);
extern WetaStackPtr weta_stack_setBytes(STACK* pstack, WetaStackPtr loc, uint8_t* bytes, uint8_t length);
#endif


extern WetaStackPtr weta_stack_getTop(STACK* pstack);
extern WetaStackPtr weta_stack_setTop(STACK* pstack, WetaStackPtr newTop);

extern WetaStackPtr weta_stack_getStringTop(STACK* pstack); // Temporary for debugging

extern WetaStackPtr weta_stack_pushn(STACK* pstack, WetaStackPtr amount);
extern WetaStackPtr weta_stack_popn(STACK* pstack, WetaStackPtr amount);

extern WetaStackPtr weta_stack_pushUint8(STACK* pstack, uint8_t val);
extern WetaStackPtr weta_stack_popUint8(STACK* pstack, uint8_t* pval);
extern WetaStackPtr weta_stack_topUint8(STACK* pstack, uint8_t* pval);
extern WetaStackPtr weta_stack_getUint8(STACK* pstack, WetaStackPtr loc, uint8_t* pval);
extern WetaStackPtr weta_stack_setUint8(STACK* pstack, WetaStackPtr loc, uint8_t val);

extern WetaStackPtr weta_stack_pushStackPtr(STACK* pstack, WetaStackPtr val);
extern WetaStackPtr weta_stack_popStackPtr(STACK* pstack, WetaStackPtr* pval);
extern WetaStackPtr weta_stack_topStackPtr(STACK* pstack, WetaStackPtr* pval);
extern WetaStackPtr weta_stack_getStackPtr(STACK* pstack, WetaStackPtr loc, WetaStackPtr* pval);
extern WetaStackPtr weta_stack_setStackPtr(STACK* pstack, WetaStackPtr loc, WetaStackPtr val); 

extern WetaStackPtr weta_stack_pushUint16(STACK* pstack, uint16_t val);
extern WetaStackPtr weta_stack_popUint16(STACK* pstack, uint16_t* pval);
extern WetaStackPtr weta_stack_topUint16(STACK* pstack, uint16_t* pval);
extern WetaStackPtr weta_stack_getUint16(STACK* pstack, WetaStackPtr loc, uint16_t* pval);
extern WetaStackPtr weta_stack_setUint16(STACK* pstack, WetaStackPtr loc, uint16_t val);

#ifdef SUPPORT_32BIT
extern WetaStackPtr weta_stack_pushUint32(STACK* pstack, uint32_t val);
extern WetaStackPtr weta_stack_popUint32(STACK* pstack, uint32_t* pval);
extern WetaStackPtr weta_stack_topUint32(STACK* pstack, uint32_t* pval);
extern WetaStackPtr weta_stack_getUint32(STACK* pstack, WetaStackPtr loc, uint32_t* pval);
extern WetaStackPtr weta_stack_setUint32(STACK* pstack, WetaStackPtr loc, uint32_t val);
#endif

#ifdef SUPPORT_FLOAT
extern WetaStackPtr weta_stack_pushFloat(STACK* pstack, float val);
extern WetaStackPtr weta_stack_popFloat(STACK* pstack, float* pval);
extern WetaStackPtr weta_stack_topFloat(STACK* pstack, float* pval);
extern WetaStackPtr weta_stack_getFloat(STACK* pstack, WetaStackPtr loc, float* pval);
extern WetaStackPtr weta_stack_setFloat(STACK* pstack, WetaStackPtr loc, float val);
#endif

#ifdef SUPPORT_DOUBLE
extern WetaStackPtr weta_stack_pushDouble(STACK* pstack, double val);
extern WetaStackPtr weta_stack_popDouble(STACK* pstack, double* pval);
extern WetaStackPtr weta_stack_topDouble(STACK* pstack, double* pval);
extern WetaStackPtr weta_stack_getDouble(STACK* pstack, WetaStackPtr loc, double* pval);
extern WetaStackPtr weta_stack_setDouble(STACK* pstack, WetaStackPtr loc, double val);
#endif

#ifdef SUPPORT_STRING
extern WetaStackPtr weta_stack_pushString(STACK* pstack, uint8_t* psz);
extern WetaStackPtr weta_stack_popString(STACK* pstack);	// Throw it away
extern WetaStackPtr weta_stack_topString(STACK* pstack, uint8_t** ppsz);
extern WetaStackPtr weta_stack_getString(STACK* pstack, WetaStackPtr loc, uint8_t** ppsz);
extern WetaStackPtr weta_stack_setString(STACK* pstack, WetaStackPtr loc, uint8_t* psz);
#endif
 
#endif // __WETA_STACK_H__
