#ifndef __TYPE_HANDLERS_H__
#define __TYPE_HANDLERS_H__

#include <weta_platform.h>
#include "../weta.h"
#include "../WvmCodes.h"
#include "../weta_stack.h"
#include <stdlib.h>
#include <stdio.h>

bool with_int8(Weta* pWeta);
bool with_uint8(Weta* pWeta);
bool with_int16(Weta* pWeta);
bool with_uint16(Weta* pWeta);
bool with_bool(Weta* pWeta);
bool with_stackptr(Weta* pWeta);
#ifdef SUPPORT_32BIT
bool with_int32(Weta* pWeta);
bool with_uint32(Weta* pWeta);
#endif
#ifdef SUPPORT_FLOAT
bool with_float(Weta* pWeta);
#endif
#ifdef SUPPORT_DOUBLE
bool with_double(Weta* pWeta);
#endif
#ifdef SUPPORT_STRING
bool with_string(Weta* pWeta);
#endif

#endif // __TYPE_HANDLERS_H__
