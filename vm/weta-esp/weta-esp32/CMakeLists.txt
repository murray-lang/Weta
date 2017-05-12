cmake_minimum_required(VERSION 3.3)
project(weta-esp32)

INCLUDE_DIRECTORIES(
        platform
        platform/boards
        build/include
        ../weta-esp-common
        ../../weta-common
        ../../weta-common/hw
        /home/murray/ESP/esp-idf-v2.0/components/driver/include
        /home/murray/ESP/esp-idf-v2.0/components/esp32/include
        /home/murray/ESP/esp-idf-v2.0/components/freertos/include
        /home/murray/ESP/esp-idf-v2.0/components/spi_flash/include
        /home/murray/ESP/esp-idf-v2.0/components/lwip/include/lwip
        /home/murray/ESP/esp-idf-v2.0/components/tcpip_adapter/include
        /home/murray/ESP/esp-idf-v2.0/components/json/include
        /home/murray/ESP/esp-idf-v2.0/components/nvs_flash/include
        /home/murray/ESP/xtensa-esp32-elf/xtensa-esp32-elf/include
       ./build/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main/main.c
    platform/weta_platform.h
    platform/hw_defs.h
    platform/arch/esp32_adc.c
    platform/arch/esp32_buzzer.c
    platform/arch/esp32_dac.c
    platform/arch/esp32_endian.c
    platform/arch/esp32_flash_raw.c
    platform/arch/esp32_gpio.c
    platform/arch/esp32_i2c.c
    platform/arch/esp32_pwm.c
    platform/arch/esp32_serial.c
    platform/arch/esp32_time.c
    platform/boards/board.c
    platform/boards/board.h
    platform/boards/mynano32/mynano32_board.c
    platform/boards/mynano32/mynano32_board.h
    platform/boards/mynano32/nano32_pin_caps.c
    platform/boards/carlos/carlos_board.c
    platform/boards/carlos/carlos_board.h
)

add_subdirectory(../../weta-common weta-common)
add_subdirectory(../weta-esp-common weta-esp-common)
add_executable(weta-esp32 ${SOURCE_FILES})
