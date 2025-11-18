#ifndef CRASHRECOVERY_H
#define CRASHRECOVERY_H

#define OPERATIONCHARSLEN   10

#include "bankServices.h"

typedef struct {
    long txnId;
    char operation[OPERATIONCHARSLEN];
    int endTxnFlag;
    int commitedFlag;
    int hasOld;
    int hasNew;
    accountDetails_t oldAccDetails;
    accountDetails_t newAccDetails;
    int hasOldFrom, hasOldTo;
    int hasNewFrom, hasNewTo;
    accountDetails_t oldFromAccDetails;
    accountDetails_t oldToAccDetails;
    accountDetails_t newFromAccDetails;
    accountDetails_t newToAccDetails;
}wal_Txn_t;

long getLastTxnId();
int readWalTransaction(FILE* walFp, wal_Txn_t* walTxn);
int accountExists(long accNo);
int undo(wal_Txn_t *walTxn);
int redo(wal_Txn_t* walTxn);
int updateCheckPoint(long lastTxnId);
void startRecovery();

#endif