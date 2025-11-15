#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include "bankServices.h"

long generateTxnId();
void writeToWal(accountDetails_t* accDetails, long txnId, char* operation);
void writeToOriginalFile(accountDetails_t* accDetails);

#endif