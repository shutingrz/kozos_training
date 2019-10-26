#include "defines.h"
#include "serial.h"
#include "lib.h"

void *memset(void *b, int c, long len){
    char *p;
    for (p = b; len > 0; len--)
        *(p++) = c;
    return b;
}

void *memcpy(void *dst, const void *src, long len){
    char *d = dst;
    const char *s = src;
    for (; len > 0; len--)
        *(d++) = *(s++);
    return dst;
}

int memcmp(const void *bl, const void *b2, long len){
    const char *p1 = b1, *p2 = b2;
    for (; len > 0; len--){
        if (*p1 != *p2)
            return (*p1 > *p2) ? 1: -1;
        p1++;
        p2++;
    }
    return 0;
}

/**
 * sに\00が含まれないアドレスを渡すと無限ループ(最悪どこかの\00で止まるが)
**/
int strlen(const char *s){
    int len;
    for (len = 0; *s; s++, len++)   //*s は \00 になると NULLなのでforを抜けられる
        ;
    return len;
}

char *strcpy(char *dst, const char *src){
    char *d = dst;
    for (;; dst++, src++){
        *dst = *src;
        if (!*src) break;   //コピーしたあとに比較しないと \00がコピーされない
    }
    return d;
}

int strcmp(const char *s1, const char *s2){
    while (*s1 || *s2) {
        if (*s1 != *s2)
            return (*s1 > *s2) ? 1 : -1;
        s1++;
        s2++;
    }
    return 0;
}

int strncmp(const char *s1, const char *s2, int len){
    while ((*s1 || *s2) && (len > 0)) {
        if (*s1 != *s2)
            return (*s1 > *s2) ? 1 : -1;
        s1++;
        s2++;
    }
    return 0;
}


int putc(unsigned char c){
    if (c == '\n')
        serial_send_byte(SERIAL_DEFAULT_DEVICE, '\r');
    
    return serial_send_byte(SERIAL_DEFAULT_DEVICE, c);
}

int puts(unsigned char *str){
    while (*str)
        putc(*(str++));

    return 0;
}

/**
 * 数値の16進表示
**/
int putxval(unsigned long value, int column){
    char buf[9];
    char *p;

    p = buf + sizeof(buf) - 1;
    *(p--) = '\0';

    if (!value && !column)
        column++;
    
    while (value || column){
        *(p--) = "0123456789abcdef"[value & 0xf];   //16進文字に変換してバッファに格納する
        value >>= 4;    //次の桁に進める
        if (column) column--;   //桁数指定がある場合にはカウントする
    }

    puts(p + 1);

    return 0;
}