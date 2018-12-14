#include "display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main(){
    display *d = newDisplay("Test", 500, 500);
    line(d, 1, 1, 250, 250);
    line(d, 500, 0, 250, 250);
    pause(d, 5000);
    clear(d);
    end(d);
    return 0;
}
