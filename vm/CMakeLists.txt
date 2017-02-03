cmake_minimum_required(VERSION 3.3)
project(weta)

INCLUDE_DIRECTORIES(
        /home/murray/Robotics/Weta/vm/weta-esp32/include
        /home/murray/Robotics/Weta/vm/weta-esp32/platform
        /home/murray/Robotics/Weta/vm/weta-esp32/platform/boards
        /home/murray/Robotics/Weta/vm/weta-esp32/build/include
        /home/murray/Robotics/Weta/vm/weta-common
        /home/murray/Robotics/Weta/vm/weta-common/hw
        /home/murray/ESP/esp-idf-v2.0/components/driver/include
        /home/murray/ESP/esp-idf-v2.0/components/esp32/include
        /home/murray/ESP/esp-idf-v2.0/components/freertos/include
        /home/murray/ESP/esp-idf-v2.0/components/spi_flash/include
        /home/murray/ESP/esp-idf-v2.0/components/lwip/include/lwip
        /home/murray/ESP/esp-idf-v2.0/components/tcpip_adapter/include
        /home/murray/ESP/esp-idf-v2.0/components/json/include
        /home/murray/ESP/xtensa-esp32-elf/xtensa-esp32-elf/include
        weta-esp32/build/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES

    weta-common/hw/hw_servo.c
    weta-common/hw/hw_stepper.c
    weta-common/hw/hw_motor.c
    weta-common/hw/hw.c
    weta-common/hw/hw.h
    weta-common/hw/hw_adc.h
    weta-common/hw/hw_buzzer.h
    weta-common/hw/hw_dac.h
    weta-common/hw/hw_endian.h
    weta-common/hw/hw_flash.c
    weta-common/hw/hw_flash.h
    weta-common/hw/hw_flash_raw.h
    weta-common/hw/hw_gpio.h
    weta-common/hw/hw_i2c.h
    weta-common/hw/hw_motor.h
    weta-common/hw/hw_pwm.h
    weta-common/hw/hw_serial.h
    weta-common/hw/hw_servo.h
    weta-common/hw/hw_stepper.h
    weta-common/hw/hw_time.h
    weta-common/type_handlers/type_handlers.h
    weta-common/type_handlers/with_bool.c
    weta-common/type_handlers/with_double.c
    weta-common/type_handlers/with_float.c
    weta-common/type_handlers/with_int16.c
    weta-common/type_handlers/with_int32.c
    weta-common/type_handlers/with_int8.c
    weta-common/type_handlers/with_stackptr.c
    weta-common/type_handlers/with_string.c
    weta-common/type_handlers/with_uint16.c
    weta-common/type_handlers/with_uint32.c
    weta-common/type_handlers/with_uint8.c
    weta-common/turtle/weta_turtle.h
    weta-common/turtle/weta_turtle_stepper.c
    weta-common/weta.c
    weta-common/weta.h
    weta-common/weta_defs.h
    weta-common/weta_stack.c
    weta-common/weta_stack.h
    weta-common/weta_store.c
    weta-common/weta_store.h
    weta-common/WvmBaseCodes.h
    weta-common/WvmCodes.h
    weta-common/WvmCommCodes.h
    weta-common/WvmIoCodes.h
    weta-common/WvmMathCodes.h
    weta-esp32/main/main.c
    weta-esp32/platform/weta_platform.h
    weta-esp32/platform/mcu/hw_adc.c
    weta-esp32/platform/mcu/hw_buzzer.c
    weta-esp32/platform/mcu/hw_dac.c
    weta-esp32/platform/mcu/hw_endian.c
    weta-esp32/platform/mcu/hw_flash_raw.c
    weta-esp32/platform/mcu/hw_gpio.c
    weta-esp32/platform/mcu/hw_i2c.c
    weta-esp32/platform/mcu/hw_pwm.c
    weta-esp32/platform/mcu/hw_serial.c
    weta-esp32/platform/mcu/hw_time.c
    weta-esp32/platform/boards/board.c
    weta-esp32/platform/boards/board.h
    weta-esp32/platform/boards/mynano32/board.c
    weta-esp32/platform/boards/mynano32/board.h
    weta-esp32/platform/boards/carlos/board.c
    weta-esp32/platform/boards/carlos/board.h
      weta-esp32/include/webserver/user_webserver.h
    weta-esp32/webserver/user_webserver.c
    weta-esp32/Makefile)

add_executable(vm ${SOURCE_FILES})


