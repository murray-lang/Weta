#include "type_handlers.h"

uint8_t WETAFUNCATTR 
min_uint8(uint8_t lhs, uint8_t rhs)
{
	return lhs <= rhs ? lhs: rhs;
}

int8_t WETAFUNCATTR 
max_uint8(uint8_t lhs, uint8_t rhs)
{
	return lhs >= rhs ? lhs: rhs;
}

bool WETAFUNCATTR 
with_uint8(Weta* pWeta)
{
	WetaStackPtr location, index;
	uint8_t  rhs, lhs;
	//char szMsg[64];
	
	switch (pWeta->regs.opCode)
	{
	case OP_ASHIFT:
	case OP_LSHIFT:
		{
			int8_t shift;
			weta_stack_popUint8(pWeta->stack, (uint8_t*)&shift);
			weta_stack_popUint8(pWeta->stack, &lhs);
			if (shift >= 0)
				weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs << shift));
			else
				weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs >> -shift));
			return true;
		}
	case OP_SET:
		//Serial.println("---set---");

		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popUint8(pWeta->stack, &rhs);

		weta_stack_setUint8(pWeta->stack, location, rhs);
		return true;

	case OP_GET:
		//Serial.println("---get---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_getUint8(pWeta->stack, location, &rhs);
		weta_stack_pushUint8(pWeta->stack, rhs);
		return true;

	case OP_ASET:
		//Serial.println("---aset---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &index);
		weta_stack_popUint8(pWeta->stack, &rhs);
		weta_stack_setUint8(pWeta->stack, location + index * sizeof(uint8_t), rhs);
		return true;

	case OP_AGET:
		//Serial.println("---aget---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &index);
		weta_stack_getUint8(pWeta->stack, location + index * sizeof(uint8_t), &rhs);
		weta_stack_pushUint8(pWeta->stack, rhs);
		return true;

	case OP_OUTPUT:
		{
			//Serial.print("output ");
			weta_stack_popUint8(pWeta->stack, &rhs);	// The return value
				// point to where the arguments size is on the stack.
			location = getArgsLocation(&pWeta->regs);
			uint8_t argsSize;
			weta_stack_getUint8(pWeta->stack, location - sizeof(uint8_t), &argsSize); // Get size of args. 
				// If the size > 0 then step over the size location too. 
				// Otherwise keep pointing to the same place because the return
				// value should overwrite where the arguments size would be. 
			if (argsSize > 0)
				argsSize += sizeof(uint8_t);
			weta_stack_setUint8(pWeta->stack, location - (WetaStackPtr)argsSize - sizeof(uint8_t), rhs);
		}
		return true;

#ifdef SUPPORT_STRING
	case OP_TOSTR:
		{
			//Serial.println("---tostr---");
			weta_stack_popUint8(pWeta->stack, &rhs);
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
			//sprintf(szMsg, "OP_FOREACH location = %d, remaining = %d\r\n", location, itemsRemaining);
			//weta_debug(pWeta, szMsg);
				// If we've gone through the block then we need to pop the
				// previous list item off the stack
			if (hasBlockExecuted(&pWeta->regs))
			{
				uint8_t tmpUint8;
				weta_stack_popUint8(pWeta->stack, &tmpUint8);
			}

			if (itemsRemaining > 0)				// Any items remaining
			{
					// Set the value of the variable to the location on the
					// stack of the next list item (ie the top)
				weta_stack_setStackPtr(pWeta->stack, location, weta_stack_getTop(pWeta->stack) - sizeof(uint8_t));
				pWeta->regs.pc = blockAddr;	// Point to the top of the block for the next iteration
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

	weta_stack_popUint8(pWeta->stack, &rhs);

	switch (pWeta->regs.opCode)
	{
	case OP_BITNOT:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)~rhs);
		return true;

	case OP_ABS:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)abs(rhs));
		return true;

	case OP_NEG:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)-rhs);
		return true;
	}

	weta_stack_popUint8(pWeta->stack, &lhs);

	switch (pWeta->regs.opCode)
	{
	case OP_ADD:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs + rhs));
		return true;

	case OP_SUB:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs - rhs));
		return true;

	case OP_MUL:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs * rhs));
		return true;

	case OP_DIV:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs / rhs));
		return true;

	case OP_MOD:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs % rhs));
		return true;

	case OP_EQ:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs == rhs));
		return true;


	case OP_GT:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs > rhs));
		return true;

	case OP_LT:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs < rhs));
		return true;

	case OP_LE:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs <= rhs));
		return true;

	case OP_GE:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs >= rhs));
		return true;

	case OP_NE:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs != rhs));
		return true;

	case OP_BITAND:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs & rhs));
		return true;

	case OP_BITOR:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs | rhs));
		return true;

	case OP_BITXOR:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs ^ rhs));
		return true;
	
	case OP_MIN:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)min_uint8(lhs, rhs));
		return true;	
	case OP_MAX:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)max_uint8(lhs, rhs));
		return true;
	}
	return false;
}
