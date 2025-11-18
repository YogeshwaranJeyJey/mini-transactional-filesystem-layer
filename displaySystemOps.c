#include "common.h"
#include "displaySystemOps.h"

int validateAdmin(){
    char enteredPassKey[PASSKEYLENGTH];
    
    printf("Enter Admin Password: ");
    fgets(enteredPassKey, sizeof(enteredPassKey), stdin);
    enteredPassKey[strcspn(enteredPassKey, "\n")] = '\0';
    
    if(strcmp(enteredPassKey, ADMINPASSKEY) == 0) return 1;
    else return 0;
}

void showAllAccounts(){
    if(validateAdmin()){
        printf("Access granted!\n");
        printf("\n-----ABC Bank Customers Account Details-----\n\n");
        FILE* fp = fopen("accountDetails.txt", "r");
        if(!fp){
            printf("Error opening accountDetails.txt file!\n");
            return;
        }
        char line[MAXBUFFERSIZE];
        while(fgets(line, sizeof(line), fp) != NULL){
            printf("%s\n", line);
        }
    }
    else{
        printf("Access Denied!\n");
        return;
    }    
}

void showWalLogs(){
    if(validateAdmin()){
        printf("Access granted!\n");
        printf("\n-----WAL Log Details-----\n\n");
        FILE* fp = fopen("wal.txt", "r");
        if(!fp){
            printf("Error opening wal.txt file!\n");
            return;
        }
        char line[MAXBUFFERSIZE];
        while(fgets(line, sizeof(line), fp) != NULL){
            printf("%s", line);
        }
    }
    else{
        printf("Access Denied!\n");
        return;
    }
}

void clearWalLogs(){
    if(validateAdmin()){
        printf("Access granted!\n");
        
        FILE* fp = fopen("wal.txt", "w");
        if(!fp){
            printf("Error opening temp.txt file!\n");
            return;
        }
        fclose(fp);
        printf("Wal logs cleared!\n");
    }
    else{
        printf("Access Denied!\n");
        return;
    }
}