#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include "bankServices.h"

int checkValidPassword(char* passwd, char* reTypedPasswd);
long generateNewAccNumber();
int validateName(char* name);
int validateMobileNumber(char *mobile);
int validateMail(char *mail);

int getNewAccountDetails();
long generateTxnId();

void writeCreateToWal(accountDetails_t* accDetails, long txnId, int recoveryFlag);
void writeUpdateToWal(accountDetails_t* oldAccountDetails, accountDetails_t* newAccountDetails, long txnId, int recoveryFlag);
void writeDeleteToWal(accountDetails_t* oldAccountDetails, long txnId);
void writeTransferToWal(accountDetails_t* oldFromAccountDetails, 
    accountDetails_t* fromAccDetails, accountDetails_t* oldToAccountDetails, 
    accountDetails_t* toAccountDetails, long txnId, int recoveryFlag);

void writeAddToOriginalFile(accountDetails_t* accDetails);
void writeUpdateToOriginalFile(accountDetails_t* newAccountDetails);
void writeDeleteToOriginalFile(accountDetails_t* accDetails);
void writeTransferToOriginalFile(accountDetails_t* fromAccDetails, accountDetails_t* toAccDetails);

void writeCommitToWAL(long txnId);

int validateAccountDetails(accountDetails_t* accDetails);
int validateReceiverAccount(accountDetails_t* toAccountDetails, long userEnteredAccNo);
void printAccountDetails(accountDetails_t* accDetails);

int updateDetails(accountDetails_t* oldAccDetails);
int deposit(accountDetails_t* oldAccDetails);
int withDraw(accountDetails_t* oldAccDetails);
int transfer(accountDetails_t* fromAccDetails);

#endif