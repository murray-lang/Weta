#include "type_handlers.h"
#include "../hw/hw_endian.h"

#ifdef SUPPORT_32BIT

int32_t WETAFUNCATTR 
min_int32(int32_t lhs, int32_t rhs)
{
	return lhs <= rhs ? lhs: rhs;
}

int32_t WETAFUNCATTR 
max_int32(int32_t lhs, int32_t rhs)
{
	return lhs >= rhs ? lhs: rhs;
}

bool WETAFUNCATTR 
with_int32(Weta* pWeta)
{
	WetaStackPtr location, index;
	int32_t  rhs, lhs;

	switch (pWeta->regs.opCode)
	{
	case OP_ENCODE:
		{
				// Convert an array of int32_t to a list of bytes
			uint8_t size;
			weta_stack_popStackPtr(pWeta->stack, &location);
			weta_stack_popUint8(pWeta->stack, &size);
			//sprintf(szMsg, "OP_ENCODE Location=%d size=%d\r\n", location, size);
			//weta_debug(pWeta, szMsg);
			//weta_stack_dump(pWeta->stack);
			
				// Lists items are pushed from last to first, so get
				// the int32_t from the end of the array and work backwards.
			int16_t i;
			for (i = size -1; i >=0; i--)
			{
				int32_t value;
				weta_stack_getUint32(
					pWeta->stack, 
					location + i * sizeof(int32_t), 
					(uint32_t*)&value
				);
				uint8_t asBytes[sizeof(int32_t)];
					//The bytes need to presented in network byte order
				hton_int32(value, asBytes);
					// Now the bytes need to go onto the stack in reverse order
				int16_t j;
				for (j = sizeof(int32_t) - 1; j >=0; j--)
				{
					weta_stack_pushUint8(pWeta->stack, asBytes[j]);
				}
			}
				// Lastly, push the number of items in the list
			weta_stack_pushUint8(pWeta->stack, size * sizeof(int32_t));
			  
		}
		return true;
		
	case OP_DECODE:
		{
				// Convert an array of bytes into a list of int32_t.
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
			for (i = size - sizeof(int32_t); i >= 0; i -= sizeof(int32_t))
			{
				int32_t next = ntoh_int32(&bytes[i]);
				weta_stack_pushUint32(pWeta->stack, (uint32_t)next);
			}
				// Lastly, push the number of items in the list
			weta_stack_pushUint8(pWeta->stack, size / sizeof(int32_t));
		}
		return true;
		
	case OP_ASHIFT:
	case OP_LSHIFT:
		{
			int8_t shift;
			weta_stack_popUint8(pWeta->stack, (uint8_t*)&shift);
			weta_stack_popUint32(pWeta->stack, (uint32_t*)&lhs);
			if (shift >= 0)
				weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs << shift));
			else
				weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs >> -shift));
			return true;
		}
	case OP_SET:
		//Serial.println("---set---");

		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popUint32(pWeta->stack, (uint32_t*)&rhs);

		weta_stack_setUint32(pWeta->stack, location, (uint32_t)rhs);
		return true;

	case OP_GET:
		//Serial.println("---get---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_getUint32(pWeta->stack, location, (uint32_t*)&rhs);
		weta_stack_pushUint32(pWeta->stack, (uint32_t)rhs);
		return true;

	case OP_ASET:
		//Serial.println("---aset---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &index);
		weta_stack_popUint32(pWeta->stack, (uint32_t*)&rhs);
		weta_stack_setUint32(pWeta->stack, location + index * sizeof(int32_t), (uint32_t)rhs);
		return true;

	case OP_AGET:
		//Serial.println("---aget---");
		weta_stack_popStackPtr(pWeta->stack, &location);
		weta_stack_popStackPtr(pWeta->stack, &index);
		weta_stack_getUint32(pWeta->stack, location + index * sizeof(int32_t), (uint32_t*)&rhs);
		weta_stack_pushUint32(pWeta->stack, (uint32_t)rhs);
		return true;

	case OP_OUTPUT:
		{
			//Serial.print("output ");
			weta_stack_popUint32(pWeta->stack, (uint32_t*)&rhs);	// The return value
				// point to where the arguments size is on the stack.
			location = getArgsLocation(&pWeta->regs);
			uint8_t argsSize;
			weta_stack_getUint8(pWeta->stack, location - sizeof(uint8_t), &argsSize); // Get size of args. 
				// If the size > 0 then step over the size location too. 
				// Otherwise keep pointing to the same place because the return
				// value should overwrite where the arguments size would be. 
			if (argsSize > 0)
				argsSize += sizeof(uint8_t);
			weta_stack_setUint32(pWeta->stack, location - (WetaStackPtr)argsSize - sizeof(int32_t), (uint32_t)rhs);
		}
		return true;

	case OP_TX:
		{
			//Serial.println("---tx---");
			weta_stack_popUint32(pWeta->stack, (uint32_t*)&rhs);
			uint8_t nbuf[sizeof(int32_t)];
			hton_int32(rhs, nbuf);
			hw_serial_write(pWeta->sport, nbuf, sizeof(int32_t));
		}
		return true;

	case OP_RX:
		{
			//Serial.println("---rx---");
			uint8_t nbuf[sizeof(int32_t)];
			hw_serial_read(pWeta->sport, nbuf, sizeof(int32_t), -1);
			weta_stack_pushUint32(pWeta->stack, (uint32_t)ntoh_int32(nbuf));
		}
		return true;
		
#ifdef SUPPORT_STRING
	case OP_TOSTR:
		{
			//Serial.println("---tostr---");
			weta_stack_popUint32(pWeta->stack, (uint32_t*)&rhs);
				// ***KLUDGE WARNING***
				// Borrowing unused (hopefully) stack space as a buffer!
				// just to avoid having to allocate another buffer.
			
			char* psz = (char *)weta_stack_getTopAddress(pWeta->stack);
			weta_sprintf(psz, pWeta->formats.pszIntFormat, rhs);
			//itoa((int) rhs, psz, 10);
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
				uint32_t tmpUint32;
				weta_stack_popUint32(pWeta->stack, &tmpUint32);
			}

			if (itemsRemaining > 0)	// Any items remaining?
			{
					// Set the value of the variable to the location on the
					// stack of the next list item (ie the top)
				weta_stack_setStackPtr(pWeta->stack, location, weta_stack_getTop(pWeta->stack) - sizeof(int32_t));
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

	weta_stack_popUint32(pWeta->stack, (uint32_t*)&rhs);

	switch (pWeta->regs.opCode)
	{
	case OP_BITNOT:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)~rhs);
		return true;

	case OP_ABS:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)abs(rhs));
		return true;

	case OP_NEG:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)-rhs);
		return true;
	}

	weta_stack_popUint32(pWeta->stack, (uint32_t*)&lhs);

	switch (pWeta->regs.opCode)
	{
	case OP_ADD:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs + rhs));
		return true;

	case OP_SUB:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs - rhs));
		return true;

	case OP_MUL:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs * rhs));
		return true;

	case OP_DIV:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs / rhs));
		return true;

	case OP_MOD:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs % rhs));
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
		weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs & rhs));
		return true;

	case OP_BITOR:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs | rhs));
		return true;

	case OP_BITXOR:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)(lhs ^ rhs));
		return true;
	
	case OP_MIN:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)min_int32(lhs, rhs));
		return true;	
	case OP_MAX:
		weta_stack_pushUint32(pWeta->stack, (uint32_t)max_int32(lhs, rhs));
		return true;
	}
	return false;
}
#endif
