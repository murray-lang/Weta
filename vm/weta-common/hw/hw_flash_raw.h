//
// Created by murray on 23/12/16.
//
#include <weta_platform.h>
#ifndef __HW_FLASH_RAW_H__
#define __HW_FLASH_RAW_H__

typedef enum {
    FLASH_RAW_RESULT_OK,
    FLASH_RAW_RESULT_ERR,
    FLASH_RAW_RESULT_TIMEOUT
} FlashRawResult;

#ifdef __cplusplus
extern "C" {
#endif

extern FlashRawResult hw_flash_raw_write(WetaFlashPtr dest, const void *src, size_t size);
extern FlashRawResult hw_flash_raw_erase_sector(size_t sector);
extern FlashRawResult hw_flash_raw_read(WetaFlashPtr src, void *dest, size_t size);

#ifdef __cplusplus
}
#endif

#endif //__HW_FLASH_RAW_H__
