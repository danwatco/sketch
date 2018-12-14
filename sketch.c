#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "display.h"
#include <stdbool.h>

enum OPCODE {DX, DY, FF, PEN};

struct state {
    display *d;
    int xpos;
    int ypos;
    int dx;
    bool pen;
};
typedef struct state state;

int getOpcode(unsigned char byte){
    return (byte & 0xC0) >> 6;
}

int getOperand(unsigned char byte){
    return (signed char) ((byte & 0x3F) << 2 )>> 2;
}

state *initalise(char *path){
    display *d = newDisplay(path, 300, 300);
    state *s = malloc(sizeof(state));
    *s = (state) {d, 0, 0, 0, false};
    /*FILE *in = fopen(path, "r");
    unsigned char byte = fgetc(in);
    int op = getOpcode(byte);
    int opnd = getOperand(byte);
    printf("Opcode is %d and operand is %d\n", op, opnd);
    finish(s);*/
    return s;
}

void finish(state *s){
    end(s->d);
    free(s);
}

void run(char *path){
    state *s = initalise(path);


    finish(s);
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

void test(){
    testOp();
    testOper();
    run("lines.sketch");
    printf("Tests pass\n");
}

int main(int n, char *args[n]){
    if(n==1) test();
    else {

    }
    
    return 0;
}
