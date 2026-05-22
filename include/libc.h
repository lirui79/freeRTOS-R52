#ifndef _LIBC_H_
#define _LIBC_H_



/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

unsigned int strlen(const char *str);
char *strcpy(char *to, const char *from);
int strncmp(const char *s1, const char *s2, unsigned int n);
void*  memset(void*  dst, int c, unsigned int n);
void *memcpy(void *dest, const void *src, unsigned int n);
int strcmp(const char *s1, const char *s2);
int memcmp(const void* s1, const void* s2, unsigned int n);
char *strstr(const char *s1, const char *s2);
void *memmem (const void *haystack_start, unsigned int haystack_len,
          const void *needle_start, unsigned int needle_len);


/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif