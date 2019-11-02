#include "defines.h"
#include "serial.h"
#include "lib.h"

/**
 * @brief memset
 * @param b 書き込み先アドレス
 * @param c 書き込むデータ
 * @param len 長さ 
**/
void *memset(void *b, int c, long len){
    char *p;
    for (p = b; len > 0; len--)
        *(p++) = c;
    return b;
}

/**
 * @brief memcpy
 * @param dst コピー先アドレス
 * @param src コピー元アドレス
 * @param len 長さ
**/
void *memcpy(void *dst, const void *src, long len){
    char *d = dst;
    const char *s = src;
    for (; len > 0; len--)
        *(d++) = *(s++);
    return dst;
}

/**
 * @brief メモリの内容を比較
 * @param b1 比較元
 * @param b2 比較先
 * @param len 比較する長さ
 * @return 最後に一致した長さ. 0は一致しなかったとき
**/
int memcmp(const void *b1, const void *b2, long len){
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

