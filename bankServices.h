#ifndef BANKSERVICES_H
#define BANKSERVICES_H

#define MAXNAMECHAR 20
#define MAXMOBILECHAR   13
#define MAXMAILCHAR 30
#define MAXPASSWDCHAR   9
#define ADDACCOUNTHEADER    "Create New Account"

typedef struct accountDetails{
    long accountNumber;
    char name[MAXNAMECHAR];
    float accountBalance;
    char mobileNumber[MAXMOBILECHAR];
    char mailId[MAXMAILCHAR];
    char password[MAXPASSWDCHAR];
}accountDetails_t;

int checkValidPassword(char* passwd, char* reTypedPasswd);
long generateNewAccNumber();
void addAccount();

#endif