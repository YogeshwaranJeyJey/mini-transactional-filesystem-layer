#include "common.h"
#include "menuPage.h"
#include "crashRecovery.h"

int main(){
    startRecovery();
    char userChoiceBuffer[MAXCHOICECHAR];
    int userChoice = 0, displayAgain = 1;
    while(1){
        if(displayAgain){
            displayProjectTitle();
        }
        displayMainMenu();
        displayTransactionalOperations();
        displaySystemOperations();
        displayCrashSimulationOptions();
        displayOtherOptions();
        
        printf("Enter your choice: ");
        fgets(userChoiceBuffer, sizeof(userChoiceBuffer), stdin);
        userChoice = (int)strtol(userChoiceBuffer, NULL, 10);

        if(userChoice == 16){
            printf("Exiting...\n");
            break;
        }

        userChoiceHandler(userChoice, &displayAgain);

        printf("Do you wish to continue(y/n): ");
        fgets(userChoiceBuffer, sizeof(userChoiceBuffer), stdin);
        userChoiceBuffer[strcspn(userChoiceBuffer, "\n")] = '\0';
        if(userChoiceBuffer[0] != 'y' && userChoiceBuffer[0] != 'Y'){
            printf("Exiting...\n");
            break;  
        }
                        
    }
    return 0;
}