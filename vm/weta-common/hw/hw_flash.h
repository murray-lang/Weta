#ifndef __HW_FLASH_H__
#define __HW_FLASH_H__

#include <weta_platform.h>
#include <stdbool.h>

#define INVALID_FLASH_PTR ((WetaFlashPtr)~0)

typedef struct _FLASH *PFLASH;

extern bool				hw_flash_init(
							uint8_t*     base, 
							WetaFlashPtr length, 
							uint16_t     flags
						);
extern PFLASH			hw_flash_open(uint8_t* base);
extern bool				hw_flash_close(PFLASH flash);
extern WetaFlashPtr 	hw_flash_free_bytes(PFLASH flash);
extern bool				hw_flash_start_write(
							PFLASH       flash, 
							WetaFlashPtr startAddress, 
							WetaFlashPtr requiredBytes
						);
extern bool				hw_flash_flush(PFLASH flash);
extern bool				hw_flash_start_read(PFLASH flash, WetaFlashPtr startAddress);
extern WetaFlashPtr		hw_flash_write_byte(PFLASH flash, uint8_t val);
extern WetaFlashPtr		hw_flash_write_bytes(
							PFLASH       flash, 
							uint8_t*     vals, 
							WetaFlashPtr count
						);
extern WetaFlashPtr		hw_flash_read_byte(
							PFLASH       flash, 
							WetaFlashPtr loc, 
							uint8_t*     pval);
extern WetaFlashPtr		hw_flash_read_bytes(
							PFLASH       flash, 
							WetaFlashPtr loc, 
							uint8_t*     vals, 
							WetaFlashPtr count);

extern bool				hw_flash_erase(PFLASH flash);

extern WetaFlashPtr hw_flash_total_size(PFLASH flash);


#endif // __HW_FLASH_H__
