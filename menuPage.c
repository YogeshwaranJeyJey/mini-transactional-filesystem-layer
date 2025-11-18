#include "common.h"
#include "menuPage.h"
#include "bankServices.h"
#include "displaySystemOps.h"
#include "crashSimulationHandler.h"

void displayProjectTitle(){
    int padding = doPadding(PROJECTTITLE);
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",PROJECTTITLE, padding, "");
    printf("--------------------------------------------------\n");
}

void displayMainMenu(){
    int padding = doPadding(WELCOMEMSG);
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",WELCOMEMSG, padding, "");
    padding = doPadding(MAINMENU);
    printf("%*s%s%*s\n",padding, "",MAINMENU, padding, "");
    printf("1. Create New Account\n");
    printf("2. View Account Details\n");
    printf("3. Update Account Details\n");
    printf("4. Delete Account\n");
}

void displayTransactionalOperations(){
    int padding = doPadding(TRANSACTIONSMENU);
    printf("\n%*s%s%*s\n",padding, "",TRANSACTIONSMENU, padding, "");
    printf("5. Deposit Cash\n");
    printf("6. Withdraw Cash\n");
    printf("7. Transfer Cash\n");
}

void displaySystemOperations(){
    int padding = doPadding(SYSTEMOPTIONSMENU);
    printf("\n%*s%s%*s\n",padding, "",SYSTEMOPTIONSMENU, padding, "");
    printf("8. Show All Accounts\n");
    printf("9. Show Wal Logs\n");
    printf("10. Show Journal Logs\n");
    printf("11. Clear Wal Logs\n");
    printf("12. Clear Journal Logs\n");
}

void displayCrashSimulationOptions(){
    int padding = doPadding(SIMULATECRASHMENU);
    printf("\n%*s%s%*s\n",padding, "",SIMULATECRASHMENU, padding, "");
    printf("13. Simulate Crash After Wal And Journal Write\n");
    printf("14. Simulate Crash After Partial Wal And Journal Write\n");
    printf("15. Simulate Crash Before Commit\n");
    printf("16. Simulate Crash After Commit\n");
}

void displayOtherOptions(){
    int padding = doPadding(OTHEROPTIONS);
    printf("\n%*s%s%*s\n",padding, "",OTHEROPTIONS, padding, "");
    printf("17. About Project\n");
    printf("18. EXIT\n");
    printf("--------------------------------------------------\n");
}

void userChoiceHandler(int userChoice, int *displayAgain){
    switch(userChoice){
            case 1:
            addAccount();
            break;

            case 2:
            viewAccount();
            break;

            case 3:
            updateAccountDetails();
            break;

            case 4:
            deleteAccount();
            break;

            case 5:
            depositCash();
            break;

            case 6:
            withDrawCash();
            break;

            case 7:
            transferCash();
            break;

            case 8:
            showAllAccounts();
            break;

            case 9:
            showWalLogs();
            break;

            /*case 10:
            printf("10\n");
            break;*/

            case 11:
            clearWalLogs();
            break;

            /*case 12:
            printf("12\n");
            break;*/

            case 13:
            simulateCrashAfterWalWrite();
            break;

            case 14:
            simulateCrashAfterPartialWalWrite();
            break;

            case 15:
            simulatCrashBeforeCommit();
            break;

            case 16:
            simulatCrashAfterCommit();
            break;

            /*case 17:
            printf("17\n");
            break;*/

            case 18:
            printf("Exiting...\n");
            break;

            default:
            printf("Enter a valid option!\n");
            *displayAgain = 0;
        }
}