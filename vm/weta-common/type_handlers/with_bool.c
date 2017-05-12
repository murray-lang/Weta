#include "type_handlers.h"
 
bool WETAFUNCATTR 
with_bool(Weta* pWeta)
{
	WetaStackPtr location, index;
	uint8_t  lhs, rhs;

	switch (pWeta->regs.opCode)
	{
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
		//Serial.println("---ack, &location);
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
			if (rhs)
				weta_stack_pushString(pWeta->stack, (uint8_t*)"true");
			else
				weta_stack_pushString(pWeta->stack, (uint8_t*)"false");
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
				uint8_t tmpUint8;
				weta_stack_popUint8(pWeta->stack, &tmpUint8);
			}

			if (itemsRemaining > 0)				// Any items remaining
			{
					// Set the value of the variable to the location on the
					// stack of the next list item (ie the weta_stack_top)
				weta_stack_setStackPtr(pWeta->stack, location, weta_stack_getTop(pWeta->stack) - sizeof(uint8_t));
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
	
		// Note: AND OR XOR and NOT are inherently boolean and don't require
		// a withbool modifier.
	weta_stack_popUint8(pWeta->stack, &rhs);
	weta_stack_popUint8(pWeta->stack, &lhs);

	switch (pWeta->regs.opCode)
	{
	case OP_EQ:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs == rhs));
		return true;

	case OP_NE:
		weta_stack_pushUint8(pWeta->stack, (uint8_t)(lhs != rhs));
		return true;
	}
	return false;
}
