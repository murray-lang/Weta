#include "type_handlers.h"

bool WETAFUNCATTR 
with_stackptr(Weta* pWeta)
{
	WetaStackPtr location, index;
	WetaStackPtr  rhs;

	switch (pWeta->regs.opCode)
	{
	case OP_SET:
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &rhs);
		weta_stack_setStackPtr(pWeta->stack, location, rhs);
		return true;

	case OP_GET:
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_getStackPtr(pWeta->stack, location, &rhs);
		weta_stack_pushStackPtr(pWeta->stack, rhs);
		return true;

	case OP_ASET:
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &index);
		weta_stack_popStackPtr(pWeta->stack, &rhs);
		weta_stack_setStackPtr(pWeta->stack, location + index * sizeof(WetaStackPtr), rhs);
		return true;

	case OP_AGET:
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &index);
		weta_stack_getStackPtr(pWeta->stack, location + index * sizeof(WetaStackPtr), &rhs);
		weta_stack_pushStackPtr(pWeta->stack, rhs);
		return true;

	case OP_OUTPUT:
		{
			//Serial.print("output ");
			weta_stack_popStackPtr(pWeta->stack, &rhs);	// The return value
				// point to where the arguments size is on the stack.
			location = getArgsLocation(&pWeta->regs);
			uint8_t argsSize;
			weta_stack_getUint8(pWeta->stack, location - sizeof(uint8_t), &argsSize); // Get size of args. 
				// If the size > 0 then step over the size location too. 
				// Otherwise keep pointing to the same place because the return
				// value should overwrite where the arguments size would be. 
			if (argsSize > 0)
				argsSize += sizeof(uint8_t);
			weta_stack_setStackPtr(pWeta->stack, location - (uint16_t)argsSize - sizeof(WetaStackPtr), rhs);
		}
		return true;
#ifdef SUPPORT_STRING
	case OP_TOSTR:
		{
			//Serial.println("---tostr---");
			weta_stack_popStackPtr(pWeta->stack, &rhs);
				// ***KLUDGE WARNING***
				// Borrowing unused (hopefully) stack space as a buffer!
				// just to avoid having to allocate another buffer.
			
			char* psz = (char *)weta_stack_getTopAddress(pWeta->stack);
			weta_sprintf(psz, pWeta->formats.pszByteFormat, rhs);
			//utoa((unsigned int) rhs, psz, 10);
				// Logically this is wrong because I'm apparently 
				// pushing a string to a location that it already 
				// temporarily occupies (the top of the stack and upwards). 
				// However, the stack implementation pushes strings 
				// onto a separate stack, then pushes that *location* 
				// onto the main stack. So the string doesn't get copied 
				// over itself, and is already copied before the first 
				// characters of the temporary original are overwritten 
				// by pushing the said location onto the main stack.
			weta_stack_pushString(pWeta->stack, (uint8_t*)psz);
		}
		return true;
#endif
	case OP_FOREACH:
		{	
			//Serial.print("foreach ");
			WetaCodePtr blockAddr;
			weta_stack_popCodePtr(pWeta->stack, &blockAddr);		// Block address
			weta_stack_popStackPtr(pWeta->stack, &location);			// Iterator variable location (Variable contains a 'pointer')
			uint8_t itemsRemaining;
			weta_stack_popUint8(pWeta->stack, &itemsRemaining);			// Number of items remaining
				// If we've gone through the block then we need to weta_stack_pop the
				// previous list item off the stack
			if (hasBlockExecuted(&pWeta->regs))
			{
				WetaStackPtr tmp;
				weta_stack_popStackPtr(pWeta->stack, &tmp);
			}

			if (itemsRemaining > 0)				// Any items remaining
			{
					// Set the value of the variable to the location on the
					// stack of the next list item (ie the weta_stack_top)
				weta_stack_setStackPtr(pWeta->stack, location, weta_stack_getTop(pWeta->stack) - sizeof(WetaStackPtr));
				pWeta->regs.pc = blockAddr;	// Point to the weta_stack_top of the block for the next iteration
				itemsRemaining--;					// Decrement the number of items remaining
				weta_stack_pushUint8(pWeta->stack, itemsRemaining);		// Save number of items remaining for next time
				weta_stack_pushStackPtr(pWeta->stack, location);		// Save iterator variable location for next time
				weta_stack_pushCodePtr(pWeta->stack, blockAddr);	// Save block address for next time
			}
			else
			{
				ascendBlock(&pWeta->regs);			// Leaving. Pop the block.
			}
		}
		return true;
	}
	return false;
}
