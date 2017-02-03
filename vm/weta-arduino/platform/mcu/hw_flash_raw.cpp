//
// Created by murray on 23/12/16.
//
#include <hw_flash_raw.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
extern "C" {
FlashRawResult WETAFUNCATTR
hw_flash_raw_write(WetaFlashPtr dest, const void *src, size_t size)
{
    // TODO: Implement write.
    return FLASH_RAW_RESULT_ERR;
}

FlashRawResult WETAFUNCATTR
hw_flash_raw_erase_sector(size_t sector)
{
    boot_page_erase(sector);
    return FLASH_RAW_RESULT_OK;
}

FlashRawResult WETAFUNCATTR
hw_flash_raw_read(WetaFlashPtr src, void *dest, size_t size)
{
    if (memcpy_PF(dest, src, size))
        return FLASH_RAW_RESULT_OK;
    else
        return FLASH_RAW_RESULT_ERR;
}
}
