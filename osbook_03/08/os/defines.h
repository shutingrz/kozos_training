#ifndef _DEFINES_H_INCLUDED_
#define _DEFINES_H_INCLUDED_

#define NULL ((void *)0) // nullポインタの定義
#define SERIAL_DEFAULT_DEVICE 1 //標準のシリアルデバイス

/**
 * ビット幅固定の整数型
**/
typedef unsigned char uint8;
typedef unsigned char uint16;
typedef unsigned char uint32;

typedef uint32 kz_thread_id_t;
typedef int (*kz_func_t)(int argc, char *argv[]);
typedef void (*kz_handler_t)(void);

#endif

