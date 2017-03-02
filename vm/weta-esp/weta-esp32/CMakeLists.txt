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
    platform/arch/hw_adc.c
    platform/arch/hw_buzzer.c
    platform/arch/hw_dac.c
    platform/arch/hw_endian.c
    platform/arch/hw_flash_raw.c
    platform/arch/hw_gpio.c
    platform/arch/hw_i2c.c
    platform/arch/hw_pwm.c
    platform/arch/hw_serial.c
    platform/arch/hw_time.c
    platform/boards/board.c
    platform/boards/board.h
    platform/boards/mynano32/board.c
    platform/boards/mynano32/board.h
    platform/boards/carlos/board.c
    platform/boards/carlos/board.h
)

add_subdirectory(../../weta-common weta-common)
add_subdirectory(../weta-esp-common weta-esp-common)
add_executable(weta-esp32 ${SOURCE_FILES})
