
// These defines are set externally in CMakeLists.txt
#if defined(TARGET_BOARD_esp12_stepper_robot)
#include "esp12_stepper_robot/board.c"
#elif defined(TARGET_BOARD_esp12_hbridge_robot)
#include "esp12_hbridge_robot/board.c"
#elif defined(TARGET_BOARD_esp01_irobot)
#include "esp01_irobot/board.c"
#endif
