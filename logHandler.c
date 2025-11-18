#include "common.h"
#include "logHandler.h"
#include "bankServices.h"
#include "crashSimulationHandler.h"

int checkValidPassword(char* passwd, char* reTypedPasswd){
    if(strcmp(passwd, reTypedPasswd) == 0) return 1;
    return 0;
}

long generateNewAccNumber(){
    long lastAccNumber = 100;
    FILE* fp = fopen(ACCOUNTNUMBERCOUNTERTXTPATH, "r");
    if(!fp){
        fp = fopen(ACCOUNTNUMBERCOUNTERTXTPATH, "w");
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

    fp = fopen(ACCOUNTNUMBERCOUNTERTXTPATH, "w");
    if(!fp){
        printf("Error occurred while writing the newAccNumber!\n");
        return -1;
    }
    fprintf(fp, "%ld", newAccNumber);
    fclose(fp);
    return newAccNumber;
}

int validateName(char* name){
    int len = strlen(name);
    if(len == 0 || len > 20)
        return 0;
    for(int i = 0; i < len; i++){
        if(!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }
    return 1;
}

int validateMobileNumber(char *mobile){
    int len = strlen(mobile);

    if(len != 10)
        return 0;

    if(mobile[0] != '6' && mobile[0] != '7' &&
       mobile[0] != '8' && mobile[0] != '9')
        return 0;

    for(int i = 0; i < 10; i++){
        if(!isdigit((unsigned char)mobile[i]))
            return 0;
    }

    return 1;
}

int validateMail(char *mail){
    int len = strlen(mail);

    if(len < 10)
        return 0;

    if(strstr(mail, "@gmail.com") != NULL)
        return 1;

    if(strstr(mail, "@yahoo.com") != NULL)
        return 1;

    return 0;
}

int getNewAccountDetails(accountDetails_t* accDetails){
    char passwd[MAXPASSWDCHAR];
    char reTypedPasswd[MAXPASSWDCHAR];
    
    int passwdRepeatFlag = 1;
    int padding = doPadding(ADDACCOUNTHEADER);
    
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",ADDACCOUNTHEADER, padding, "");

    printf("Enter Your Name: ");
    fgets(accDetails->name, sizeof(accDetails->name), stdin);
    accDetails->name[strcspn(accDetails->name, "\n")] = '\0';

    while(!validateName(accDetails->name)){
        printf("Invalid Name! Max 20 chars allowed!\n");
        printf("Re-enter your name: ");
        fgets(accDetails->name, sizeof(accDetails->name), stdin);
        accDetails->name[strcspn(accDetails->name, "\n")] = '\0';
    }

    printf("Enter Your MobileNumber: ");
    fgets(accDetails->mobileNumber, sizeof(accDetails->mobileNumber), stdin);
    accDetails->mobileNumber[strcspn(accDetails->mobileNumber, "\n")] = '\0';

    while(!validateMobileNumber(accDetails->mobileNumber)){
        printf("Invalid Mobile Number! Max 10 digits, no alpha allowed, should start with 6,7,8 or 9\n");
        printf("Re-enter your Mobile Number: ");
        fgets(accDetails->mobileNumber, sizeof(accDetails->mobileNumber), stdin);
        accDetails->mobileNumber[strcspn(accDetails->mobileNumber, "\n")] = '\0';
    }

    printf("Enter Your Mail Id: ");
    fgets(accDetails->mailId, sizeof(accDetails->mailId), stdin);
    accDetails->mailId[strcspn(accDetails->mailId, "\n")] = '\0';

    while(!validateMail(accDetails->mailId)){
        printf("Invalid Mail id! Mail id should end with @gmail.com or @yahoo.com\n");
        printf("Re-enter your Mail Id: ");
        fgets(accDetails->mailId, sizeof(accDetails->mailId), stdin);
        accDetails->mailId[strcspn(accDetails->mailId, "\n")] = '\0';
    }

    while(passwdRepeatFlag){
        printf("Set Password For Your Account\n");
        printf("Enter New Password (Max 6 characters): ");
        fgets(passwd, sizeof(passwd), stdin);
        passwd[strcspn(passwd, "\n")] = '\0';

        printf("Confirm Password: ");
        fgets(reTypedPasswd, sizeof(reTypedPasswd), stdin);
        reTypedPasswd[strcspn(reTypedPasswd, "\n")] = '\0';
        
        if(checkValidPassword(passwd, reTypedPasswd)){
            strcpy(accDetails->password, passwd);
            printf("Password Set Successfully!\n");
            break;
        }
        else{
            printf("Error: Passwords do not match!\n\n");
            return 0;
        }
    }
    return 1;
}

long generateTxnId(){
    long lastTxnCounterId = LAST_TXN_COUNTERID;
    FILE* fp = fopen(TXN_COUNTER_TXTFILE_PATH, "r");
    if(!fp){
        fp = fopen(TXN_COUNTER_TXTFILE_PATH, "w");
        if(!fp){
            printf("Error creating txnCounter.txt file!\n");
            return -1;
        }
        lastTxnCounterId = 1001;
        fprintf(fp, "%ld", lastTxnCounterId);
        fclose(fp);
        return lastTxnCounterId;
    }
    fscanf(fp, "%ld", &lastTxnCounterId);
    fclose(fp);
    long newTxnCounterId = lastTxnCounterId + 1;

    fp = fopen(TXN_COUNTER_TXTFILE_PATH, "w");
    if(!fp){
        printf("Error occurred while writing the newCounterId!\n");
        return -1;
    }
    fprintf(fp, "%ld", newTxnCounterId);
    fclose(fp);
    return newTxnCounterId;
}

void writeCreateToWal(accountDetails_t* accDetails, long txnId, int recoveryFlag){
    FILE* fp = NULL;
    if(recoveryFlag)
        fp = fopen(NEW_WAL_TXTFILE_PATH, "a");
    else
        fp = fopen(WAL_TXTFILE_PATH, "a");
    
    if (!fp) {
        printf("Error opening wal.txt log file!\n");
        return;
    }

    fprintf(fp, "TXN ID: %ld\n", txnId);
    fprintf(fp, "Operation: CREATE\n\n");
    
    fprintf(fp, "Account Number: %ld\n", accDetails->accountNumber);
    fprintf(fp, "Name: %s\n", accDetails->name);
    fprintf(fp, "Account Balance: %.2f\n", accDetails->accountBalance);
    fprintf(fp, "MobileNumber: %s\n", accDetails->mobileNumber);

    if (crashMode == CRASH_AFTER_PARTIAL_WAL) {
        fprintf(fp, "\n");
        printf("Simulating crash after partial Wal write...\n");
        crashMode = NO_CRASH;
        exit(1);
    }

    fprintf(fp, "MailId: %s\n", accDetails->mailId);
    fprintf(fp, "Password: %s\n\n", accDetails->password);

    fprintf(fp, "Commit status: Pending\n");
    fprintf(fp, "End Txn\n\n");

    if(recoveryFlag)
        fprintf(fp, "COMMIT %ld\n\n", txnId);

    fflush(fp);

    if(crashMode == CRASH_AFTER_WAL){
        printf("Simulating crash after Wal write...\n");
        crashMode = NO_CRASH;
        exit(1);
    }

    fclose(fp);
}

void writeUpdateToWal(accountDetails_t* oldAccountDetails, accountDetails_t* newAccountDetails, long txnId, int recoveryFlag){
    FILE* fp = NULL;
    if(recoveryFlag)
        fp = fopen(NEW_WAL_TXTFILE_PATH, "a");
    else
        fp = fopen(WAL_TXTFILE_PATH, "a");
    
    if (!fp) {
        printf("Error opening wal.txt log file!\n");
        return;
    }

    fprintf(fp, "TXN ID: %ld\n", txnId);
    fprintf(fp, "Operation: UPDATE\n\n");
    
    fprintf(fp, "Old Account Number: %ld\n", oldAccountDetails->accountNumber);
    fprintf(fp, "Old Name: %s\n", oldAccountDetails->name);
    fprintf(fp, "Old Account Balance: %.2f\n", oldAccountDetails->accountBalance);
    fprintf(fp, "Old MobileNumber: %s\n\n", oldAccountDetails->mobileNumber);

    if (crashMode == CRASH_AFTER_PARTIAL_WAL) {
        fprintf(fp, "\n");
        printf("Simulating crash after partial Wal write...\n");
        crashMode = NO_CRASH;
        exit(1);
    }

    fprintf(fp, "Old MailId: %s\n", oldAccountDetails->mailId);
    fprintf(fp, "Old Password: %s\n\n", oldAccountDetails->password);

    fprintf(fp, "New Account Number: %ld\n", newAccountDetails->accountNumber);
    fprintf(fp, "New Name: %s\n", newAccountDetails->name);
    fprintf(fp, "New Account Balance: %.2f\n", newAccountDetails->accountBalance);
    fprintf(fp, "New MobileNumber: %s\n", newAccountDetails->mobileNumber);
    fprintf(fp, "New MailId: %s\n", newAccountDetails->mailId);
    fprintf(fp, "New Password: %s\n\n", newAccountDetails->password);

    fprintf(fp, "Commit status: Pending\n");
    fprintf(fp, "End Txn\n\n");
    
    if(recoveryFlag)
        fprintf(fp, "COMMIT %ld\n\n", txnId);

    fflush(fp);
    
    if(crashMode == CRASH_AFTER_WAL){
        printf("Simulating crash after Wal write...\n");
        crashMode = NO_CRASH;
        exit(1);
    }

    fclose(fp);
}

void writeDeleteToWal(accountDetails_t* oldAccountDetails, long txnId){
    FILE* fp = fopen(WAL_TXTFILE_PATH, "a");
    if (!fp) {
        printf("Error opening wal.txt log file!\n");
        return;
    }

    fprintf(fp, "TXN ID: %ld\n", txnId);
    fprintf(fp, "Operation: DELETE\n\n");
    
    fprintf(fp, "Old Account Number: %ld\n", oldAccountDetails->accountNumber);
    fprintf(fp, "Old Name: %s\n", oldAccountDetails->name);
    fprintf(fp, "Old Account Balance: %.2f\n", oldAccountDetails->accountBalance);
    fprintf(fp, "Old MobileNumber: %s\n\n", oldAccountDetails->mobileNumber);

    if (crashMode == CRASH_AFTER_PARTIAL_WAL) {
        fprintf(fp, "\n");
        printf("Simulating crash after partial Wal write...\n");
        crashMode = NO_CRASH;
        exit(1);
    }

    fprintf(fp, "Old MailId: %s\n", oldAccountDetails->mailId);
    fprintf(fp, "Old Password: %s\n\n", oldAccountDetails->password);

    fprintf(fp, "Commit status: Pending\n");
    fprintf(fp, "End Txn\n\n");

    fflush(fp);

    if(crashMode == CRASH_AFTER_WAL){
        printf("Simulating crash after Wal write...\n");
        crashMode = NO_CRASH;
        exit(1);
    }

    fclose(fp);
}

void writeTransferToWal(accountDetails_t* oldFromAccountDetails, 
    accountDetails_t* fromAccDetails, accountDetails_t* oldToAccountDetails, 
    accountDetails_t* toAccountDetails, long txnId, int recoveryFlag){
        FILE* fp = NULL;
        if(recoveryFlag)
            fp = fopen(NEW_WAL_TXTFILE_PATH, "a");
        else
            fp = fopen(WAL_TXTFILE_PATH, "a");
        
        if (!fp) {
            printf("Error opening wal.txt log file!\n");
            return;
        }
        fprintf(fp, "TXN ID: %ld\n", txnId);
        fprintf(fp, "Operation: TRANSFER\n\n");
    
        fprintf(fp, "Old Account Number: %ld\n", oldFromAccountDetails->accountNumber);
        fprintf(fp, "Old Name: %s\n", oldFromAccountDetails->name);
        fprintf(fp, "Old Account Balance: %.2f\n\n", oldFromAccountDetails->accountBalance);
        fprintf(fp, "Old MobileNumber: %s\n", oldFromAccountDetails->mobileNumber);
        fprintf(fp, "Old MailId: %s\n", oldFromAccountDetails->mailId);
        fprintf(fp, "Old Password: %s\n\n", oldFromAccountDetails->password);

        fprintf(fp, "New Account Number: %ld\n", fromAccDetails->accountNumber);
        fprintf(fp, "New Name: %s\n\n", fromAccDetails->name);

        if (crashMode == CRASH_AFTER_PARTIAL_WAL) {
            printf("Simulating crash after partial Wal write...\n");
            printf("System crashed!\n");
            crashMode = NO_CRASH;
            exit(1);
        }

        fprintf(fp, "New Account Balance: %f\n\n", fromAccDetails->accountBalance);
        fprintf(fp, "New MobileNumber: %s\n", fromAccDetails->mobileNumber);
        fprintf(fp, "New MailId: %s\n", fromAccDetails->mailId);
        fprintf(fp, "New Password: %s\n\n", fromAccDetails->password);

        fprintf(fp, "Old Account Number: %ld\n", oldToAccountDetails->accountNumber);
        fprintf(fp, "Old Name: %s\n", oldToAccountDetails->name);
        fprintf(fp, "Old Account Balance: %.2f\n\n", oldToAccountDetails->accountBalance);
        fprintf(fp, "Old MobileNumber: %s\n", oldToAccountDetails->mobileNumber);
        fprintf(fp, "Old MailId: %s\n", oldToAccountDetails->mailId);
        fprintf(fp, "Old Password: %s\n\n", oldToAccountDetails->password);

        fprintf(fp, "New Account Number: %ld\n", toAccountDetails->accountNumber);
        fprintf(fp, "New Name: %s\n", toAccountDetails->name);
        fprintf(fp, "New Account Balance: %.2f\n\n", toAccountDetails->accountBalance);
        fprintf(fp, "New MobileNumber: %s\n", toAccountDetails->mobileNumber);
        fprintf(fp, "New MailId: %s\n", toAccountDetails->mailId);
        fprintf(fp, "New Password: %s\n\n", toAccountDetails->password);

        fprintf(fp, "Commit status: Pending\n");
        fprintf(fp, "End Txn\n\n");

        if(recoveryFlag)
            fprintf(fp, "COMMIT %ld\n\n", txnId);

        fflush(fp);

        if(crashMode == CRASH_AFTER_WAL){
            printf("Simulating crash after Wal write...\n");
            printf("System crashed!\n");
            crashMode = NO_CRASH;
            exit(1);
        }

        fclose(fp);
}

void writeAddToOriginalFile(accountDetails_t* accDetails){
    int fileExists = 1;
    FILE* fp = fopen(ACCOUNT_DETAILS_TXTFILE_PATH, "r");
    if(!fp) {
        fileExists = 0;
    }
    fp = fopen(ACCOUNT_DETAILS_TXTFILE_PATH, "a");
    if (!fp) {
        printf("Error opening accountDetails.txt log file!\n");
        return;
    }

    if(!fileExists){
        fprintf(fp, "AccountNo|Name|Balance|MobileNumber|MailId|Password\n");
    }

    fprintf(fp, "%ld|%s|%.2f|%s|%s|%s\n", accDetails->accountNumber, accDetails->name,
                 accDetails->accountBalance, accDetails->mobileNumber, accDetails->mailId,
                                                        accDetails->password);

    fclose(fp);
}

void writeUpdateToOriginalFile(accountDetails_t* newAccountDetails){
    FILE* orgFp = fopen(ACCOUNT_DETAILS_TXTFILE_PATH, "r");
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
            fprintf(tempFp, "%ld|%s|%.2f|%s|%s|%s\n", newAccountDetails->accountNumber, newAccountDetails->name,
                 newAccountDetails->accountBalance, newAccountDetails->mobileNumber, newAccountDetails->mailId,
                                                        newAccountDetails->password);
        }
        else{
            fprintf(tempFp, "%s", line);
        }
    }
    fclose(orgFp);
    fclose(tempFp);

    remove(ACCOUNT_DETAILS_TXTFILE_PATH);
    rename("temp.txt", ACCOUNT_DETAILS_TXTFILE_PATH);
}

void writeDeleteToOriginalFile(accountDetails_t* accDetails){
    FILE* orgFp = fopen(ACCOUNT_DETAILS_TXTFILE_PATH, "r");
    if(!orgFp){
        printf("Error opening the accountDetails.txt file");
        return;
    }

    FILE* tempFp = fopen("temp.txt", "w");
    if(!tempFp){
        printf("Error while creating the temp.txt file");
        return;
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

    remove(ACCOUNT_DETAILS_TXTFILE_PATH);
    rename("temp.txt", ACCOUNT_DETAILS_TXTFILE_PATH);
}

void writeTransferToOriginalFile(accountDetails_t* fromAccDetails, accountDetails_t* toAccDetails){
    FILE* orgFp = fopen(ACCOUNT_DETAILS_TXTFILE_PATH, "r");
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
            fprintf(tempFp, "%ld|%s|%.2f|%s|%s|%s\n", fromAccDetails->accountNumber, fromAccDetails->name,
                 fromAccDetails->accountBalance, fromAccDetails->mobileNumber, fromAccDetails->mailId,
                                                        fromAccDetails->password);
        }
        else if(fileAccNo == toAccDetails->accountNumber){
            fprintf(tempFp, "%ld|%s|%.2f|%s|%s|%s\n", toAccDetails->accountNumber, toAccDetails->name,
                 toAccDetails->accountBalance, toAccDetails->mobileNumber, toAccDetails->mailId,
                                                        toAccDetails->password);
        }
        else{
            fprintf(tempFp, "%s", line);
        }
    }
    fclose(orgFp);
    fclose(tempFp);

    remove(ACCOUNT_DETAILS_TXTFILE_PATH);
    rename("temp.txt", ACCOUNT_DETAILS_TXTFILE_PATH);
}

void writeCommitToWAL(long txnId){
    if(crashMode == CRASH_BEFORE_COMMIT){
        printf("Simulating crash before commit...\n");
        printf("System crashed!\n");
        crashMode = NO_CRASH;
        exit(1);
    }
    
    FILE* fp = fopen("wal.txt", "a");
    if (!fp) return;

    fprintf(fp, "COMMIT %ld\n\n", txnId);

    fflush(fp);
    fclose(fp);

    if(crashMode == CRASH_AFTER_COMMIT){
        printf("Simulating crash after commit...\n");
        printf("System crashed!\n");
        crashMode = NO_CRASH;
        exit(1);
    }
}

int validateAccountDetails(accountDetails_t* accDetails){
    char accInput[50];
    long userEnteredAccNo = 0;
    
    printf("Enter Your Account Number: ");
    fgets(accInput, sizeof(accInput), stdin);
    userEnteredAccNo = atol(accInput);

    FILE* fp = fopen(ACCOUNT_DETAILS_TXTFILE_PATH, "r");
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
                fclose(fp);
                return 1;
            }
            else{
                printf("Wrong Password!\n");
                fclose(fp);
                return 0;
            }
        }
    }
    if(!AccDetailsFound){
        printf("Invalid AccountNumber!\n");
    }
    fclose(fp);
    return 0;
}

void printAccountDetails(accountDetails_t* accDetails){
    printf("\n-----Account Details-----\n");
    printf("AccountNumber: %ld\n", accDetails->accountNumber);
    printf("Name: %s\n", accDetails->name);
    printf("MobileNo: %s\n", accDetails->mobileNumber);
    printf("MailId: %s\n", accDetails->mailId);
    printf("Account Balance: %f\n", accDetails->accountBalance);
}

int updateDetails(accountDetails_t* oldAccDetails){
    accountDetails_t newAccountDetails;
    newAccountDetails = *oldAccDetails;

    char userChoiceBuffer[MAXCHOICECHAR];
    int userChoice = 0;
    
    printf("Choose an option to update\n1. Name\n2. MobileNumber\n3. MailId\n4.Password\n");
    printf("Enter your choice: ");
    fgets(userChoiceBuffer, sizeof(userChoiceBuffer), stdin);
    userChoice = strtol(userChoiceBuffer, NULL, 10);
    char updatedDetail[MAXBUFFERSIZE];
    
    switch(userChoice){
        case 1:
        printf("Enter New Name: ");
        fgets(newAccountDetails.name, sizeof(newAccountDetails.name), stdin);
        newAccountDetails.name[strcspn(newAccountDetails.name, "\n")] = '\0';
        strcpy(updatedDetail, newAccountDetails.name);
        break;
        case 2:
        printf("Enter New MobileNumber: ");
        fgets(newAccountDetails.mobileNumber, sizeof(newAccountDetails.mobileNumber), stdin);
        newAccountDetails.mobileNumber[strcspn(newAccountDetails.mobileNumber, "\n")] = '\0';
        strcpy(updatedDetail, newAccountDetails.mobileNumber);
        break;
        case 3:
        printf("Enter New MailId: ");
        fgets(newAccountDetails.mailId, sizeof(newAccountDetails.mailId), stdin);
        newAccountDetails.mailId[strcspn(newAccountDetails.mailId, "\n")] = '\0';
        strcpy(updatedDetail, newAccountDetails.mailId);
        break;
        case 4:
        printf("Enter New Password: ");
        fgets(newAccountDetails.password, sizeof(newAccountDetails.password), stdin);
        newAccountDetails.password[strcspn(newAccountDetails.password, "\n")] = '\0';
        strcpy(updatedDetail, newAccountDetails.password);
        break;
        default:
        printf("Invalid choice\n"); 
        return 0;  
    }
    long txnId = generateTxnId();
    writeUpdateToWal(oldAccDetails, &newAccountDetails, txnId, 0);
    writeCommitToWAL(txnId);
    writeUpdateToOriginalFile(&newAccountDetails);
    
    printf("%s updated successfully for the account number %ld!\n", updatedDetail, newAccountDetails.accountNumber);
    return 1;  
}
int deposit(accountDetails_t* oldAccDetails){
    accountDetails_t newAccountDetails;
    newAccountDetails = *oldAccDetails;

    char cashBuffer[50];
    float depositAmount = 0;

    printf("Enter the amount to deposit: ");
    fgets(cashBuffer, sizeof(cashBuffer), stdin);
    depositAmount = strtof(cashBuffer, NULL);

    if(depositAmount <= 0){
        printf("Invalid amount! Please enter a valid amount!\n");
        return 0;
    }
    newAccountDetails.accountBalance += depositAmount;
    long txnId = generateTxnId();
    writeUpdateToWal(oldAccDetails, &newAccountDetails, txnId, 0);
    writeCommitToWAL(txnId);
    writeUpdateToOriginalFile(&newAccountDetails);
    
    printf("Rs.%.2f deposited to the account number %ld\n", depositAmount, newAccountDetails.accountNumber);
    return 1;
}

int withDraw(accountDetails_t* oldAccDetails){
    accountDetails_t newAccountDetails;
    newAccountDetails = *oldAccDetails;

    char cashBuffer[50];
    float withDrawAmount = 0;

    printf("Enter the amount to withdraw: ");
    fgets(cashBuffer, sizeof(cashBuffer), stdin);
    withDrawAmount = strtof(cashBuffer, NULL);

    if(withDrawAmount > oldAccDetails->accountBalance){
        printf("Insufficient balance!\n");
        return 0;
    }
    newAccountDetails.accountBalance -= withDrawAmount;
    long txnId = generateTxnId();
    writeUpdateToWal(oldAccDetails, &newAccountDetails, txnId, 0);
    writeCommitToWAL(txnId);
    writeUpdateToOriginalFile(&newAccountDetails);
    
    printf("Rs.%.2f withdrawn from the account number %ld\n", withDrawAmount, newAccountDetails.accountNumber);
    return 1;
}

int validateReceiverAccount(accountDetails_t* toAccountDetails, long userEnteredAccNo){
    FILE* fp = fopen(ACCOUNT_DETAILS_TXTFILE_PATH, "r");
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
        
        if(accNoFromDataFile == userEnteredAccNo){
            accFound = 1;
            toAccountDetails->accountNumber = accNoFromDataFile;
            strcpy(toAccountDetails->name, strtok(NULL, "|"));
            toAccountDetails->accountBalance =  atof(strtok(NULL, "|"));
            strcpy(toAccountDetails->mobileNumber, strtok(NULL, "|"));
            strcpy(toAccountDetails->mailId, strtok(NULL, "|"));
            strcpy(toAccountDetails->password, strtok(NULL, "|"));

            toAccountDetails->password[strcspn(toAccountDetails->password, "\n")] = '\0';
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    if(!accFound)
        return 0;
    return 0;  
}

int transfer(accountDetails_t* fromAccDetails){
    accountDetails_t toAccountDetails;
    accountDetails_t oldFromAccountDetails, oldToAccountDetails;

    oldFromAccountDetails = *fromAccDetails;
    
    char userEnteredAccNoBuffer[10];
    printf("Enter the Receiver's Account Number: ");
    fgets(userEnteredAccNoBuffer, sizeof(userEnteredAccNoBuffer), stdin);
    long userEnteredAccNo = strtol(userEnteredAccNoBuffer, NULL, 10);
    
    if(!validateReceiverAccount(&toAccountDetails, userEnteredAccNo)){
        printf("Receiver account not found!\n");
        return 0;
    }
    
    oldToAccountDetails = toAccountDetails;

    char buffer[10];
    printf("Enter the amount to transfer: ");
    fgets(buffer, sizeof(buffer), stdin);
    float amount = strtof(buffer, NULL);
    
    fromAccDetails->accountBalance -= amount;
    toAccountDetails.accountBalance += amount;
            
    long txnId = generateTxnId();
    writeTransferToWal(&oldFromAccountDetails, fromAccDetails, &oldToAccountDetails, &toAccountDetails, txnId, 0);
    writeCommitToWAL(txnId);
    writeTransferToOriginalFile(fromAccDetails, &toAccountDetails);      
    
    printf("Rs.%.2f is transferred from accountNumber %ld to accountNumber %ld!\n", amount, fromAccDetails->accountNumber, toAccountDetails.accountNumber);
    return 1;
}