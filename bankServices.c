#include "common.h"
#include "bankServices.h"
#include "logHandler.h"

accountDetails_t accDetails;

void addAccount(){
    if(getNewAccountDetails(&accDetails)){
        accDetails.accountNumber = generateNewAccNumber();
        accDetails.accountBalance = 0.0;
        
        long txnId = generateTxnId();
        if(txnId == -1){
            return;
        }
        
        writeCreateToWal(&accDetails, txnId, 0);
        writeCommitToWAL(txnId);
        writeAddToOriginalFile(&accDetails);
        
        printf("Account created successfully!\n");
    }
    else{
        printf("Account creation failed!\n");
        return;
    }
}

void viewAccount(){
    int padding = doPadding(VIEWACCOUNTHEADER);
    
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",VIEWACCOUNTHEADER, padding, "");
    if(validateAccountDetails(&accDetails)){
        printAccountDetails(&accDetails);
    }
    else{
        printf("Account Validation failed!\n");
        return;
    }
       
}

void updateAccountDetails(){
    int padding = doPadding(UPDATEACCOUNTHEADER);
    
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",UPDATEACCOUNTHEADER, padding, "");
    if(validateAccountDetails(&accDetails)){
        if(updateDetails(&accDetails))
            printf("Detail updated successfully!\n");
        else
             printf("Detail updation failed!\n");  
    }
    else{
        printf("Account Validation failed!\n");
        return;
    }
}

void deleteAccount(){
    int padding = doPadding(DELETEACCOUNTHEADER);
    
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",DELETEACCOUNTHEADER, padding, "");
    if(validateAccountDetails(&accDetails)){
        long txnId = generateTxnId();
        writeDeleteToWal(&accDetails, txnId);
        writeCommitToWAL(txnId);
        writeDeleteToOriginalFile(&accDetails);
        
        printf("Account deleted successfully!\n");
    }
    else{
        printf("Account Validation failed!\n");
        return;
    }
}

void depositCash(){
    int padding = doPadding(DEPOSITHEADER);
    
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",DEPOSITHEADER, padding, "");
    if(validateAccountDetails(&accDetails)){
        if(deposit(&accDetails))
            printf("Cash deposited successfully!\n");
        else
            printf("Cash Not deposited!\n");
    }
    else{
        printf("Account Validation failed!\n");
        return;
    }
}

void withDrawCash(){
    int padding = doPadding(WITHDRAWHEADER);
    
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",WITHDRAWHEADER, padding, "");
    if(validateAccountDetails(&accDetails)){
        if(withDraw(&accDetails))
            printf("Cash withdrawn successfully!\n");
        else
           printf("Cash withdrawl failed!\n"); 
    }
    else{
        printf("Account Validation failed!\n");
        return;
    }
}

void transferCash(){
    int padding = doPadding(TRANSFERHEADER);
    
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",TRANSFERHEADER, padding, "");
    if(validateAccountDetails(&accDetails)){
        if(transfer(&accDetails))
            printf("Cash transferred successfully!\n");
        else
            printf("Cash Not transferred!\n");
    }
    else{
        printf("Account Validation failed!\n");
        return;
    }
}