#ifndef BANKSERVICES_H
#define BANKSERVICES_H

#define MAXNAMECHAR 20
#define MAXMOBILECHAR   13
#define MAXMAILCHAR 30
#define MAXPASSWDCHAR   9
#define ADDACCOUNTHEADER    "Create New Account"
#define VIEWACCOUNTHEADER    "View Account Details"
#define UPDATEACCOUNTHEADER    "Update Account Details"
#define DELETEACCOUNTHEADER    "Delete Account Details"
#define DEPOSITHEADER   "Deposit Cash"
#define WITHDRAWHEADER   "Withdraw Cash"
#define TRANSFERHEADER   "Transfer Cash"
#define ACCOUNTNUMBERCOUNTERTXTPATH "accountNumberCounter.txt"

typedef struct accountDetails{
    long accountNumber;
    char name[MAXNAMECHAR];
    float accountBalance;
    char mobileNumber[MAXMOBILECHAR];
    char mailId[MAXMAILCHAR];
    char password[MAXPASSWDCHAR];
}accountDetails_t;

void addAccount();
void viewAccount();
void updateAccountDetails();
void deleteAccount();
void depositCash();
void withDrawCash();
void transferCash();

#endif