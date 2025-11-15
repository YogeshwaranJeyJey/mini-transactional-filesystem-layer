#include "common.h"

int doPadding(char* string){
    return (WIDTH - strlen(string)) / 2;
}