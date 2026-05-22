#include "libc.h"

#include <stdarg.h>

#ifndef  NULL
#define NULL            0
#endif

/** \brief Division result
 *  \sa uidiv
 */
typedef struct uidiv_result {
    unsigned int quo;  ///< Quotient
    unsigned int rem;  ///< Remainder
} uidiv_result_t;

/********************************************//**
 *  \brief Unsigned integer division
 *
 *  ARM does not have native division support
 *  \returns Result of operation or zero if
 *  dividing by zero.
 ***********************************************/
uidiv_result_t
uidiv (unsigned int num,   ///< Numerator
       unsigned int dem)   ///< Denominator
{
    unsigned int tmp = dem;
    uidiv_result_t ans = {0};

    if (dem == 0)
    {
        // TODO: Somehow make error
        return ans;
    }

    while (tmp <= num >> 1)
    {
        tmp <<= 1;
    }

    do
    {
        if (num >= tmp)
        {
            num -= tmp;
            ans.quo++;
        }
        ans.quo <<= 1;
        tmp >>= 1;
    } while (tmp >= dem);
    ans.quo >>= 1;
    ans.rem = num;

    return ans;
}

void*  memset(void*  dst, int c, unsigned int n)
{
    char*  q   = dst;
    char*  end = q + n;

    for (;;) {
        if (q >= end) break;
        *q++ = (char) c;
        if (q >= end) break;
        *q++ = (char) c;
        if (q >= end) break;
        *q++ = (char) c;
        if (q >= end) break;
        *q++ = (char) c;
    }

  return dst;
}

unsigned int
strlen(const char *str)
{
    const char *s;

    for (s = str; *s; ++s)
        ;
    return (unsigned int) (s - str);
}

char *
strcpy(char *to, const char *from)
{
    char *save = to;

    for (; (*to = *from) != '\0'; ++from, ++to);
    return(save);
}

/*
 * Compare strings.
 */
int
strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return (0);
    return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

int
strncmp(const char *s1, const char *s2, unsigned int n)
{
    if (n == 0)
        return (0);
    do {
        if (*s1 != *s2++)
            return (*(unsigned char *)s1 - *(unsigned char *)--s2);
        if (*s1++ == 0)
            break;
    } while (--n != 0);
    return (0);
}

void *memcpy(void *dest, const void *src, unsigned int n)
{
    char *dp = dest;
    const char *sp = src;
    while (n--)
        *dp++ = *sp++;
    return dest;
}

int memcmp(const void* s1, const void* s2, unsigned int n)
{
    const unsigned char *p1 = s1, *p2 = s2;
    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

char *strstr(const char *s1, const char *s2)
{
    unsigned int n = strlen(s2);
    while(*s1)
        if(!memcmp(s1++,s2,n))
            return (char *)s1-1;
    return 0;
}

void *memmem(const void *haystack_start, unsigned int haystack_len,
          const void *needle_start, unsigned int needle_len)
{
    const unsigned char *haystack = NULL;
    if (needle_len == 0)
        return (void *) haystack_start;
    if (haystack_len < needle_len)
        return NULL;
    for (unsigned int i = 0; i < haystack_len - needle_len; i++) {
        if (((unsigned char *)haystack_start + i)[0] == ((unsigned char *)needle_start)[0]) {
            haystack = (const unsigned char *)haystack_start + i;
            haystack_len -= i;
            break;
        }
    }
    if (!haystack || haystack_len < needle_len)
        return NULL;
    for (unsigned int i = 0; i < haystack_len - needle_len; i++) {
        if (!memcmp(haystack + i, needle_start, needle_len))
            return (void *) (haystack + i);
    }
    return NULL;
}
