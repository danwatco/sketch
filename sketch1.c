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
    return s;
}

void finish(state *s){
    end(s->d);
    free(s);
}

void handleX(state *s, unsigned char byte){
    int value = getOperand(byte);
    s->dx = value;
}

void handleY(state *s, unsigned char byte){
    int value = getOperand(byte);
    if(s->pen){
        display *d = s->d;
        line(d, s->xpos, s->ypos, s->xpos + s->dx, s->ypos + value);
        s->xpos = s->xpos + s->dx;
        s->dx = 0;
        s->ypos = s->ypos + value;
    } else {
        s->xpos = s->xpos + s->dx;
        s->dx = 0;
        s->ypos = s->ypos + value;
    }
}

void update(state *s, FILE *in){
    unsigned char byte = fgetc(in);
    switch(getOpcode(byte)){
        case DX:
            handleX(s, byte);
            break;
        case DY:
            handleY(s, byte);
            break;
        case PEN:
            printf("Pen\n");
            s->pen = ! (s->pen);
            break;
    }
}

void run(char *path){
    state *s = initalise(path);
    FILE *in = fopen(path, "r");
    update(s, in);
    while(!feof(in)){
        update(s, in);
    }
    fclose(in);
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
    printf("Tests pass\n");
}

int main(int n, char *args[n]){
    if(n==1) test();
    else if (n == 2) {
        run(args[1]);
    } else {
        printf("Use ./sketch path/to/file \n");
    }
    
    return 0;
}
