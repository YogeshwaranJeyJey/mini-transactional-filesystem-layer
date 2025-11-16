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

void writeCreateToWal(accountDetails_t* accDetails, long txnId){
    FILE* fp = fopen("wal.txt", "a");
    if (!fp) {
        printf("Error opening wal.txt log file!\n");
        return;
    }

    fprintf(fp, "TXN ID: %ld\n", txnId);
    fprintf(fp, "Operation: CREATE\n\n");
    
    fprintf(fp, "Account Number: %ld\n", accDetails->accountNumber);
    fprintf(fp, "Name: %s\n", accDetails->name);
    fprintf(fp, "Account Balance: %f\n", accDetails->accountBalance);
    fprintf(fp, "MobileNumber: %s\n", accDetails->mobileNumber);
    fprintf(fp, "MailId: %s\n", accDetails->mailId);
    fprintf(fp, "Password: %s\n\n", accDetails->password);

    fprintf(fp, "Commit status: Pending\n");
    fprintf(fp, "End Txn\n\n");

    fflush(fp);
    fclose(fp);
}

void writeUpdateToWal(accountDetails_t* oldAccountDetails, accountDetails_t* newAccountDetails, long txnId){
    FILE* fp = fopen("wal.txt", "a");
    if (!fp) {
        printf("Error opening wal.txt log file!\n");
        return;
    }

    fprintf(fp, "TXN ID: %ld\n", txnId);
    fprintf(fp, "Operation: UPDATE\n\n");
    
    fprintf(fp, "Old Account Number: %ld\n", oldAccountDetails->accountNumber);
    fprintf(fp, "Old Name: %s\n", oldAccountDetails->name);
    fprintf(fp, "Old Account Balance: %f\n", oldAccountDetails->accountBalance);
    fprintf(fp, "Old MobileNumber: %s\n", oldAccountDetails->mobileNumber);
    fprintf(fp, "Old MailId: %s\n", oldAccountDetails->mailId);
    fprintf(fp, "Old Password: %s\n\n", oldAccountDetails->password);

    fprintf(fp, "New Account Number: %ld\n", newAccountDetails->accountNumber);
    fprintf(fp, "New Name: %s\n", newAccountDetails->name);
    fprintf(fp, "New Account Balance: %f\n", newAccountDetails->accountBalance);
    fprintf(fp, "New MobileNumber: %s\n", newAccountDetails->mobileNumber);
    fprintf(fp, "New MailId: %s\n", newAccountDetails->mailId);
    fprintf(fp, "New Password: %s\n\n", newAccountDetails->password);

    fprintf(fp, "Commit status: Pending\n");
    fprintf(fp, "End Txn\n\n");

    fflush(fp);
    fclose(fp);
}

void writeDeleteToWal(accountDetails_t* oldAccountDetails, long txnId){
    FILE* fp = fopen("wal.txt", "a");
    if (!fp) {
        printf("Error opening wal.txt log file!\n");
        return;
    }

    fprintf(fp, "TXN ID: %ld\n", txnId);
    fprintf(fp, "Operation: DELETE\n\n");
    
    fprintf(fp, "Old Account Number: %ld\n", oldAccountDetails->accountNumber);
    fprintf(fp, "Old Name: %s\n", oldAccountDetails->name);
    fprintf(fp, "Old Account Balance: %f\n", oldAccountDetails->accountBalance);
    fprintf(fp, "Old MobileNumber: %s\n", oldAccountDetails->mobileNumber);
    fprintf(fp, "Old MailId: %s\n", oldAccountDetails->mailId);
    fprintf(fp, "Old Password: %s\n\n", oldAccountDetails->password);

    fprintf(fp, "Commit status: Pending\n");
    fprintf(fp, "End Txn\n\n");

    fflush(fp);
    fclose(fp);
}

void writeTransferToWal(accountDetails_t* oldFromAccountDetails, 
    accountDetails_t* fromAccDetails, accountDetails_t* oldToAccountDetails, 
    accountDetails_t* toAccountDetails, long txnId){
        FILE* fp = fopen("wal.txt", "a");
        if (!fp) {
            printf("Error opening wal.txt log file!\n");
            return;
        }
        fprintf(fp, "TXN ID: %ld\n", txnId);
        fprintf(fp, "Operation: TRANSFER\n\n");
    
        fprintf(fp, "Old Account Number: %ld\n", oldFromAccountDetails->accountNumber);
        fprintf(fp, "Old Name: %s\n", oldFromAccountDetails->name);
        fprintf(fp, "Old Account Balance: %f\n\n", oldFromAccountDetails->accountBalance);

        fprintf(fp, "New Account Number: %ld\n", fromAccDetails->accountNumber);
        fprintf(fp, "New Name: %s\n", fromAccDetails->name);
        fprintf(fp, "New Account Balance: %f\n\n", fromAccDetails->accountBalance);

        fprintf(fp, "Old Account Number: %ld\n", oldToAccountDetails->accountNumber);
        fprintf(fp, "Old Name: %s\n", oldToAccountDetails->name);
        fprintf(fp, "Old Account Balance: %f\n\n", oldToAccountDetails->accountBalance);

        fprintf(fp, "New Account Number: %ld\n", toAccountDetails->accountNumber);
        fprintf(fp, "New Name: %s\n", toAccountDetails->name);
        fprintf(fp, "New Account Balance: %f\n\n", toAccountDetails->accountBalance);

        fprintf(fp, "Commit status: Pending\n");
        fprintf(fp, "End Txn\n\n");

        fflush(fp);
        fclose(fp);
    }

void writeAddToOriginalFile(accountDetails_t* accDetails){
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

    fclose(fp);
}

void writeUpdateToOriginalFile(accountDetails_t* newAccountDetails){
    FILE* orgFp = fopen("accountDetails.txt", "r");
    if(!orgFp){
        printf("Error opening the accountDetails.txt file");
    }

    FILE* tempFp = fopen("temp.txt", "w");
    if(!tempFp){
        printf("Error while creating the temp.txt file");
    }

    char line[MAXBUFFERSIZE];
    fgets(line, sizeof(line), orgFp);
    fprintf(tempFp, "%s", line);

    while(fgets(line, sizeof(line), orgFp) != NULL){
        char copyLine[MAXBUFFERSIZE];
        strcpy(copyLine, line);
        char* token = strtok(copyLine, "|");
        long fileAccNo = atol(token);
        
        if(fileAccNo == newAccountDetails->accountNumber){
            fprintf(tempFp, "%ld|%s|%f|%s|%s|%s\n", newAccountDetails->accountNumber, newAccountDetails->name,
                 newAccountDetails->accountBalance, newAccountDetails->mobileNumber, newAccountDetails->mailId,
                                                        newAccountDetails->password);

    
        }
        else{
            fprintf(tempFp, "%s", line);
        }
    }
    fclose(orgFp);
    fclose(tempFp);

    remove("accountDetails.txt");
    rename("temp.txt", "accountDetails.txt");
}

void writeDeleteToOriginalFile(accountDetails_t* accDetails){
    FILE* orgFp = fopen("accountDetails.txt", "r");
    if(!orgFp){
        printf("Error opening the accountDetails.txt file");
    }

    FILE* tempFp = fopen("temp.txt", "w");
    if(!tempFp){
        printf("Error while creating the temp.txt file");
    }

    char line[MAXBUFFERSIZE];
    fgets(line, sizeof(line), orgFp);
    fprintf(tempFp, "%s", line);

    while(fgets(line, sizeof(line), orgFp) != NULL){
        char copyLine[MAXBUFFERSIZE];
        strcpy(copyLine, line);
        char* token = strtok(copyLine, "|");
        long fileAccNo = atol(token);
        
        if(fileAccNo == accDetails->accountNumber){
           continue;
        }
        else{
            fprintf(tempFp, "%s", line);
        }
    }
    fclose(orgFp);
    fclose(tempFp);

    remove("accountDetails.txt");
    rename("temp.txt", "accountDetails.txt");
}

void writeTransferToOriginalFile(accountDetails_t* fromAccDetails, accountDetails_t* toAccDetails){
    FILE* orgFp = fopen("accountDetails.txt", "r");
    if(!orgFp){
        printf("Error opening the accountDetails.txt file");
    }

    FILE* tempFp = fopen("temp.txt", "w");
    if(!tempFp){
        printf("Error while creating the temp.txt file");
    }

    char line[MAXBUFFERSIZE];
    fgets(line, sizeof(line), orgFp);
    fprintf(tempFp, "%s", line);

    while(fgets(line, sizeof(line), orgFp) != NULL){
        char copyLine[MAXBUFFERSIZE];
        strcpy(copyLine, line);
        char* token = strtok(copyLine, "|");
        long fileAccNo = atol(token);
        
        if(fileAccNo == fromAccDetails->accountNumber){
            fprintf(tempFp, "%ld|%s|%f|%s|%s|%s\n", fromAccDetails->accountNumber, fromAccDetails->name,
                 fromAccDetails->accountBalance, fromAccDetails->mobileNumber, fromAccDetails->mailId,
                                                        fromAccDetails->password);
        }
        else if(fileAccNo == toAccDetails->accountNumber){
            fprintf(tempFp, "%ld|%s|%f|%s|%s|%s\n", toAccDetails->accountNumber, toAccDetails->name,
                 toAccDetails->accountBalance, toAccDetails->mobileNumber, toAccDetails->mailId,
                                                        toAccDetails->password);
        }
        else{
            fprintf(tempFp, "%s", line);
        }
    }
    fclose(orgFp);
    fclose(tempFp);

    remove("accountDetails.txt");
    rename("temp.txt", "accountDetails.txt");
}

void writeCommitToWAL(long txnId){
    FILE* fp = fopen("wal.txt", "a");
    if (!fp) return;

    fprintf(fp, "COMMIT %ld\n\n", txnId);

    fflush(fp);
    fclose(fp);
}

int validateAccountDetails(accountDetails_t* accDetails){
    char accInput[50];
    long userEnteredAccNo = 0;
    
    printf("Enter Your Account Number: ");
    fgets(accInput, sizeof(accInput), stdin);
    userEnteredAccNo = atol(accInput);

    FILE* fp = fopen("accountDetails.txt", "r");
    if(!fp) {
        printf("accountDetails.txt log file not found!\n");
        return -1;
    }
    
    char line[MAXBUFFERSIZE];
    fgets(line, sizeof(line), fp);
    
    int AccDetailsFound = 0;

    while(fgets(line, sizeof(line), fp) != NULL){
        char temp[MAXBUFFERSIZE];
        strcpy(temp, line);
        char* token = strtok(temp, "|");
        if(!token)
            continue;
        long accNoFromDataFile = atol(token);
        
        if(accNoFromDataFile == userEnteredAccNo){
            char userEnteredPassword[MAXPASSWDCHAR];
            printf("Enter your account password: ");
            fgets(userEnteredPassword, sizeof(userEnteredPassword), stdin);
            userEnteredPassword[strcspn(userEnteredPassword, "\n")] = '\0';
            
            accDetails->accountNumber = accNoFromDataFile;
            strcpy(accDetails->name, strtok(NULL, "|"));
            accDetails->accountBalance =  atof(strtok(NULL, "|"));
            strcpy(accDetails->mobileNumber, strtok(NULL, "|"));
            strcpy(accDetails->mailId, strtok(NULL, "|"));
            strcpy(accDetails->password, strtok(NULL, "|"));

            accDetails->password[strcspn(accDetails->password, "\n")] = '\0';
        
            if(strcmp(accDetails->password, userEnteredPassword) == 0){
                AccDetailsFound = 1;
                return 1;
            }
            else{
                printf("Wrong Password!\n");
                return 0;
            }
        }
    }
    if(!AccDetailsFound){
        printf("Invalid AccountNumber!\n");
        return 0;
    }
}

void printAccountDetails(accountDetails_t* accDetails){
    printf("\n-----Account Details-----\n");
    printf("AccountNumber: %ld\n", accDetails->accountNumber);
    printf("Name: %s\n", accDetails->name);
    printf("MobileNo: %s\n", accDetails->mobileNumber);
    printf("MailId: %s\n", accDetails->mailId);
    printf("Account Balance: %f\n", accDetails->accountBalance);
}

void updateDetails(accountDetails_t* oldAccDetails){
    accountDetails_t newAccountDetails;
    newAccountDetails = *oldAccDetails;

    char userChoiceBuffer[MAXCHOICECHAR];
    int userChoice = 0;
    
    printf("Choose an option to update\n1. Name\n2. MobileNumber\n3. MailId\n4.Password\n");
    printf("Enter your choice: ");
    fgets(userChoiceBuffer, sizeof(userChoiceBuffer), stdin);
    userChoice = strtol(userChoiceBuffer, NULL, 10);
    
    switch(userChoice){
        case 1:
        printf("Enter New Name: ");
        fgets(newAccountDetails.name, sizeof(newAccountDetails.name), stdin);
        newAccountDetails.name[strcspn(newAccountDetails.name, "\n")] = '\0';
        break;
        case 2:
        printf("Enter New MobileNumber: ");
        fgets(newAccountDetails.mobileNumber, sizeof(newAccountDetails.mobileNumber), stdin);
        newAccountDetails.mobileNumber[strcspn(newAccountDetails.mobileNumber, "\n")] = '\0';
        break;
        case 3:
        printf("Enter New MailId: ");
        fgets(newAccountDetails.mailId, sizeof(newAccountDetails.mailId), stdin);
        newAccountDetails.mailId[strcspn(newAccountDetails.mailId, "\n")] = '\0';
        break;
        case 4:
        printf("Enter New Password: ");
        fgets(newAccountDetails.password, sizeof(newAccountDetails.password), stdin);
        newAccountDetails.password[strcspn(newAccountDetails.password, "\n")] = '\0';
        break;
        default:
        printf("Invalid choice\n");   
    }
    long txnId = generateTxnId();
    writeUpdateToWal(oldAccDetails, &newAccountDetails, txnId);
    writeUpdateToOriginalFile(&newAccountDetails);
    writeCommitToWAL(txnId);
    
}
void deposit(accountDetails_t* oldAccDetails){
    accountDetails_t newAccountDetails;
    newAccountDetails = *oldAccDetails;

    char cashBuffer[50];
    float depositAmount = 0;

    printf("Enter the amount to deposit: ");
    fgets(cashBuffer, sizeof(cashBuffer), stdin);
    depositAmount = strtof(cashBuffer, NULL);

    if(depositAmount <= 0){
        printf("Invalid amount! Please enter a valid amount!\n");
        return;
    }
    newAccountDetails.accountBalance += depositAmount;
    long txnId = generateTxnId();
    writeUpdateToWal(oldAccDetails, &newAccountDetails, txnId);
    writeUpdateToOriginalFile(&newAccountDetails);
    writeCommitToWAL(txnId);
    return;
}

void withDraw(accountDetails_t* oldAccDetails){
    accountDetails_t newAccountDetails;
    newAccountDetails = *oldAccDetails;

    char cashBuffer[50];
    float withDrawAmount = 0;

    printf("Enter the amount to withdraw: ");
    fgets(cashBuffer, sizeof(cashBuffer), stdin);
    withDrawAmount = strtof(cashBuffer, NULL);

    if(withDrawAmount >= oldAccDetails->accountBalance){
        printf("Insufficient balance!\n");
        return;
    }
    newAccountDetails.accountBalance -= withDrawAmount;
    long txnId = generateTxnId();
    writeUpdateToWal(oldAccDetails, &newAccountDetails, txnId);
    writeUpdateToOriginalFile(&newAccountDetails);
    writeCommitToWAL(txnId);
    return;
}

int validateReceiverAccount(accountDetails_t* toAccountDetails, long userEnteredAccNo){
    FILE* fp = fopen("accountDetails.txt", "r");
    if(!fp) {
        printf("accountDetails.txt log file not found!\n");
        return -1;
    }
    
    char line[MAXBUFFERSIZE];
    fgets(line, sizeof(line), fp);
    int accFound = 0;
    while(fgets(line, sizeof(line), fp) != NULL){
        char temp[MAXBUFFERSIZE];
        strcpy(temp, line);
        char* token = strtok(temp, "|");
        if(!token)
            continue;
        long accNoFromDataFile = atol(token);
        printf("%ld and %ld\n", accNoFromDataFile, userEnteredAccNo);
        
        if(accNoFromDataFile == userEnteredAccNo){
            accFound = 1;
            toAccountDetails->accountNumber = accNoFromDataFile;
            strcpy(toAccountDetails->name, strtok(NULL, "|"));
            toAccountDetails->accountBalance =  atof(strtok(NULL, "|"));
            strcpy(toAccountDetails->mobileNumber, strtok(NULL, "|"));
            strcpy(toAccountDetails->mailId, strtok(NULL, "|"));
            strcpy(toAccountDetails->password, strtok(NULL, "|"));

            toAccountDetails->password[strcspn(toAccountDetails->password, "\n")] = '\0';
            return 1;
        }
    }
    fclose(fp);
    if(!accFound)
        return 0;  
    
}

void transfer(accountDetails_t* fromAccDetails){
    accountDetails_t toAccountDetails;
    accountDetails_t oldFromAccountDetails, oldToAccountDetails;

    oldFromAccountDetails = *fromAccDetails;
    
    char userEnteredAccNoBuffer[10];
    printf("Enter the Receiver's Account Number: ");
    fgets(userEnteredAccNoBuffer, sizeof(userEnteredAccNoBuffer), stdin);
    long userEnteredAccNo = strtol(userEnteredAccNoBuffer, NULL, 10);
    
    if(!validateReceiverAccount(&toAccountDetails, userEnteredAccNo)){
        printf("Receiver account not found!\n");
        return;
    }
    
    oldToAccountDetails = toAccountDetails;

    char buffer[10];
    printf("Enter the amount to transfer: ");
    fgets(buffer, sizeof(buffer), stdin);
    float amount = strtof(buffer, NULL);
    
    fromAccDetails->accountBalance -= amount;
    toAccountDetails.accountBalance += amount;
            
    long txnId = generateTxnId();
    writeTransferToWal(&oldFromAccountDetails, fromAccDetails, &oldToAccountDetails, &toAccountDetails, txnId);
    writeTransferToOriginalFile(fromAccDetails, &toAccountDetails);      
    writeCommitToWAL(txnId);
    return;
}

/*void writeToJournal(){
    
}*/