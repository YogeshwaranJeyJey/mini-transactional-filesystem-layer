//generateTxnId()
    //writeToWal();
    //writeToJournal();

#include "common.h"
#include "logHandler.h"
#include "bankServices.h"


long generateTxnId(){
    long lastCounterId = 1000;
    FILE* fp = fopen("txnCounter.txt", "r");
    if(!fp){
        fp = fopen("txnCounter.txt", "w");
        if(!fp){
            printf("Error creating txnCounter.txt file!\n");
            return -1;
        }
        lastCounterId = 1001;
        fprintf(fp, "%ld", lastCounterId);
        fclose(fp);
        return lastCounterId;
    }
    fscanf(fp, "%ld", &lastCounterId);
    fclose(fp);
    long newCounterId = lastCounterId + 1;

    fp = fopen("txnCounter.txt", "w");
    if(!fp){
        printf("Error occurred while writing the newCounterId!\n");
        return -1;
    }
    fprintf(fp, "%ld", newCounterId);
    fclose(fp);
    return newCounterId;
}

void writeToWal(accountDetails_t* accDetails, long txnId, char* operation){
    FILE* fp = fopen("wal.txt", "a");
    if (!fp) {
        printf("Error opening wal.txt log file!\n");
        return;
    }

    fprintf(fp, "TXN ID: %ld\n", txnId);
    fprintf(fp, "Account Number: %ld\n", accDetails->accountNumber);
    fprintf(fp, "Name: %s\n", accDetails->name);
    fprintf(fp, "Account Balance: %f\n", accDetails->accountBalance);
    fprintf(fp, "MobileNumber: %s\n", accDetails->mobileNumber);
    fprintf(fp, "MailId: %s\n", accDetails->mailId);
    fprintf(fp, "Password: %s\n", accDetails->password);

    fprintf(fp, "Commit status: Pending\n");
    fprintf(fp, "End Txn\n\n");

    fflush(fp);
    fclose(fp);
}

void writeToOriginalFile(accountDetails_t* accDetails){
    int fileExists = 1;
    FILE* fp = fopen("accountDetails.txt", "r");
    if(!fp) {
        fileExists = 0;
    }
    fp = fopen("accountDetails.txt", "a");
    if (!fp) {
        printf("Error opening accountDetails.txt log file!\n");
        return;
    }

    if(!fileExists){
        fprintf(fp, "AccountNo|Name|Balance|MobileNumber|MailId|Password\n");
    }

    fprintf(fp, "%ld|%s|%f|%s|%s|%s\n", accDetails->accountNumber, accDetails->name,
                 accDetails->accountBalance, accDetails->mobileNumber, accDetails->mailId,
                                                        accDetails->password);

    fflush(fp);
    fclose(fp);
}

/*void writeToJournal(){
    
}*/