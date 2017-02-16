#ifndef ESPMISSINGINCLUDES_H
#define ESPMISSINGINCLUDES_H

#include <stdint.h>
#include <c_types.h>
#include <os_type.h>


int strcasecmp(const char *a, const char *b);
#ifndef FREERTOS
#include <eagle_soc.h>
#include <ets_sys.h>
//Missing function prototypes in include folders. Gcc will warn on these if we don't define 'em anywhere.
//MOST OF THESE ARE GUESSED! but they seem to swork and shut up the compiler.
typedef struct espconn espconn;

extern int atoi(const char *nptr);
extern void ets_install_putc1(void *routine);
extern void ets_isr_attach(int intr, void *handler, void *arg);
extern void ets_isr_mask(unsigned intr);
extern void ets_isr_unmask(unsigned intr);
extern int ets_memcmp(const void *s1, const void *s2, size_t n);
extern void *ets_memcpy(void *dest, const void *src, size_t n);
extern void *ets_memset(void *s, int c, size_t n);
extern int ets_sprintf(char *str, const char *format, ...)  __attribute__ ((format (printf, 2, 3)));
extern int ets_str2macaddr(void *, void *);
extern int ets_strcmp(const char *s1, const char *s2);
extern char *ets_strcpy(char *dest, const char *src);
extern size_t ets_strlen(const char *s);
extern int ets_strncmp(const char *s1, const char *s2, int len);
extern char *ets_strncpy(char *dest, const char *src, size_t n);
extern char *ets_strstr(const char *haystack, const char *needle);
extern void ets_timer_arm_new(os_timer_t *a, int b, int c, int isMstimer);
extern void ets_timer_disarm(os_timer_t *a);
extern void ets_timer_setfn(os_timer_t *t, ETSTimerFunc *fn, void *parg);
extern void ets_update_cpu_frequency(int freqmhz);
extern void *os_memmove(void *dest, const void *src, size_t n);
extern int os_printf(const char *format, ...)  __attribute__ ((format (printf, 1, 2)));
extern int os_snprintf(char *str, size_t size, const char *format, ...) __attribute__ ((format (printf, 3, 4)));
extern int os_printf_plus(const char *format, ...)  __attribute__ ((format (printf, 1, 2)));
extern void uart_div_modify(int no, unsigned int freq);
extern uint8 wifi_get_opmode(void);
extern uint32 system_get_time();
extern int rand(void);
extern void ets_bzero(void *s, size_t n);
extern void ets_delay_us(int ms);

void *pvPortMalloc(size_t xWantedSize, const char *file, int line);
void *pvPortZalloc(size_t, const char *file, int line);
void vPortFree(void *ptr, const char *file, int line);
void *vPortMalloc(size_t xWantedSize, const char *file, int line);
void pvPortFree(void *ptr, const char *file, int line);

//Standard PIN_FUNC_SELECT gives a warning. Replace by a non-warning one.
#ifdef PIN_FUNC_SELECT
#undef PIN_FUNC_SELECT
#define PIN_FUNC_SELECT(PIN_NAME, FUNC)  do { \
    WRITE_PERI_REG(PIN_NAME,   \
                                (READ_PERI_REG(PIN_NAME) \
                                     &  (~(PERIPHS_IO_MUX_FUNC<<PERIPHS_IO_MUX_FUNC_S)))  \
                                     |( (((FUNC&BIT2)<<2)|(FUNC&0x3))<<PERIPHS_IO_MUX_FUNC_S) );  \
    } while (0)
#endif //PIN_FUNC_SELECT

#endif //FREERTOS

#endif //ESPMISSINGINCLUDES_H
