#ifndef VM_BOARD_H
#define VM_BOARD_H

#include  <sdkconfig.h>

extern void init_board(void);

#if defined(CONFIG_TARGET_BOARD_MYNANO32)
#include "mynano32/mynano32_board.h"
#elif defined(CONFIG_TARGET_BOARD_CARLOS)
#include "carlos/carlos_board.h"
#endif

#endif // VM_BOARD_H
