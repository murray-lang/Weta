cmake_minimum_required(VERSION 3.3)
project(weta_esp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main/main.c
    ../weta-common/hw/hw.c
        ../weta-common/hw/hw.h
        ../weta-common/hw/hw_adc.h
        ../weta-common/hw/hw_buzzer.h
        ../weta-common/hw/hw_dac.h
        ../weta-common/hw/hw_endian.h
        ../weta-common/hw/hw_flash.h
        ../weta-common/hw/hw_flash.c
        ../weta-common/hw/hw_flash_raw.h
        ../weta-common/hw/hw_gpio.h
        ../weta-common/hw/hw_i2c.h
        ../weta-common/hw/hw_motor.h
        ../weta-common/hw/hw_motor.c
        ../weta-common/hw/hw_pwm.h
        ../weta-common/hw/hw_serial.h
        ../weta-common/hw/hw_servo.h
        ../weta-common/hw/hw_servo.c
        ../weta-common/hw/hw_stepper.h
        ../weta-common/hw/hw_stepper.c
        ../weta-common/hw/hw_time.h
        ../weta-common/turtle/weta_turtle.h
        ../weta-common/turtle/weta_turtle_stepper.c
        ../weta-common/type_handlers/type_handlers.h
        ../weta-common/type_handlers/with_bool.c
        ../weta-common/type_handlers/with_double.c
        ../weta-common/type_handlers/with_float.c
        ../weta-common/type_handlers/with_int16.c
        ../weta-common/type_handlers/with_int32.c
        ../weta-common/type_handlers/with_int8.c
        ../weta-common/type_handlers/with_stackptr.c
        ../weta-common/type_handlers/with_string.c
        ../weta-common/type_handlers/with_uint16.c
        ../weta-common/type_handlers/with_uint32.c
        ../weta-common/type_handlers/with_uint8.c
        ../weta-common/weta.c
        ../weta-common/weta.h
        ../weta-common/weta_defs.h
        ../weta-common/weta_stack.c
        ../weta-common/weta_stack.h
        ../weta-common/weta_store.c
        ../weta-common/weta_store.h
        ../weta-common/WvmBaseCodes.h
        ../weta-common/WvmCodes.h
        ../weta-common/WvmCommCodes.h
        ../weta-common/WvmIoCodes.h
        ../weta-common/WvmMathCodes.h)

add_executable(weta_esp ${SOURCE_FILES})
