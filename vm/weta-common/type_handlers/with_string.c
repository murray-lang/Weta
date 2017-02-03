#include "type_handlers.h"

#ifdef SUPPORT_STRING
#include <string.h>

bool WETAFUNCATTR 
with_string(Weta* pWeta)
{
	//Serial.println("---withstring---");
	WetaStackPtr location;
	uint8_t* psz;

	switch (pWeta->regs.opCode)
	{
	case OP_ENCODE:
		{
				// Convert a string to a list of bytes
			uint8_t size;
			weta_stack_popStackPtr(pWeta->stack, &location);
			weta_stack_popUint8(pWeta->stack, &size);
			weta_stack_getString(pWeta->stack, location, &psz);
			//sprintf(szMsg, "OP_ENCODE Location=%d size=%d\r\n", location, size);
			//weta_debug(pWeta, szMsg);
			//weta_stack_dump(pWeta->stack);
			
				// Lists items are pushed from last to first, so get
				// the characters from the end of the array and work backwards.
			int16_t i;
			for (i = size -1; i >=0; i--)
			{
				weta_stack_pushUint8(pWeta->stack, (uint8_t)psz[i]);
			}
				// Lastly, push the number of items in the list
			weta_stack_pushUint8(pWeta->stack, size);
		}
		return true;
		
	case OP_DECODE:
		{
				// Convert an array of bytes into a string.
			uint8_t size;
			weta_stack_popStackPtr(pWeta->stack, &location);
			weta_stack_popUint8(pWeta->stack, &size);
			//sprintf(szMsg, "OP_DECODE Location=%d size=%d\r\n", location, size);
			//weta_debug(pWeta, szMsg);
				//Get a pointer to the array of bytes on the stack.
			uint8_t* bytes = (uint8_t*)weta_stack_getStackAddress(pWeta->stack, location);
				// ***KLUDGE WARNING***
				// Borrowing unused (hopefully) stack space as a buffer!
				// (To avoid having to allocate another buffer.
			psz = (uint8_t*)weta_stack_getTopAddress(pWeta->stack);
				// Copy the bytes to the temporary space beyond the stack
			strncpy((char *)psz, (char *)bytes, (size_t)size);
			psz[size] = 0;	// Terminate the string
			weta_stack_pushString(pWeta->stack, psz);
				// Lastly, push the number of items in the list
			weta_stack_pushUint8(pWeta->stack, 1);
		}
		return true;
		
	case OP_EQ:
		{
				// Get a pointer to the string on weta_stack_top of the stack
				// (ie. rhs)
			weta_stack_topString(pWeta->stack, &psz);
				// Pop the stack to get to the lhs string.
				// Yes, the pointer we just got will be pointing past the weta_stack_top
				// of the stack, but that is only dangerous if we start weta_stack_pushing.
			weta_stack_popString(pWeta->stack);
				// Get the next weta_stack_top string (lhs)	
			uint8_t* lhs;
			weta_stack_topString(pWeta->stack, &lhs);
			weta_stack_popString(pWeta->stack);
			//Serial.print((char*)lhs); Serial.print(" == "); Serial.print((char*)psz);Serial.println("?");
				// Push a boolean based on a comparison (case sensitive)
			weta_stack_pushUint8(pWeta->stack, (uint8_t)strcmp((const char *)lhs, (const char *)psz) == 0);
		}
		return true;
	case OP_SET:
		{
			//Serial.print("set ");
			weta_stack_popStackPtr(pWeta->stack, &location);
				// Copy the string directly from the stack top to avoid the need
				// for another buffer.
				// Get a pointer to where the string is on the stack.
			weta_stack_topString(pWeta->stack, &psz);
			//Serial.print((char*)psz); Serial.print("("); Serial.print((uint16_t)psz); Serial.println(")");
			weta_stack_setString(pWeta->stack, location, psz);
			weta_stack_popString(pWeta->stack);	// Now dispose of the string at the top
		}
		return true;

	case OP_GET:
		{
			//Serial.print("get ");
			weta_stack_popStackPtr(pWeta->stack, &location);
			weta_stack_getString(pWeta->stack, location, &psz);
			//Serial.print((char*)psz); Serial.print("("); Serial.print((uint16_t)psz); Serial.println(")");
			weta_stack_pushString(pWeta->stack, psz);
		}
		return true;

	case OP_TX:
		{
			//Serial.print("---tx---");
			weta_stack_topString(pWeta->stack, &psz);
			//Serial.print((const char*)psz);
			//Serial.println("\"");
			// TO DO: replace _defaultStream->write((const char*)psz);
			//Serial.print("' [");
			//Serial.print(weta_stack_getStringTop(pWeta->stack));
			//Serial.print(" ("); Serial.print((uint16_t)psz); Serial.print(")"); 
			weta_stack_popString(pWeta->stack);
			hw_serial_write(pWeta->sport, psz, strlen((char *)psz));
			//Serial.print(", "); Serial.print(weta_stack_getStringTop(pWeta->stack)); Serial.println("]");
		}
		return true;

	case OP_OUTPUT:
		{
			//Serial.print("output: ");
			WetaStackPtr strPtr;
				// The return string is at the top of the stack, but what's
				// actually on top of the stack is a stack pointer to the 
				// string on a separate hidden stack. This pointer is what
				// will be returned.
			weta_stack_topStackPtr(pWeta->stack, &strPtr);	// The return value
			uint8_t* psz;
			weta_stack_topString(pWeta->stack, &psz);
			//Serial.print((char*)psz);
			//Serial.print(" [");
			//Serial.print(weta_stack_getStringTop(pWeta->stack));
			//Serial.print(", ");	
			
				// ***NOTE CAREFULLY - BAD STUFF***
				// We need to pop the string because the calling code expects 
				// the parameters that it pushed to be at the top of the stack
				// when the function returns. 
				// However, the string will actually still exist in the land of
				// the dead beyond the stack top. The pointer we obtained above 
				// still points to it.
			weta_stack_popString(pWeta->stack);
			//Serial.print(weta_stack_getStringTop(pWeta->stack));
			//Serial.print("] "); Serial.print(strPtr); Serial.print("->"); 
			//Serial.print((uint16_t)weta_stack_getStackAddress(pWeta->stack, strPtr)); 
				// point to where the arguments size is on the stack.
			location = getArgsLocation(&pWeta->regs);

			uint8_t argsSize;
			weta_stack_getUint8(pWeta->stack, location - sizeof(uint8_t), &argsSize); // Get size of args. 
				// Functions that return strings must always have the args size
				// byte supplied. Also add the size of the arg size byte itself. 
			argsSize += sizeof(uint8_t);
				// Replace the string pointer in the return value location with 
				// the one that points to the return string.
			//Serial.print("->"); Serial.println(location - (WetaStackPtr)argsSize - sizeof(WetaStackPtr));
			weta_stack_setStackPtr(pWeta->stack, location - (WetaStackPtr)argsSize - sizeof(WetaStackPtr), strPtr);
		}
		return true;

	case OP_FOREACH:
		{	
			//Serial.println("---foreach---");
			WetaCodePtr blockAddr;
			weta_stack_popCodePtr(pWeta->stack, &blockAddr);		// Block address
			//Serial.println(location);
			weta_stack_popStackPtr(pWeta->stack, &location);  // Iterator variable location (Variable contains a 'pointer')
			//Serial.println(iteratorLoc);
			uint8_t itemsRemaining;
			weta_stack_popUint8(pWeta->stack, &itemsRemaining);   // Number of items remaining
			//Serial.println(itemsRemaining);
				// If we've gone through the block then we need to weta_stack_pop the
				// last list item off the stack
			if (hasBlockExecuted(&pWeta->regs))
				weta_stack_popString(pWeta->stack);

			if (itemsRemaining > 0)				// Any items remaining
			{
					// Set the value of the variable to the location on the
					// stack of the next list item (ie the weta_stack_top)
					// Note that the main stack will only contain a pointer to
					// a second stack set up for strings.
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
#endif

