cmake_minimum_required(VERSION 3.3)
project(vm)

INCLUDE_DIRECTORIES(
        /home/murray/Robotics/Weta/vm/weta-esp32/include
        /home/murray/Robotics/Weta/vm/weta-esp32/boards
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
    src/hw/esp32/hw_misc.c
    src/hw/esp32/hw_adc.c
    src/hw/esp32/hw_buzzer.c
    src/hw/esp32/hw_dac.c
    src/hw/esp32/hw_endian.c
    src/hw/esp32/hw_flash_raw.c
    src/hw/esp32/hw_gpio.c
    src/hw/esp32/hw_i2c.c
    src/hw/esp32/hw_pwm.c
    src/hw/esp32/hw_serial.c
    src/hw/esp32/hw_time.c
    src/hw/hw_servo.c
    src/hw/hw_stepper.c
    src/hw/hw_motor.c
    src/hw/hw.c
    src/hw/hw.h
    src/hw/hw_adc.h
    src/hw/hw_buzzer.h
    src/hw/hw_dac.h
    src/hw/hw_endian.h
    src/hw/hw_flash.c
    src/hw/hw_flash.h
    src/hw/hw_flash_raw.h
    src/hw/hw_gpio.h
    src/hw/hw_i2c.h
    src/hw/hw_motor.h
    src/hw/hw_pwm.h
    src/hw/hw_serial.h
    src/hw/hw_servo.h
    src/hw/hw_stepper.h
    src/hw/hw_time.h
    src/type_handlers/type_handlers.h
    src/type_handlers/with_bool.c
    src/type_handlers/with_double.c
    src/type_handlers/with_float.c
    src/type_handlers/with_int16.c
    src/type_handlers/with_int32.c
    src/type_handlers/with_int8.c
    src/type_handlers/with_stackptr.c
    src/type_handlers/with_string.c
    src/type_handlers/with_uint16.c
    src/type_handlers/with_uint32.c
    src/type_handlers/with_uint8.c
    src/turtle/weta_turtle.h
    src/turtle/weta_turtle_stepper.c
    src/weta.c
    src/weta.h
    src/weta_defs.h
    src/weta_stack.c
    src/weta_stack.h
    src/weta_store.c
    src/weta_store.h
    src/WvmBaseCodes.h
    src/WvmCodes.h
    src/WvmCommCodes.h
    src/WvmIoCodes.h
    src/WvmMathCodes.h
    weta-esp32/main/main.c
    weta-esp32/include/weta_platform.h
    weta-esp32/boards/board.c
    weta-esp32/boards/board.h
    weta-esp32/boards/mynano32/board.c
    weta-esp32/boards/mynano32/board.h
    weta-esp32/boards/carlos/board.c
    weta-esp32/boards/carlos/board.h
      weta-esp32/include/webserver/user_webserver.h
    weta-esp32/webserver/user_webserver.c
    weta-esp32/Makefile)

add_executable(vm ${SOURCE_FILES})


