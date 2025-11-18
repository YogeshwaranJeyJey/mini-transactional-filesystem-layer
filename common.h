#ifndef MAIN_H
#define MAIN_H

#define MAXCHOICECHAR   5
#define WIDTH   50
#define MAXBUFFERSIZE   256
#define CHECKSUMFILEPATH    "checkSum.txt"
#define WALFILEPATH "wal.txt"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int doPadding(char* string);
void trimNewLine(char* line);

#endif