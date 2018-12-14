#include "display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main(){
    display *d = newDisplay("Test", 500, 500);
    line(d, 0, 0, 250, 250);
    end(d);
    return 0;
}
