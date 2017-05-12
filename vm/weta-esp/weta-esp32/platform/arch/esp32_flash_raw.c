//
// Created by murray on 23/12/16.
//
#include <hw_flash_raw.h>
#include <esp_spi_flash.h>


FlashRawResult WETAFUNCATTR
hw_flash_raw_write (WetaFlashPtr dest, const void *src, size_t size)
{
    return (FlashRawResult)spi_flash_write(dest, src, size);
}

FlashRawResult WETAFUNCATTR
hw_flash_raw_erase_sector (size_t sector)
{
    return (FlashRawResult)spi_flash_erase_sector(sector);
}

FlashRawResult WETAFUNCATTR
hw_flash_raw_read (WetaFlashPtr src, void *dest, size_t size)
{
    return (FlashRawResult)spi_flash_read(src, dest, size);
}
