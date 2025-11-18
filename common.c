#include "common.h"

int doPadding(char* string){
    return (WIDTH - strlen(string)) / 2;
}

void trimNewLine(char* line){
    line[strcspn(line, "\n")] = '\0';
}