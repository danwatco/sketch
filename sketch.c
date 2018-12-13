#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

enum OPCODE {DX, DY, FF, PEN};

int getOpcode(uint8_t byte){
    return (byte & 0xC0) >> 6;
}

void testOp(){
    unsigned char test = 0x7D;
    assert(getOpcode(test) == 1);
    test = 0xC0;
    assert(getOpcode(test) == 3);
    test = 0x03;
    assert(getOpcode(test) == 0);
}

int main(){
    testOp();
    printf("Tests pass\n");
    return 0;
}
