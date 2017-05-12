#include <weta_platform.h>
#include <weta.h>

#include "lib_comm.h"
#include "../WvmCodes.h"

static void comm_serial_config(Weta* pWeta);
static void comm_serial_tx(Weta* pWeta);
static void comm_serial_rx(Weta* pWeta);

void WETAFUNCATTR
lib_comm_handler(Weta* pWeta)
{
    if (weta_store_read_byte(pWeta->store,pWeta->regs.pc, &pWeta->regs.opCode) == 0)
    {
        DEBUGMSG("Unable to read byte from code store.\n");
        weta_reset(pWeta);
        return;
    }
    DEBUGMSG("lib_comm_handler() - weta_store_read_byte(%d) = %d\r\n", pWeta->regs.pc, pWeta->regs.opCode);
    pWeta->regs.pc++;

    switch (pWeta->regs.opCode) {
    case OP_COMM_SERIAL_CONFIG:
        comm_serial_config(pWeta);
        break;

    case OP_COMM_SERIAL_TX:
        comm_serial_tx(pWeta);
        break;

    case OP_COMM_SERIAL_RX:
        comm_serial_rx(pWeta);
        break;

    case OP_COMM_SERIAL_NEWRX:
        weta_stack_pushUint8(pWeta->stack, (uint8_t)weta_serial_available(pWeta->hal, 0));
        break;

    case OP_COMM_SERIAL_TXN:
        {
            //DEBUGMSG( "comm.serial.txn\r\n");
            uint8_t  port;
            uint16_t txBuffLocation;
            uint8_t  txNumBytes;
            weta_stack_popUint8(pWeta->stack, &port);
            weta_stack_popUint16(pWeta->stack, &txBuffLocation);
            weta_stack_popUint8(pWeta->stack, &txNumBytes);
            uint8_t* buffAddr = (uint8_t*)weta_stack_getStackAddress(
                pWeta->stack,
                txBuffLocation
            );
            //uint8_t rc = 42;

            uint8_t bytesWritten = weta_serial_write(
                pWeta->hal,
                port,
                buffAddr,  //(uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation),
                txNumBytes
            );
            weta_stack_pushUint8(pWeta->stack, bytesWritten);
        }
        break;
    case OP_COMM_SERIAL_RXN:
        {
            //DEBUGMSG( "comm.serial.rxn\r\n");
            uint8_t  port;
            uint16_t rxBuffLocation;
            uint8_t  rxNumBytes;
            int16_t timeout;
            weta_stack_popUint8(pWeta->stack, &port);
            weta_stack_popUint16(pWeta->stack, &rxBuffLocation);
            weta_stack_popUint8(pWeta->stack, &rxNumBytes);
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&timeout);

            uint8_t* buffAddr = (uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation);

            uint8_t bytesRead = weta_serial_read(
                pWeta->hal,
                port,
                buffAddr,  //(uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation),
                rxNumBytes,
                timeout
            );
            weta_stack_pushUint8(pWeta->stack, bytesRead);
        }
        break;

    case OP_COMM_SERIAL_NEWRXN:
        {
            uint8_t port;
            weta_stack_popUint8(pWeta->stack, &port);
            weta_stack_pushUint8(
                pWeta->stack,
                (uint8_t)weta_serial_available(pWeta->hal, port)
            );
        }
        break;

    case OP_COMM_I2C_START:
        //DEBUGMSG( "comm.i2c.start\r\n");
        weta_i2c_start(pWeta->hal);
        break;

    case OP_COMM_I2C_STOP:
        //DEBUGMSG( "comm.i2c.stop\r\n");
        weta_i2c_stop(pWeta->hal);
        break;

    case OP_COMM_I2C_WRITE:
        {
            //DEBUGMSG( "comm.i2c.write\r\n");
            uint8_t  slaveAddr;
            uint32_t registerAddr = 0;
            uint8_t  registerAddrWidth;
            uint16_t txBuffLocation;
            uint8_t  txNumBytes;
            weta_stack_popUint8(pWeta->stack, &slaveAddr);
            weta_stack_popUint8(pWeta->stack, &registerAddrWidth);
            if (registerAddrWidth == 1)
                weta_stack_popUint8(pWeta->stack, (uint8_t*)&registerAddr);
            else if (registerAddrWidth == 2)
                weta_stack_popUint16(pWeta->stack, (uint16_t*)&registerAddr);
#ifdef SUPPORT_32BIT
            else if (registerAddrWidth == 3 || registerAddrWidth == 4)
                weta_stack_popUint32(pWeta->stack, &registerAddr);
#endif

            weta_stack_popUint16(pWeta->stack, &txBuffLocation);
            weta_stack_popUint8(pWeta->stack, &txNumBytes);

            uint8_t* buffAddr = (uint8_t*)weta_stack_getStackAddress(pWeta->stack, txBuffLocation);

            //uint8_t rc =
            weta_i2c_write(
                pWeta->hal,
                slaveAddr,
                registerAddr,
                registerAddrWidth,
                buffAddr,   //(uint8_t*)weta_stack_getStackAddress(pWeta->stack, txBuffLocation),
                (uint32_t)txNumBytes
            );
            /*
            sprintf(szMsg, "hw_i2c_write(%d, %d, %d, %d, %d) = %d\r\n",
                    slaveAddr,
                    registerAddr,
                    registerAddrWidth,
                    (uint8_t)*buffAddr,
                    txNumBytes,
                    rc);
            weta_debug(pWeta, szMsg);
            */
        }
        break;

    case OP_COMM_I2C_READ:
        {
            DEBUGMSG( "comm.i2c.read\r\n");
            uint8_t  slaveAddr;
            uint32_t registerAddr = 0;
            uint8_t  registerAddrWidth;
            uint16_t rxBuffLocation;
            uint8_t  rxNumBytes;
            weta_stack_popUint8(pWeta->stack, &slaveAddr);
            weta_stack_popUint8(pWeta->stack, &registerAddrWidth);
            // A register address width of 0 means that no register
            // address is popped off the stack
            if (registerAddrWidth == 1)
                weta_stack_popUint8(pWeta->stack, (uint8_t*)&registerAddr);
            else if (registerAddrWidth == 2)
                weta_stack_popUint16(pWeta->stack, (uint16_t*)&registerAddr);
#ifdef SUPPORT_32BIT
            else if (registerAddrWidth == 4)
                weta_stack_popUint32(pWeta->stack, &registerAddr);
#endif
            weta_stack_popUint16(pWeta->stack, &rxBuffLocation);
            weta_stack_popUint8(pWeta->stack, &rxNumBytes);

            uint8_t* buffAddr = (uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation);
            //uint8_t rc = 42;

            //uint8_t rc =
            weta_i2c_read(
                pWeta->hal,
                slaveAddr,
                registerAddr,
                registerAddrWidth,
                buffAddr,  //(uint8_t*)weta_stack_getStackAddress(pWeta->stack, rxBuffLocation),
                (uint32_t)rxNumBytes
            );
            /*
            sprintf(szMsg, "hw_i2c_read(%d, %d, %d, %d, %d) = %d\r\n",
                    slaveAddr,
                    registerAddr,
                    registerAddrWidth,
                    (uint8_t)*buffAddr,
                    rxNumBytes,
                    rc);
            weta_debug(pWeta, szMsg);
            */
        }
        break;

    case OP_COMM_I2C_ERROR:
        {
#ifdef SUPPORT_32BIT
            //Serial.println("comm.i2c.error");
            // Atmel TWID API doesn't make error codes available.
            // Just return no error.
            weta_stack_pushUint32(pWeta->stack, 0);
#endif
        }
        break;
    default:
        // All of the type specific codes are dealt with here
        if (!with_current_type(pWeta))
        {
            //beep();	// Just an indication for now.
            //Serial.print("unrecognised opcode: ");
            //Serial.println(pWeta->regs.opCode);
        }
        break;
    }
}

static void WETAFUNCATTR
comm_serial_config(Weta* pWeta)
{
    SerialPort config;

    weta_stack_popUint8(pWeta->stack, &config.paramsbyte);

    uint32_t baud;

#ifdef SUPPORT_32BIT
    weta_stack_popUint32(pWeta->stack, &baud);
#else
    uint16_t baudBits;
	weta_stack_popUint16(pWeta->stack, &baudBits);	// Low 16 bits
	baud = (uint32_t)baudBits;
	weta_stack_popUint16(pWeta->stack, &baudBits);	// High 16 bits
	baud |= (baudBits << 16);
#endif
    config.baud = baud;

    weta_stack_popUint8(pWeta->stack, &config.port);

    hw_serial_config(pWeta->hal, &config);
}

static WETAFUNCATTR
void comm_serial_tx(Weta* pWeta)
{
    switch (pWeta->regs.withCode)
    {
    case OP_WITHUINT8:
    case OP_WITHINT8: {
        uint8_t val;
        weta_stack_popUint8(pWeta->stack, (uint8_t*)&val);
        hw_serial_write_byte(pWeta->sport, (uint8_t)val);
        }
        break;

    case OP_WITHINT16:
    case OP_WITHUINT16: {
        uint16_t val;
        weta_stack_popUint16(pWeta->stack, (uint16_t*)&val);
        uint8_t nbuf[sizeof(int16_t)];
        hton_int16(val, nbuf);
        hw_serial_write(pWeta->sport, nbuf, sizeof(int16_t));
        }
        break;

    case OP_WITHBOOL:
        {
            uint8_t val;
            //Serial.println("---tx---");
            weta_stack_popUint8(pWeta->stack, &val);
            hw_serial_write_byte(pWeta->sport, val);
        }
        break;
    case OP_WITHPTR:
#ifdef SUPPORT_32BIT
    case OP_WITHINT32:
    case OP_WITHUINT32: {
        uint32_t val;
        weta_stack_popUint32(pWeta->stack, (uint32_t*)&val);
        uint8_t nbuf[sizeof(int32_t)];
        hton_int32(val, nbuf);
        hw_serial_write(pWeta->sport, nbuf, sizeof(int32_t));
        }
        break;
#endif
#ifdef SUPPORT_FLOAT
    case OP_WITHFLOAT: {
        float val;
        weta_stack_popFloat(pWeta->stack, &val);
        uint8_t nbuf[sizeof(float)];
        hton_float(val, nbuf);
        hw_serial_write(pWeta->sport, nbuf, sizeof(float));
        }
        break;
#endif
#ifdef SUPPORT_DOUBLE
    case OP_WITHDOUBLE: {
        double val;
        weta_stack_popDouble(pWeta->stack, &val);
        uint8_t nbuf[sizeof(double)];
        hton_double(val, nbuf);
        hw_serial_write(pWeta->sport, nbuf, sizeof(double));
        }
        break;
#endif
#ifdef SUPPORT_STRING
    case OP_WITHSTRING: {
        uint8_t* psz;
        weta_stack_topString(pWeta->stack, &psz);
        weta_stack_popString(pWeta->stack);
        hw_serial_write(pWeta->sport, psz, strlen((char *) psz));
        }
        break;
#endif

    }
}

static WETAFUNCATTR
void comm_serial_rx(Weta* pWeta)
{
    switch (pWeta->regs.withCode)
    {
    case OP_WITHINT8:
    case OP_WITHUINT8:  {

            uint8_t val = hw_serial_read_byte(pWeta->sport);
            weta_stack_pushUint8(pWeta->stack, (uint8_t)val);
        }
        break;
    case OP_WITHINT16:
    case OP_WITHUINT16: {
        uint8_t nbuf[sizeof(int16_t)];
        hw_serial_read(pWeta->sport, nbuf, sizeof(int16_t), -1);
        weta_stack_pushUint16(pWeta->stack, (uint16_t)ntoh_int16(nbuf));
        }
        break;

    case OP_WITHBOOL: {
        uint8_t val = hw_serial_read_byte(pWeta->sport);
        weta_stack_pushUint8(pWeta->stack, val);
        }
        break;
    case OP_WITHPTR:
#ifdef SUPPORT_32BIT
    case OP_WITHINT32:
    case OP_WITHUINT32: {
        uint8_t nbuf[sizeof(int32_t)];
        hw_serial_read(pWeta->sport, nbuf, sizeof(int32_t), -1);
        weta_stack_pushUint32(pWeta->stack, (uint32_t)ntoh_int32(nbuf));
        }
        break;
#endif
#ifdef SUPPORT_FLOAT
    case OP_WITHFLOAT: {
        uint8_t nbuf[sizeof(float)];
        hw_serial_read(pWeta->sport, nbuf, sizeof(float), -1);
        weta_stack_pushFloat(pWeta->stack, ntoh_float(nbuf));
        }
        break;
#endif
#ifdef SUPPORT_DOUBLE
    case OP_WITHDOUBLE: {
        uint8_t nbuf[sizeof(double)];
        hw_serial_read(pWeta->sport, nbuf, sizeof(double), -1);
        weta_stack_pushDouble(pWeta->stack, ntoh_double(nbuf));
        }
        break;
#endif

    }
}