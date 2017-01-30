#include <weta_platform.h>
#ifdef SUPPORT_DOUBLE
#include "type_handlers.h"
#include "../hw/hw_endian.h"
#include <math.h>

bool WETAFUNCATTR 
with_double(Weta* pWeta)
{
	WetaStackPtr location, index;
	double  rhs, lhs;

	switch (pWeta->regs.opCode)
	{
	case OP_ENCODE:
		{
				// Convert an array of doubles to a list of bytes
			uint8_t size;
			weta_stack_popStackPtr(pWeta->stack, &location);
			weta_stack_popUint8(pWeta->stack, &size);
			//sprintf(szMsg, "OP_ENCODE Location=%d size=%d\r\n", location, size);
			//weta_debug(pWeta, szMsg);
			//weta_stack_dump(pWeta->stack);
			
				// Lists items are pushed from last to first, so get
				// the doubles from the end of the array and work backwards.
			int16_t i;
			for (i = size -1; i >=0; i--)
			{
				double value;
				weta_stack_getDouble(
					pWeta->stack, 
					location + i * sizeof(double), 
					&value
				);
				uint8_t asBytes[sizeof(double)];
					//The bytes need to presented in network byte order
				hton_double(value, asBytes);
					// Now the bytes need to go onto the stack in reverse order
				int16_t j;
				for (j = sizeof(double) - 1; j >=0; j--)
				{
					weta_stack_pushUint8(pWeta->stack, asBytes[j]);
				}
			}
				// Lastly, push the number of items in the list
			weta_stack_pushUint8(pWeta->stack, size * sizeof(double));
		}
		return true;
		
	case OP_DECODE:
		{
				// Convert an array of bytes into a list of doubles.
				// The bytes are in network byte order, so need to be converted
				// to host order for casting as doubles.
			uint8_t size;
			weta_stack_popStackPtr(pWeta->stack, &location);
			weta_stack_popUint8(pWeta->stack, &size);
			//sprintf(szMsg, "OP_DECODE Location=%d size=%d\r\n", location, size);
			//weta_debug(pWeta, szMsg);
				//Get a pointer to the array of bytes on the stack.
			uint8_t* bytes = (uint8_t*)weta_stack_getStackAddress(pWeta->stack, location);
				// Start from the end of the array, since list items are pushed
				// from last to first.
			int16_t i;
			for (i = size - sizeof(double); i >= 0; i -= sizeof(double))
			{
				double next = ntoh_double(&bytes[i]);
				weta_stack_pushDouble(pWeta->stack, next);
			}
				// Lastly, push the number of items in the list
			weta_stack_pushUint8(pWeta->stack, size / sizeof(double));
		}
		return true;
		
	case OP_SET:
		//Serial.println("---set---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popDouble(pWeta->stack, &rhs);
		weta_stack_setDouble(pWeta->stack, location, rhs);
		return true;

	case OP_GET:
		//Serial.println("---get---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_getDouble(pWeta->stack, location, &rhs);
		weta_stack_pushDouble(pWeta->stack, rhs);
		return true;

	case OP_ASET:
		//Serial.println("---aset---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &index);
		weta_stack_popDouble(pWeta->stack, &rhs);
		weta_stack_setDouble(pWeta->stack, location + index * sizeof(double), rhs);
		return true;

	case OP_AGET:	
		//Serial.println("---aget---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &index);
		weta_stack_getDouble(pWeta->stack, location + index * sizeof(double), &rhs);
		weta_stack_pushDouble(pWeta->stack, rhs);
		return true;

	case OP_OUTPUT:
		{
			//Serial.print("output ");
			weta_stack_popDouble(pWeta->stack, &rhs);	// The return value
				// point where the arguments size is on the stack.
			location = getArgsLocation(&pWeta->regs);
			uint8_t argsSize;
			weta_stack_getUint8(pWeta->stack, location - sizeof(uint8_t), &argsSize); // Get size of args. 
				// If the size > 0 then step over the size location too. 
				// Otherwise keep pointing to the same place because the return
				// value should overwrite where the arguments size would be.
			if (argsSize > 0)
				argsSize += sizeof(uint8_t);
			weta_stack_setDouble(pWeta->stack, location - (WetaStackPtr)argsSize - sizeof(double), rhs);
		}
		return true;

	case OP_TX:
		{
			//Serial.println("---tx---");
			weta_stack_popDouble(pWeta->stack, &rhs);
			uint8_t nbuf[sizeof(double)];
			hton_double(rhs, nbuf);
			hw_serial_write(pWeta->sport, nbuf, sizeof(double));
		}
		return true;

	case OP_RX:
		{
			//Serial.println("---rx---");
			uint8_t nbuf[sizeof(double)];
			hw_serial_read(pWeta->sport, nbuf, sizeof(double), -1);
			weta_stack_pushDouble(pWeta->stack, ntoh_double(nbuf));
		}
		return true;
#ifdef SUPPORT_STRING
	case OP_TOSTR:
		{
			//Serial.println("---tostr---");
			weta_stack_popDouble(pWeta->stack, &rhs);
				// ***KLUDGE WARNING***
				// Borrowing unused (hopefully) stack space as a buffer!
				// just to avoid having to allocate another buffer.
			
			char* psz = (char *)weta_stack_getTopAddress(pWeta->stack);
			weta_sprintf(psz, pWeta->formats.pszFloatFormat, rhs);
			//dtostrf((double)rhs, 5, 2, psz);
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
				double tmp;
				weta_stack_popDouble(pWeta->stack, &tmp);
			}

			if (itemsRemaining > 0)				// Any items remaining
			{
					// Set the value of the variable to the location on the
					// stack of the next list item (ie the top)
				weta_stack_setStackPtr(pWeta->stack, location, weta_stack_getTop(pWeta->stack) - sizeof(double));
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

	weta_stack_popDouble(pWeta->stack, &rhs);	// rhs

	switch (pWeta->regs.opCode)
	{
	case OP_ABS:
		weta_stack_pushDouble(pWeta->stack, (double)fabs((double)rhs));
		return true;
	case OP_NEG:
		weta_stack_pushDouble(pWeta->stack, -rhs);
		return true;
	}

	weta_stack_popDouble(pWeta->stack, &lhs);	// lhs

	switch (pWeta->regs.opCode)
	{
	case OP_ADD:
		weta_stack_pushDouble(pWeta->stack, (double)(lhs + rhs));
		return true;

	case OP_SUB:
		weta_stack_pushDouble(pWeta->stack, (double)(lhs - rhs));
		return true;

	case OP_MUL:
		weta_stack_pushDouble(pWeta->stack, (double)(lhs * rhs));
		return true;

	case OP_DIV:
		//Serial.print("div (");
		//Serial.print(lhs);
		//Serial.print("/");
		//Serial.print(rhs);
		//Serial.print(") ");
		weta_stack_pushDouble(pWeta->stack, (double)(lhs / rhs));
		return true;

	case OP_MOD:
		weta_stack_pushDouble(pWeta->stack, (double)fmod((double)lhs, (double)rhs));
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
	
	case OP_MIN:
		weta_stack_pushDouble(pWeta->stack, (double)fmin((double)lhs, (double)rhs));
		return true;	
	case OP_MAX:
		weta_stack_pushDouble(pWeta->stack, (double)fmax((double)lhs, (double)rhs));
		return true;
	}
	return false;
}
#endif

