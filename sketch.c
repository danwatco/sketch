#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "display.h"

enum OPCODE {DX, DY, FF, PEN};

int getOpcode(unsigned char byte){
    return (byte & 0xC0) >> 6;
}

int getOperand(unsigned char byte){
    return (signed char) ((byte & 0x3F) << 2 )>> 2;
}

void testOp(){
    unsigned char test = 0x7D;
    assert(getOpcode(test) == 1);
    test = 0xC0;
    assert(getOpcode(test) == 3);
    test = 0x03;
    assert(getOpcode(test) == 0);
}

void testOper(){
    unsigned char test = 0x7D;
    assert(getOperand(test) == -3);
    test = 0x03;
    assert(getOperand(test) == 3);
}

int main(){
    testOp();
    testOper();
    printf("Tests pass\n");
    return 0;
}
