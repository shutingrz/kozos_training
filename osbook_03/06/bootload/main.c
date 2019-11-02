#include "defines.h"
#include "serial.h"
#include "xmodem.h"
#include "elf.h"
#include "lib.h"

static int init(void){
    // 以下はリンカスクリプトで定義してあるシンボル
    extern int erodata, data_start, edata, bss_start, ebss;

    /**
     * データ領域とBSS領域を初期化する。この処理以降でないとグローバル変数が初期化されていないので注意
    **/
    memcpy(&data_start, &erodata, (long)&edata - (long)&data_start);
    memset(&bss_start, 0, (long)&ebss - (long)&bss_start);

    //シリアルの初期化
    serial_init(SERIAL_DEFAULT_DEVICE);

    return 0;
}

/**
 * メモリの16進ダンプ出力
**/
static int dump(char *buf, long size){
    long i;

    if (size < 0){
        puts("no data.\n");
        return -1;
    }
    for (i = 0; i < size; i++){
        putxval(buf[i], 2);
        if ((i & 0xf) == 15){ //16バイトごとに改行
            puts("\n");
        } else {
            if ((i & 0xf) == 7) puts(" "); //8バイトごとに空白
            puts(" ");
        }
    }
    puts("\n");

    return 0;
}

/**
 * ウェイト用のサービス関数
 * 別プロセッサの場合は再考の必要あり
**/
static void wait(){
    volatile long i;
    for (i = 0; i < 300000; i++)
        ;
}

int main(void){

    static char buf[16];
    static long size = -1;
    static unsigned char *loadbuf = NULL;
    char *entry_point;
    void (*f)(void);
    extern int buffer_start; //リンカスクリプトで定義されているバッファ

    init();

    puts("kzload (kozos boot loader) started.\n");

    while (1){
        puts("kzload> "); //プロンプト
        gets(buf); //シリアルからのコマンド受信
        
        if(!strcmp(buf, "load")){ //XMODEMでのファイルのダウンロード
            loadbuf = (char *)(&buffer_start);
            size = xmodem_recv(loadbuf);
            wait(); //転送アプリが終了し端末アプリに制御が戻るまで待ち合わせる
            if (size < 0) {
                puts("\nXMODEM receive error!\n");
            } else {
                puts("\nXMODEM receive succeeded.\n");
            }
        } else if (!strcmp(buf, "dump")){ //メモリの16進ダンプ出力
            puts("size: ");
            putxval(size, 0);
            puts("\n");
            dump(loadbuf, size);
        } else if (!strcmp(buf, "run")) { //ELF形式ファイルの実行
            entry_point = elf_load(loadbuf); //メモリ上に展開(ロード)
            if (!entry_point){
                puts("run error!\n");
            } else {
                puts("starting from entry point: ");
                putxval((unsigned long)entry_point, 0);
                puts("\n");
                f = (void (*)(void))entry_point;
                f(); //ロードしたプログラムに処理を渡す
                
                /* ここには返ってこないはず */
            }
        } else {
            puts("unknown.\n");
        }
    }

    return 0;
}
