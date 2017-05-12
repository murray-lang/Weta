#include <weta_platform.h>
#include <weta.h>

#include "lib_math.h"
#include "../WvmCodes.h"
#include <stdlib.h>
#if defined (SUPPORT_FLOAT) || defined (SUPPORT_DOUBLE)
#include <math.h>
#endif

#ifdef SUPPORT_FLOAT
static bool math_float(Weta* pWeta);
#endif
#ifdef SUPPORT_DOUBLE
static bool math_double(Weta* pWeta);
#endif

void WETAFUNCATTR
lib_math_handler(Weta* pWeta)
{
    if (weta_store_read_byte(pWeta->store, pWeta->regs.pc, &pWeta->regs.opCode) == 0) {
        DEBUGMSG("Unable to read byte from code store.\n");
        weta_reset(pWeta);
        return;
    }
    DEBUGMSG("lib_math_handler() - weta_store_read_byte(%d) = %d\r\n", pWeta->regs.pc, pWeta->regs.opCode);
    pWeta->regs.pc++;

    switch (pWeta->regs.opCode) {
    case OP_MATH_RANDOM:
        weta_stack_pushUint16(pWeta->stack, (uint16_t)((32767 * rand()) / RAND_MAX));
        break;

    case OP_MATH_RANDOMXY:
        {
            int16_t x;
            int16_t y;
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&y);
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&x);
            // Nasty hack.
            // Assumes y > x.
            // Use the difference as the range, then add x to shift
            // the result back into the desired range.
            // Note that though the result is cast as unsigned,
            // when it is popped it can still be interpreted as signed.
            weta_stack_pushUint16(
                pWeta->stack,
                (uint16_t)((((y-x) * rand()) / RAND_MAX) + x)
            );
        }
        break;
    default:
#if defined (SUPPORT_FLOAT) || defined (SUPPORT_DOUBLE)
        if (pWeta->regs.withCode == OP_WITHFLOAT) {
#ifdef SUPPORT_FLOAT
            math_float(pWeta);
#endif
        }
        else if (pWeta->regs.withCode == OP_WITHDOUBLE) {
#ifdef SUPPORT_DOUBLE
            math_double(pWeta);
#endif
        }
#endif // support either
    }
}


#ifdef SUPPORT_FLOAT
static bool WETAFUNCATTR
math_float(Weta* pWeta)
{
    float  rhs, lhs;
    weta_stack_popFloat(pWeta->stack, &rhs);	// rhs

    switch (pWeta->regs.opCode)
    {
    case OP_MATH_SQR:
        weta_stack_pushFloat(pWeta->stack, rhs * rhs);
        return true;
    case OP_MATH_SQRT:
        weta_stack_pushFloat(pWeta->stack, (float)sqrt((double)rhs));
        return true;
    case OP_MATH_EXP:
        weta_stack_pushFloat(pWeta->stack, (float)exp((double)rhs));
        return true;
    case OP_MATH_SIN:
        //Serial.print("sin (");
        //Serial.print(rhs);
        //Serial.print(") ");
        weta_stack_pushFloat(pWeta->stack, (float)sin((double)rhs));
        return true;
    case OP_MATH_COS:
        weta_stack_pushFloat(pWeta->stack, (float)cos((double)rhs));
        return true;
    case OP_MATH_TAN:
        weta_stack_pushFloat(pWeta->stack, (float)tan((double)rhs));
        return true;
    case OP_MATH_ASIN:
        weta_stack_pushFloat(pWeta->stack, (float)asin((double)rhs));
        return true;
    case OP_MATH_ACOS:
        weta_stack_pushFloat(pWeta->stack, (float)acos((double)rhs));
        return true;
    case OP_MATH_ATAN:
        weta_stack_pushFloat(pWeta->stack, (float)atan((double)rhs));
        return true;
    case OP_MATH_SINH:
        weta_stack_pushFloat(pWeta->stack, (float)sinh((double)rhs));
        return true;
    case OP_MATH_COSH:
        weta_stack_pushFloat(pWeta->stack, (float)cosh((double)rhs));
        return true;
    case OP_MATH_TANH:
        weta_stack_pushFloat(pWeta->stack, (float)tanh((double)rhs));
        return true;
    case OP_MATH_LN:
        weta_stack_pushFloat(pWeta->stack, (float)log((double)rhs));
        return true;
    case OP_MATH_LOG10:
        weta_stack_pushFloat(pWeta->stack, (float)log10((double)rhs));
        return true;
    case OP_MATH_RND:
        weta_stack_pushFloat(pWeta->stack, (float)round((double)rhs));
        return true;
    case OP_MATH_TRUNC:
        weta_stack_pushFloat(pWeta->stack, (float)trunc((double)rhs));
        return true;
    case OP_MATH_FLOOR:
        weta_stack_pushFloat(pWeta->stack, (float)floor((double)rhs));
        return true;
    case OP_MATH_CEIL:
        weta_stack_pushFloat(pWeta->stack, (float)ceil((double)rhs));
        return true;
    case OP_MATH_ISNAN:
        weta_stack_pushFloat(pWeta->stack, (float)isnan((double)rhs));
        return true;
    case OP_MATH_ISINF:
        weta_stack_pushFloat(pWeta->stack, (float)isinf((double)rhs));
        return true;

    }
    weta_stack_popFloat(pWeta->stack, &lhs);	// lhs

    switch (pWeta->regs.opCode)
    {
    case OP_MATH_POW:
        weta_stack_pushFloat(pWeta->stack, (float)pow((double)lhs, (double)rhs));
        return true;
    case OP_MATH_HYPOT:
        weta_stack_pushFloat(pWeta->stack, (float)hypot((double)lhs, (double)rhs));
        return true;
    case OP_MATH_ATAN2:
        weta_stack_pushFloat(pWeta->stack, (float)atan2((double)lhs, (double)rhs));
        return true;
    }

    return false;
}
#endif //SUPPORT_FLOAT

#ifdef SUPPORT_DOUBLE
static bool WETAFUNCATTR
math_double(Weta* pWeta)
{
    double  rhs, lhs;
    weta_stack_popDouble(pWeta->stack, &rhs);	// rhs

    switch (pWeta->regs.opCode)
    {
    case OP_MATH_SQR:
        weta_stack_pushDouble(pWeta->stack, rhs * rhs);
        return true;
    case OP_MATH_SQRT:
        weta_stack_pushDouble(pWeta->stack, sqrt(rhs));
        return true;
    case OP_MATH_EXP:
        weta_stack_pushDouble(pWeta->stack, exp(rhs));
        return true;
    case OP_MATH_SIN:
        //Serial.print("sin (");
        //Serial.print(rhs);
        //Serial.print(") ");
        weta_stack_pushDouble(pWeta->stack, sin(rhs));
        return true;
    case OP_MATH_COS:
        weta_stack_pushDouble(pWeta->stack, cos(rhs));
        return true;
    case OP_MATH_TAN:
        weta_stack_pushDouble(pWeta->stack, tan(rhs));
        return true;
    case OP_MATH_ASIN:
        weta_stack_pushDouble(pWeta->stack, asin(rhs));
        return true;
    case OP_MATH_ACOS:
        weta_stack_pushDouble(pWeta->stack, acos(rhs));
        return true;
    case OP_MATH_ATAN:
        weta_stack_pushDouble(pWeta->stack, atan(rhs));
        return true;
    case OP_MATH_SINH:
        weta_stack_pushDouble(pWeta->stack, sinh(rhs));
        return true;
    case OP_MATH_COSH:
        weta_stack_pushDouble(pWeta->stack, cosh(rhs));
        return true;
    case OP_MATH_TANH:
        weta_stack_pushDouble(pWeta->stack, tanh(rhs));
        return true;
    case OP_MATH_LN:
        weta_stack_pushDouble(pWeta->stack, log(rhs));
        return true;
    case OP_MATH_LOG10:
        weta_stack_pushDouble(pWeta->stack, log10(rhs));
        return true;
    case OP_MATH_RND:
        weta_stack_pushDouble(pWeta->stack, round(rhs));
        return true;
    case OP_MATH_TRUNC:
        weta_stack_pushDouble(pWeta->stack, trunc(rhs));
        return true;
    case OP_MATH_FLOOR:
        weta_stack_pushDouble(pWeta->stack, floor(rhs));
        return true;
    case OP_MATH_CEIL:
        weta_stack_pushDouble(pWeta->stack, ceil(rhs));
        return true;
    case OP_MATH_ISNAN:
        weta_stack_pushDouble(pWeta->stack, isnan(rhs));
        return true;
    case OP_MATH_ISINF:
        weta_stack_pushDouble(pWeta->stack, isinf(rhs));
        return true;

    }
    weta_stack_popDouble(pWeta->stack, &lhs);	// lhs

    switch (pWeta->regs.opCode)
    {
    case OP_MATH_POW:
        weta_stack_pushDouble(pWeta->stack, pow(lhs, rhs));
        return true;
    case OP_MATH_HYPOT:
        weta_stack_pushDouble(pWeta->stack, hypot(lhs, rhs));
        return true;
    case OP_MATH_ATAN2:
        weta_stack_pushDouble(pWeta->stack, atan2(lhs, rhs));
        return true;
    }

    return false;
}
#endif // SUPPORT_DOUBLE
