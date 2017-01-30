
LOGO_VM_SRC	= ${NVM_SRC}/weta.c              \
				${NVM_SRC}/weta_store.c      \
				${NVM_SRC}/weta_stack.c      \
				${NVM_SRC}/hw/hw.c           \
                ${NVM_SRC}/hw/${PLATFORM}/hw_board.c     \
				${NVM_SRC}/hw/${PLATFORM}/hw_adc.c       \
				${NVM_SRC}/hw/${PLATFORM}/hw_buzzer.c    \
				${NVM_SRC}/hw/${PLATFORM}/hw_dac.c       \
				${NVM_SRC}/hw/${PLATFORM}/hw_endian.c    \
				${NVM_SRC}/hw/${PLATFORM}/hw_gpio.c      \
				${NVM_SRC}/hw/${PLATFORM}/hw_i2c.c       \
				${NVM_SRC}/hw/${PLATFORM}/hw_motor.c     \
				${NVM_SRC}/hw/${PLATFORM}/hw_serial.c    \
				${NVM_SRC}/hw/${PLATFORM}/hw_servo.c     \
				${NVM_SRC}/hw/${PLATFORM}/hw_time.c 

# Required include directories
LOGO_VM_INC = ${NVM_SRC}
