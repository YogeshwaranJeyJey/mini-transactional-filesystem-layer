#include "common.h"
#include "bankServices.h"
#include "logHandler.h"

accountDetails_t accDetails;

int checkValidPassword(char* passwd, char* reTypedPasswd){
    if(strcmp(passwd, reTypedPasswd) == 0) return 1;
    return 0;
}

long generateNewAccNumber(){
    long lastAccNumber = 100;
    FILE* fp = fopen("accountNumberCounter.txt", "r");
    if(!fp){
        fp = fopen("accountNumberCounter.txt", "w");
        if(!fp){
            printf("Error creating accountNumberCounter.txt file!\n");
            return -1;
        }
        lastAccNumber = 101;
        fprintf(fp, "%ld", lastAccNumber);
        fclose(fp);
        return lastAccNumber;
    }
    fscanf(fp, "%ld", &lastAccNumber);
    fclose(fp);
    long newAccNumber = lastAccNumber + 1;

    fp = fopen("accountNumberCounter.txt", "w");
    if(!fp){
        printf("Error occurred while writing the newAccNumber!\n");
        return -1;
    }
    fprintf(fp, "%ld", newAccNumber);
    fclose(fp);
    return newAccNumber;
}

void addAccount(){
    char passwd[MAXPASSWDCHAR];
    char reTypedPasswd[MAXPASSWDCHAR];
    int passwdRepeatFlag = 1;
    int padding = doPadding(ADDACCOUNTHEADER);
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",ADDACCOUNTHEADER, padding, "");

    printf("Enter Name: ");
    fgets(accDetails.name, sizeof(accDetails.name), stdin);
    accDetails.name[strcspn(accDetails.name, "\n")] = '\0';

    printf("Enter MobileNumber: ");
    fgets(accDetails.mobileNumber, sizeof(accDetails.mobileNumber), stdin);
    accDetails.mobileNumber[strcspn(accDetails.mobileNumber, "\n")] = '\0';

    printf("Enter Mail Id: ");
    fgets(accDetails.mailId, sizeof(accDetails.mailId), stdin);
    accDetails.mailId[strcspn(accDetails.mailId, "\n")] = '\0';

    while(passwdRepeatFlag){
        printf("Set Password\n");
        printf("Enter New Password (Max 6 characters): ");
        fgets(passwd, sizeof(passwd), stdin);
        passwd[strcspn(passwd, "\n")] = '\0';

        printf("Confirm Password: ");
        fgets(reTypedPasswd, sizeof(reTypedPasswd), stdin);
        reTypedPasswd[strcspn(reTypedPasswd, "\n")] = '\0';
        
        if(checkValidPassword(passwd, reTypedPasswd)){
            strcpy(accDetails.password, passwd);
            printf("Password Set Successfully!\n");
            break;
        }
        else{
            printf("Error: Passwords do not match!\n\n");
        }
    }
    
    accDetails.accountNumber = generateNewAccNumber();
    accDetails.accountBalance = 0;

    long txnId = generateTxnId();
    if(txnId == -1){
        return;
    }
    writeCreateToWal(&accDetails, txnId);
    writeAddToOriginalFile(&accDetails);
    writeCommitToWAL(txnId);
}

void viewAccount(){
    if(validateAccountDetails(&accDetails)){
        printAccountDetails(&accDetails);
    }
    else
        return;
}

void updateAccountDetails(){
    if(validateAccountDetails(&accDetails)){
        updateDetails(&accDetails);
    }
    else
        return;
}

void deleteAccount(){
    if(validateAccountDetails(&accDetails)){
        long txnId = generateTxnId();
        writeDeleteToWal(&accDetails, txnId);
        writeDeleteToOriginalFile(&accDetails);
        writeCommitToWAL(txnId);
    }
    else
        return;
}

void depositCash(){
    if(validateAccountDetails(&accDetails)){
        deposit(&accDetails);
    }
    else
        return;
}

void withDrawCash(){
    if(validateAccountDetails(&accDetails)){
        withDraw(&accDetails);
    }
    else
        return;
}

void transferCash(){
    if(validateAccountDetails(&accDetails)){
        transfer(&accDetails);
    }
    else
        return;
}