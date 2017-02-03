#ifndef VM_BOARD_H
#define VM_BOARD_H

#include  <sdkconfig.h>

#if defined(CONFIG_TARGET_BOARD_MYNANO32)
#include "mynano32/board.h"
#elif defined(CONFIG_TARGET_BOARD_CARLOS)
#include "carlos/board.h"
#endif

#endif // VM_BOARD_H