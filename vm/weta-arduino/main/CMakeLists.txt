# Set a variable for commands below
set(PROJECT_NAME weta)

# Define your project and language
project(${PROJECT_NAME} C CXX)

INCLUDE_DIRECTORIES(
        /home/murray/Robotics/Weta/vm/weta-arduino/platform
        /home/murray/Robotics/Weta/vm/weta-arduino/platform/mcu
        /home/murray/Robotics/Weta/vm/weta-arduino/platform/boards
        /home/murray/Robotics/Weta/vm/src/hw)

set(${PROJECT_NAME}_BOARD megaADK)

# Define the source code
set(${PROJECT_NAME}_SRCS
        wetamain.cpp
        ../platform/weta_platform.h
        ../platform/boards/board.cpp
        ../platform/boards/board.h
        ../platform/mcu/hw_defs.h
        ../platform/mcu/hw_adc.cpp
        ../platform/mcu/hw_buzzer.cpp
        ../platform/mcu/hw_dac.cpp
        ../platform/mcu/hw_endian.c
        ../platform/mcu/hw_flash_raw.cpp
        ../platform/mcu/hw_gpio.cpp
        ../platform/mcu/hw_i2c.cpp
        ../platform/mcu/hw_pwm.cpp
        ../platform/mcu/hw_serial.cpp
        ../platform/mcu/hw_time.cpp
        ../../src/hw/hw_servo.c
        ../../src/hw/hw_stepper.c
        ../../src/hw/hw_motor.c
        ../../src/hw/hw.c
        ../../src/hw/hw.h
        ../../src/hw/hw_adc.h
        ../../src/hw/hw_buzzer.h
        ../../src/hw/hw_dac.h
        ../../src/hw/hw_endian.h
        ../../src/hw/hw_flash.c
        ../../src/hw/hw_flash.h
        ../../src/hw/hw_flash_raw.h
        ../../src/hw/hw_gpio.h
        ../../src/hw/hw_i2c.h
        ../../src/hw/hw_motor.h
        ../../src/hw/hw_pwm.h
        ../../src/hw/hw_serial.h
        ../../src/hw/hw_servo.h
        ../../src/hw/hw_stepper.h
        ../../src/hw/hw_time.h
        ../../src/type_handlers/type_handlers.h
        ../../src/type_handlers/with_bool.c
        ../../src/type_handlers/with_double.c
        ../../src/type_handlers/with_float.c
        ../../src/type_handlers/with_int16.c
        ../../src/type_handlers/with_int32.c
        ../../src/type_handlers/with_int8.c
        ../../src/type_handlers/with_stackptr.c
        ../../src/type_handlers/with_string.c
        ../../src/type_handlers/with_uint16.c
        ../../src/type_handlers/with_uint32.c
        ../../src/type_handlers/with_uint8.c
        ../../src/turtle/weta_turtle.h
        ../../src/turtle/weta_turtle_stepper.c
        ../../src/weta.c
        ../../src/weta.h
        ../../src/weta_defs.h
        ../../src/weta_stack.c
        ../../src/weta_stack.h
        ../../src/weta_store.c
        ../../src/weta_store.h
        ../../src/WvmBaseCodes.h
        ../../src/WvmCodes.h
        ../../src/WvmCommCodes.h
        ../../src/WvmIoCodes.h
        ../../src/WvmMathCodes.h

        )

set(${TARGET_NAME}_PROGRAMMER arduinoisp)

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT /dev/ttyUSB0)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})
