//
// Created by murray on 23/12/16.
//
#include <hw_flash_raw.h>
#include <spi_flash.h>


FlashRawResult WETAFUNCATTR
hw_flash_raw_write (WetaFlashPtr dest, const void *src, size_t size)
{
    //const uint8_t* dbg = src;
    //DEBUGMSG("hw_flash_raw_write(%0X, [%0X, %0X, %0X, %0X], %d)\r\n", dest, dbg[0], dbg[1], dbg[2], dbg[3], size);
    return (FlashRawResult)spi_flash_write(dest, (void *)src, size);
}

FlashRawResult WETAFUNCATTR
hw_flash_raw_erase_sector (size_t sector)
{
    //DEBUGMSG("hw_flash_raw_erase_sector(%0X)\r\n", sector);
    return (FlashRawResult)spi_flash_erase_sector(sector);
}

FlashRawResult WETAFUNCATTR
hw_flash_raw_read (WetaFlashPtr src, void *dest, size_t size)
{
    FlashRawResult rc = (FlashRawResult)spi_flash_read(src, dest, size);
    //const uint8_t* dbg = dest;
    //DEBUGMSG("hw_flash_raw_read(%0X, [%0X, %0X, %0X, %0X], %d)\r\n", src, dbg[0], dbg[1], dbg[2], dbg[3], size);
    return rc;
}
