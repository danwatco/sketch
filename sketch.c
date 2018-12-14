#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "display.h"
#include <stdbool.h>

enum OPCODE {DX, DY, PR, DO};
enum DO {PEN, DT, CLEAR, KEY, COL};

struct state {
    display *d;
    int xpos;
    int ypos;
    int dx;
    bool pen;
    int operandval;
    bool opinit;
    int dt;
};
typedef struct state state;

// Using right shift to extract the opcode
int getOpcode(unsigned char byte){
    return (byte & 0xC0) >> 6;
}

// Using masking and sign extension
int getOperand(unsigned char byte){
    int value = byte & 0x3F;
    if((value & 0x20) != 0 ) value = (~0U << 6) | value;
    return value;
}

// Create a new drawing state
state *initalise(char *path){
    display *d = newDisplay(path, 300, 300);
    state *s = malloc(sizeof(state));
    *s = (state) {d, 0, 0, 0, false, 0, false};
    return s;
}

// End display and free state
void finish(state *s){
    end(s->d);
    free(s);
}

void handleX(state *s, unsigned char byte){ 
    if(s->opinit){
        // Puts current operand under construction into dx 
        // plus new operand
        int value = s->operandval;
        s->dx = ((unsigned)(value) << 6) | (byte & 0x3F);
        s->operandval = 0;
        s->opinit = false;
    } else {
        // Takes operand and put into dx
        int value = getOperand(byte);
        s->dx = value;
    }
    
}

// Function to update current state with new posisitons, 
// used to reduce repitition
void updatePos(state *s, int yval){
    s->xpos = s->xpos + s->dx;
    s->dx = 0;
    s->ypos = s->ypos + yval;
}

void handleY(state *s, unsigned char byte){
    if(s->opinit){
        // Uses current operand under contsruction plus new operand to 
        // create y value. Draws line if pen is down.
        int value = ((unsigned)(s->operandval) << 6) | (byte & 0x3F);
        if(s->pen){
            display *d = s->d;
            line(d, s->xpos, s->ypos, s->xpos + s->dx, s->ypos + value);
        }
        updatePos(s, value);
        s->operandval = 0;
        s->opinit = false;
    } else {
        // Takes operand and draws if pen is down.
        int value = getOperand(byte);
        if(s->pen){
            display *d = s->d;
            line(d, s->xpos, s->ypos, s->xpos + s->dx, s->ypos + value);
            updatePos(s, value);
        } else {
            updatePos(s, value);
        }
    }
    
}

// Handles prefix operations using a initalizer variable
void handlePR(state *s, unsigned char byte){
    if(!s->opinit){
        int value = 0;
        s->opinit = true;
        value = getOperand(byte);
        s->operandval = value;
    } else {
        unsigned int value = byte & 0x3F;
        s->operandval = (s->operandval << 6) | value;
    }
}

// Handles pausing using prefixes
void handleDT(state *s, unsigned char byte){
    display *d = s->d;
    if(s->opinit){
        int value = s->operandval;
        if(value == 0) pause(d, s->dt);
        else {
            pause(d, value);
            s->dt = value;
        }
        s->operandval = 0;
        s->opinit = false;
    } else {
        pause(d, s->dt);
    }
}

// Handles pen colour change using prefixes
void handleCOL(state *s, unsigned char byte){
    if(s->opinit){
        int value = s->operandval;
        colour(s->d, value);
        s->operandval = 0;
        s->opinit = false;
    }
}

// Handles opcode 3 (DO) switches over enum for readability
void handleDO(state *s, unsigned char byte){
    switch(getOperand(byte)){
        case PEN:
            s->pen = ! (s->pen);
            break;
        case DT:
            handleDT(s, byte);
            break;
        case CLEAR:
            clear(s->d);
            break;
        case KEY:
            key(s->d);
            break;
        case COL:
            handleCOL(s, byte);
            break;
    }
}

// Updates the state with a new byte from a file
// Switch statement used over the op codes for readability
void update(state *s, FILE *in){
    unsigned char byte = fgetc(in);
    switch(getOpcode(byte)){
        case DX:
            handleX(s, byte);
            break;
        case DY:
            handleY(s, byte);
            break;
        case PR:
            handlePR(s, byte);
            break;
        case DO:
            handleDO(s, byte);
            break;
    }
}

// Creates new state and opens and closes file when finsihed.
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
