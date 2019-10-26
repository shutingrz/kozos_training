#ifndef _SERIAL_H_INCLUDED
#define _SERIAL_H_INCLUDED

int serial_init(int index); //デバイス初期化
int serial_is_send_enable(int index); //送信可能か
int serial_send_byte(int index, unsigned char b); //1文字送信

#endif

