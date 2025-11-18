#include "common.h"
#include "crashRecovery.h"
#include "bankServices.h"
#include "logHandler.h"

long getLastTxnId(char* checkSumPath){
    FILE* fp = fopen(checkSumPath, "r");
    if(!fp){
        fp = fopen(checkSumPath, "w");
        if(!fp){
            printf("Error occurred while creating checksum file!\n");
            return -1;
        }
        fprintf(fp, "0\n");
        fclose(fp);
        return 0;
    }
    
    long txnId = 0;
    if(fscanf(fp, "%ld", &txnId) != 1)
        txnId = 0;
    
        fclose(fp);
    return txnId;
}

int isBlank(const char *s){
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    return *s == '\0';
}

int readWalTransaction(FILE *walFp, wal_Txn_t *walTxn)
{
    char line[MAXBUFFERSIZE];
    memset(walTxn, 0, sizeof(wal_Txn_t));

    long posBefore;
    int oldCount = 0;
    int newCount = 0;

    while (1) {
        if (!fgets(line, sizeof(line), walFp))
            return 0;
        if (strncmp(line, "TXN ID:", 7) == 0) {
            char *p = line + 7; while (*p==' '||*p=='\t') p++;
            walTxn->txnId = strtol(p, NULL, 10);
            break;
        }
    }

    if (!fgets(line, sizeof(line), walFp)) return -1;
    if (strncmp(line, "Operation:", 10) != 0) return -1;
    sscanf(line, "Operation: %31s", walTxn->operation);

    fgets(line, sizeof(line), walFp);

    while (1) {
        posBefore = ftell(walFp);
        if (!fgets(line, sizeof(line), walFp))
            return -1;
        trimNewLine(line);

        if (isBlank(line))
            continue;

        if (strncmp(line, "TXN ID:", 7) == 0) {
            fseek(walFp, posBefore, SEEK_SET);
            return -1;
        }

        if (strcmp(line, "End Txn") == 0) {
            walTxn->endTxnFlag = 1;
            break;
        }

        if (strncmp(line, "Old Account Number:", 19) == 0) {
            oldCount++;
            char *p = line + 19; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (oldCount == 1){
                    walTxn->oldFromAccDetails.accountNumber = strtol(p, NULL, 10);
                    walTxn->hasOldFrom = 1;
                }
                else{
                    walTxn->oldToAccDetails.accountNumber = strtol(p, NULL, 10);
                    walTxn->hasOldTo = 1;
                }
            } else {
                walTxn->oldAccDetails.accountNumber = strtol(p, NULL, 10);
                walTxn->hasOld = 1;
            }
            continue;
        }

        if (strncmp(line, "Old Name:", 9) == 0) {
            char *p = line + 9; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (oldCount == 1)
                    strncpy(walTxn->oldFromAccDetails.name, p, sizeof(walTxn->oldFromAccDetails.name)-1);
                else
                    strncpy(walTxn->oldToAccDetails.name, p, sizeof(walTxn->oldToAccDetails.name)-1);
            } else {
                strncpy(walTxn->oldAccDetails.name, p, sizeof(walTxn->oldAccDetails.name)-1);
            }
            continue;
        }

        if (strncmp(line, "Old Account Balance:", 21) == 0) {
            char *p = line + 21; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (oldCount == 1)
                    walTxn->oldFromAccDetails.accountBalance = strtof(p, NULL);
                else
                    walTxn->oldToAccDetails.accountBalance = strtof(p, NULL);
            } else {
                walTxn->oldAccDetails.accountBalance = strtof(p, NULL);
            }
            continue;
        }

        if (strncmp(line, "Old MobileNumber:", 17) == 0) {
            char *p = line + 17; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (oldCount == 1)
                    strncpy(walTxn->oldFromAccDetails.mobileNumber, p, sizeof(walTxn->oldFromAccDetails.mobileNumber)-1);
                else
                    strncpy(walTxn->oldToAccDetails.mobileNumber, p, sizeof(walTxn->oldToAccDetails.mobileNumber)-1);
            } else {
                strncpy(walTxn->oldAccDetails.mobileNumber, p, sizeof(walTxn->oldAccDetails.mobileNumber)-1);
            }
            continue;
        }

        if (strncmp(line, "Old MailId:", 11) == 0) {
            char *p = line + 11; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (oldCount == 1)
                    strncpy(walTxn->oldFromAccDetails.mailId, p, sizeof(walTxn->oldFromAccDetails.mailId)-1);
                else
                    strncpy(walTxn->oldToAccDetails.mailId, p, sizeof(walTxn->oldToAccDetails.mailId)-1);
            } else {
                strncpy(walTxn->oldAccDetails.mailId, p, sizeof(walTxn->oldAccDetails.mailId)-1);
            }
            continue;
        }

        if (strncmp(line, "Old Password:", 13) == 0) {
            char *p = line + 13; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (oldCount == 1)
                    strncpy(walTxn->oldFromAccDetails.password, p, sizeof(walTxn->oldFromAccDetails.password)-1);
                else
                    strncpy(walTxn->oldToAccDetails.password, p, sizeof(walTxn->oldToAccDetails.password)-1);
            } else {
                strncpy(walTxn->oldAccDetails.password, p, sizeof(walTxn->oldAccDetails.password)-1);
            }
            continue;
        }

        if (strncmp(line, "New Account Number:", 19) == 0) {
            newCount++;
            char *p = line + 19; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (newCount == 1){
                    walTxn->newFromAccDetails.accountNumber = strtol(p, NULL, 10);
                    walTxn->hasNewFrom = 1;
                }
                else{
                    walTxn->newToAccDetails.accountNumber = strtol(p, NULL, 10);
                    walTxn->hasNewTo = 1;
                }
            } else {
                walTxn->newAccDetails.accountNumber = strtol(p, NULL, 10);
                walTxn->hasNew = 1;
            }
            continue;
        }

        if (strncmp(line, "New Name:", 9) == 0) {
            char *p = line + 9; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (newCount == 1)
                    strncpy(walTxn->newFromAccDetails.name, p, sizeof(walTxn->newFromAccDetails.name)-1);
                else
                    strncpy(walTxn->newToAccDetails.name, p, sizeof(walTxn->newToAccDetails.name)-1);
            } else {
                strncpy(walTxn->newAccDetails.name, p, sizeof(walTxn->newAccDetails.name)-1);
            }
            continue;
        }

        if (strncmp(line, "New Account Balance:", 20) == 0) {
            char *p = line + 21; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (newCount == 1)
                    walTxn->newFromAccDetails.accountBalance = strtof(p, NULL);
                else
                    walTxn->newToAccDetails.accountBalance = strtof(p, NULL);
            } else {
                walTxn->newAccDetails.accountBalance = strtof(p, NULL);
            }
            continue;
        }

        if (strncmp(line, "New MobileNumber:", 17) == 0) {
            char *p = line + 17; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (newCount == 1)
                    strncpy(walTxn->newFromAccDetails.mobileNumber, p, sizeof(walTxn->newFromAccDetails.mobileNumber)-1);
                else
                    strncpy(walTxn->newToAccDetails.mobileNumber, p, sizeof(walTxn->newToAccDetails.mobileNumber)-1);
            } else {
                strncpy(walTxn->newAccDetails.mobileNumber, p, sizeof(walTxn->newAccDetails.mobileNumber)-1);
            }
            continue;
        }

        if (strncmp(line, "New MailId:", 11) == 0) {
            char *p = line + 11; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (newCount == 1)
                    strncpy(walTxn->newFromAccDetails.mailId, p, sizeof(walTxn->newFromAccDetails.mailId)-1);
                else
                    strncpy(walTxn->newToAccDetails.mailId, p, sizeof(walTxn->newToAccDetails.mailId)-1);
            } else {
                strncpy(walTxn->newAccDetails.mailId, p, sizeof(walTxn->newAccDetails.mailId)-1);
            }
            continue;
        }

        if (strncmp(line, "New Password:", 13) == 0) {
            char *p = line + 13; while(*p==' '||*p=='\t') p++;

            if (!strcmp(walTxn->operation, "TRANSFER")) {
                if (newCount == 1)
                    strncpy(walTxn->newFromAccDetails.password, p, sizeof(walTxn->newFromAccDetails.password)-1);
                else
                    strncpy(walTxn->newToAccDetails.password, p, sizeof(walTxn->newToAccDetails.password)-1);
            } else {
                strncpy(walTxn->newAccDetails.password, p, sizeof(walTxn->newAccDetails.password)-1);
            }
            continue;
        }

        if (strncmp(line, "Account Number:", 15) == 0) {
            char *p = line + 15; while(*p==' '||*p=='\t') p++;
            walTxn->newAccDetails.accountNumber = strtol(p, NULL, 10);
            walTxn->hasNew = 1;
            continue;
        }

        if (strncmp(line, "Name:", 5) == 0) {
            char *p = line + 5; while(*p==' '||*p=='\t') p++;
            strncpy(walTxn->newAccDetails.name, p, sizeof(walTxn->newAccDetails.name)-1);
            walTxn->hasNew = 1;
            continue;
        }

        if (strncmp(line, "Account Balance:", 16) == 0) {
            char *p = line + 16; while(*p==' '||*p=='\t') p++;
            walTxn->newAccDetails.accountBalance = strtof(p, NULL);
            walTxn->hasNew = 1;
            continue;
        }

        if (strncmp(line, "MobileNumber:", 13) == 0) {
            char *p = line + 13; while(*p==' '||*p=='\t') p++;
            strncpy(walTxn->newAccDetails.mobileNumber, p, sizeof(walTxn->newAccDetails.mobileNumber)-1);
            walTxn->hasNew = 1;
            continue;
        }

        if (strncmp(line, "MailId:", 7) == 0) {
            char *p = line + 7; while(*p==' '||*p=='\t') p++;
            strncpy(walTxn->newAccDetails.mailId, p, sizeof(walTxn->newAccDetails.mailId)-1);
            walTxn->hasNew = 1;
            continue;
        }

        if (strncmp(line, "Password:", 9) == 0) {
            char *p = line + 9; while(*p==' '||*p=='\t') p++;
            strncpy(walTxn->newAccDetails.password, p, sizeof(walTxn->newAccDetails.password)-1);
            walTxn->hasNew = 1;
            continue;
        }
    }

    if (!walTxn->endTxnFlag)
        return -1;

    while (1) {
        posBefore = ftell(walFp);
        if (!fgets(line, sizeof(line), walFp))
            break;
        trimNewLine(line);

        if (isBlank(line))
            continue;

        if (strncmp(line, "COMMIT", 6) == 0) {
            char *p = line + 6; while(*p==' '||*p=='\t') p++;
            long cid = strtol(p, NULL, 10);
            if (cid == walTxn->txnId)
                walTxn->commitedFlag = 1;
            break;
        }

        if (strncmp(line, "TXN ID:", 7) == 0) {
            fseek(walFp, posBefore, SEEK_SET);
            break;
        }
    }

    return 1;
}

int accountExists(long accNo){
    FILE* fp = fopen("accountDetails.txt", "r");
    if (!fp) return 0;

    char line[MAXBUFFERSIZE];
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        char copy[MAXBUFFERSIZE];
        strcpy(copy, line);

        long fileAccNo = atol(strtok(copy, "|"));
        if (fileAccNo == accNo) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

int undo(wal_Txn_t *walTxn){
    if (!walTxn) return -1;
    if (!walTxn->endTxnFlag) return 0;
    if (walTxn->commitedFlag) return 0;

    if (strcmp(walTxn->operation, "CREATE") == 0) {
        long accNo = walTxn->newAccDetails.accountNumber;
        if (accNo <= 0) return 0;
        if (accountExists(accNo)) {
            writeDeleteToOriginalFile(&walTxn->newAccDetails);
        }
        return 1;
    }

    if (strcmp(walTxn->operation, "UPDATE") == 0) {
        if (!walTxn->hasOld) return 0;
        long accNo = walTxn->oldAccDetails.accountNumber;
        if (accNo <= 0) return 0;
        if (accountExists(accNo)) {
            writeUpdateToOriginalFile(&walTxn->oldAccDetails);
        } else {
            writeAddToOriginalFile(&walTxn->oldAccDetails);
        }
        return 1;
    }

    if (strcmp(walTxn->operation, "DELETE") == 0) {
        long accNo = walTxn->oldAccDetails.accountNumber;
        if (accNo <= 0) return 0;
        if (!accountExists(accNo)) {
            writeAddToOriginalFile(&walTxn->oldAccDetails);
        }
        return 1;
    }

    if (strcmp(walTxn->operation, "TRANSFER") == 0) {
        if (!(walTxn->hasOldFrom && walTxn->hasOldTo)) return 0;
        writeTransferToOriginalFile(&walTxn->oldFromAccDetails,
                                    &walTxn->oldToAccDetails);
        return 1;
    }

    return 0;
}


int redo(wal_Txn_t* walTxn){
    if (!walTxn)
        return 0;
    
    if (!walTxn->commitedFlag) {
        return 0;
    }

    if(strcmp(walTxn->operation, "CREATE") == 0){
        if(!accountExists(walTxn->newAccDetails.accountNumber)){
            writeAddToOriginalFile(&walTxn->newAccDetails);
        }
        return 1;
    }

    if(strcmp(walTxn->operation, "UPDATE") == 0){
        if(walTxn->hasOld && walTxn->hasNew){
            printf("hi\n");
            printf("%s\n", walTxn->newAccDetails.name);
            writeUpdateToOriginalFile(&walTxn->newAccDetails);
            return 1;
        }
        return 0;
    }

    if(strcmp(walTxn->operation, "DELETE") == 0){
        if(walTxn->hasOld){
            writeUpdateToOriginalFile(&walTxn->newAccDetails);
            return 1;
        }
        return 0;  
    }

    if (strcmp(walTxn->operation, "TRANSFER") == 0) {
        if (!walTxn->hasNewFrom || !walTxn->hasNewTo)
            return 0;
        writeTransferToOriginalFile(
            &walTxn->newFromAccDetails,
            &walTxn->newToAccDetails
        );

        return 1;
    }
    return 0;
}

int updateCheckSum(char* checkSumPath, long lastTxnId){
    FILE* fp = fopen(checkSumPath, "w");
    if(!fp){
        printf("Error while opening checksum.txt file\n");
        return 0;
    }
    fprintf(fp, "%ld", lastTxnId);
    fclose(fp);
    return 1;
}

void printStruct(wal_Txn_t* walTxn){
    if(strcmp(walTxn->operation, "UPDATE") == 0){
        printf("Update old acc details:\n");
        printf("%ld|%s|%f|%s|%s|%s\n", walTxn->oldAccDetails.accountNumber, walTxn->oldAccDetails.name,
                 walTxn->oldAccDetails.accountBalance, walTxn->oldAccDetails.mobileNumber, walTxn->oldAccDetails.mailId,
                                                        walTxn->oldAccDetails.password);
        printf("Update new acc details:\n");
        printf("%ld|%s|%f|%s|%s|%s\n", walTxn->newAccDetails.accountNumber, walTxn->newAccDetails.name,
                 walTxn->newAccDetails.accountBalance, walTxn->newAccDetails.mobileNumber, walTxn->newAccDetails.mailId,
                                                        walTxn->newAccDetails.password);
    }
    else if(strcmp(walTxn->operation, "TRANSFER") == 0){
        printf("Transfer old from acc details:\n");
        printf("%ld|%s|%f|%s|%s|%s\n", walTxn->oldFromAccDetails.accountNumber, walTxn->oldFromAccDetails.name,
                 walTxn->oldFromAccDetails.accountBalance, walTxn->oldFromAccDetails.mobileNumber, walTxn->oldFromAccDetails.mailId,
                                                        walTxn->oldFromAccDetails.password);
         printf("Transfer old to acc details:\n");
        printf("%ld|%s|%f|%s|%s|%s\n", walTxn->oldToAccDetails.accountNumber, walTxn->oldToAccDetails.name,
                 walTxn->oldToAccDetails.accountBalance, walTxn->oldToAccDetails.mobileNumber, walTxn->oldToAccDetails.mailId,
                                                        walTxn->oldToAccDetails.password);
        printf("Transfer new from acc details:\n");
        printf("%ld|%s|%f|%s|%s|%s\n", walTxn->newFromAccDetails.accountNumber, walTxn->newFromAccDetails.name,
                 walTxn->newFromAccDetails.accountBalance, walTxn->newFromAccDetails.mobileNumber, walTxn->newFromAccDetails.mailId,
                                                        walTxn->newFromAccDetails.password);
        printf("Transfer new To acc details:\n");
        printf("%ld|%s|%f|%s|%s|%s\n", walTxn->newToAccDetails.accountNumber, walTxn->newToAccDetails.name,
                 walTxn->newToAccDetails.accountBalance, walTxn->newToAccDetails.mobileNumber, walTxn->newToAccDetails.mailId,
                                                        walTxn->newToAccDetails.password);
    }
}

void startRecovery(){
    printf("Recovery started...\n");
    long lastTxnId = getLastTxnId(CHECKSUMFILEPATH);
    
    FILE* walFp = fopen(WALFILEPATH, "r");
    if(!walFp){
        walFp = fopen(WALFILEPATH, "w");
        if(!walFp){
            printf("Error creating wal.txt file!\n");
            return;
        }
        fclose(walFp);
        printf("No WAL file detected! New WAL file created! No recovery needed\n");
        return;   
    }
    fseek(walFp, 0, SEEK_END);
    long size = ftell(walFp);
    if(size == 0){
        printf("WAL file is Empty! No recovery needed\n");
        return;
    }
    rewind(walFp);

    wal_Txn_t walTxn;
    while(1){
        int returnVal = readWalTransaction(walFp, &walTxn);
         if (returnVal == 0)
            break;
        if (returnVal < 0) {
            printf("Error: Wal format is wrong!\n");
            break;
        }

        printStruct(&walTxn);

        if (!walTxn.endTxnFlag) {
            printf("Skipping partial transaction %ld (incomplete)\n", walTxn.txnId);
            break;
        }

        if (!walTxn.commitedFlag) {
            printf("Undoing uncommited transaction %ld\n", walTxn.txnId);
            if (!undo(&walTxn)){
                printf("Failed to undo txn %ld. Aborting recovery\n", walTxn.txnId);
                break;
            }
            continue;
        }

        if (walTxn.txnId <= lastTxnId) {
            printf("Skipping txn %ld (already applied <= %ld)\n", walTxn.txnId, lastTxnId);
            continue;
        }

        printf("Replaying (REDO) txn %ld op=%s\n", walTxn.txnId, walTxn.operation);
        if (!redo(&walTxn)) {
            printf("Failed to apply txn %ld. Aborting recovery\n", walTxn.txnId);
            break;
        }

        if (!updateCheckSum(CHECKSUMFILEPATH, walTxn.txnId)) {
            printf("Failed to update checkpoint to %ld. Aborting recovery.\n", walTxn.txnId);
            break;
        }
        lastTxnId = walTxn.txnId;
        printf("Checkpoint advanced to %ld\n", lastTxnId);
    }

    fclose(walFp);

    printf("WAL recovery complete. Last applied = %ld\n", lastTxnId);
}