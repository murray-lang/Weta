#include "board.h"

#if defined(CONFIG_TARGET_BOARD_MYNANO32)
#include "mynano32/mynano32_board.c"
#include "mynano32/nano32_pin_caps.c"
#elif defined(CONFIG_TARGET_BOARD_CARLOS)
#include "carlos/carlos_board.c"
#endif
