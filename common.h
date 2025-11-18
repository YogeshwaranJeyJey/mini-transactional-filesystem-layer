#ifndef MAIN_H
#define MAIN_H

#define MAXCHOICECHAR   5
#define WIDTH   50
#define MAXBUFFERSIZE   256
#define LAST_TXN_COUNTERID    1000
#define TXN_COUNTER_TXTFILE_PATH   "txnCounter.txt"
#define CHECKPOINT_FILE_PATH    "checkPoint.txt"
#define WAL_TXTFILE_PATH  "wal.txt"
#define NEW_WAL_TXTFILE_PATH    "newWal.txt"
#define ACCOUNT_DETAILS_TXTFILE_PATH   "accountDetails.txt"
#define ABOUT_PROJECT_FILE_PATH "aboutProject.txt"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int doPadding(char* string);
void trimNewLine(char* line);
int isBlank(const char *s);
void aboutProject();

#endif