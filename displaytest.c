#include "display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main(){
    display *d = newDisplay("Test", 50, 50);
    line(d, 0, 0, 25, 25);
    return 0;
}
