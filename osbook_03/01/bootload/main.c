#include "defines.h"
#include "serial.h"
#include "lib.h"

int main(void){
    serial_init(SERIAL_DEFAULT_DEVICE);

    puts("Hello, world!\n");

    while(1){
        // no action
    }

    return 0;
}