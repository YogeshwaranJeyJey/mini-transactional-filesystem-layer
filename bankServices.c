#include "common.h"
#include "bankServices.h"

accountDetails_t accDetails;

int checkValidPassword(char* passwd, char* reTypedPasswd){
    if(strcmp(passwd, reTypedPasswd) == 0) return 1;
    return 0;
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
}