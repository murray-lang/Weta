cmake_minimum_required(VERSION 3.3)
project(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    hw/esp32/hw_adc.c
    hw/esp32/hw_board.c
    hw/esp32/hw_board.h
    hw/esp32/hw_buzzer.c
    hw/esp32/hw_dac.c
    hw/esp32/hw_endian.c
    hw/esp32/hw_flash.c
    hw/esp32/hw_gpio.c
    hw/esp32/hw_i2c.c
        hw/hw_motor.c
    hw/esp32/hw_pwm.c
    hw/esp32/hw_serial.c
        hw/hw_servo.c
        hw/hw_stepper.c
    hw/esp32/hw_time.c
    hw/hw.c
    hw/hw.h
    hw/hw_adc.h
    hw/hw_buzzer.h
    hw/hw_dac.h
    hw/hw_endian.h
    hw/hw_flash.h
    hw/hw_gpio.h
    hw/hw_i2c.h
    hw/hw_motor.h
    hw/hw_pwm.h
    hw/hw_serial.h
    hw/hw_servo.h
    hw/hw_stepper.h
    hw/hw_time.h
    type_handlers/type_handlers.h
    type_handlers/with_bool.c
    type_handlers/with_double.c
    type_handlers/with_float.c
    type_handlers/with_int16.c
    type_handlers/with_int32.c
    type_handlers/with_int8.c
    type_handlers/with_stackptr.c
    type_handlers/with_string.c
    type_handlers/with_uint16.c
    type_handlers/with_uint32.c
    type_handlers/with_uint8.c
    weta.c
    weta.h
    weta_defs.h
    weta_stack.c
    weta_stack.h
    weta_store.c
    weta_store.h
        turtle/weta_turtle.c
        turtle/weta_turtle.h
    WvmBaseCodes.h
    WvmCodes.h
    WvmCommCodes.h
    WvmIoCodes.h
    WvmMathCodes.h)

add_executable(src ${SOURCE_FILES})