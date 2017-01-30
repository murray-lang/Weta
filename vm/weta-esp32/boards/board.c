#include "board.h"

#if defined(CONFIG_TARGET_BOARD_MYNANO32)
#include "mynano32/board.c"
#elif defined(CONFIG_TARGET_BOARD_CARLOS)
#include "carlos/board.c"
#endif