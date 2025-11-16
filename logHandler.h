#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include "bankServices.h"

#define MAXBUFFERSIZE   256

long generateTxnId();
void writeCreateToWal(accountDetails_t* accDetails, long txnId);
void writeUpdateToWal(accountDetails_t* oldAccountDetails, accountDetails_t* newAccountDetails, long txnId);
void writeDeleteToWal(accountDetails_t* oldAccountDetails, long txnId);
void writeTransferToWal(accountDetails_t* oldFromAccountDetails, 
    accountDetails_t* fromAccDetails, accountDetails_t* oldToAccountDetails, 
    accountDetails_t* toAccountDetails, long txnId);
void writeAddToOriginalFile(accountDetails_t* accDetails);
void writeUpdateToOriginalFile(accountDetails_t* newAccountDetails);
void writeDeleteToOriginalFile(accountDetails_t* accDetails);
void writeTransferToOriginalFile(accountDetails_t* fromAccDetails, accountDetails_t* toAccDetails);
void writeCommitToWAL(long txnId);
int validateAccountDetails(accountDetails_t* accDetails);
int validateReceiverAccount(accountDetails_t* toAccountDetails, long userEnteredAccNo);
void printAccountDetails(accountDetails_t* accDetails);
void updateDetails(accountDetails_t* oldAccDetails);
void deposit(accountDetails_t* oldAccDetails);
void withDraw(accountDetails_t* oldAccDetails);
void transfer(accountDetails_t* fromAccDetails);

#endif