#include "common.h"
#include "menuPage.h"

int main(){
    char userChoiceBuffer[MAXCHOICECHAR];
    int userChoice = 0, displayAgain = 1;
    while(userChoice != 18){
        if(displayAgain){
            displayProjectTitle();
        }
        //startRecovery();
        displayMainMenu();
        displayTransactionalOperations();
        displaySystemOperations();
        displayCrashSimulationOptions();
        displayOtherOptions();
        
        printf("Enter your choice: ");
        fgets(userChoiceBuffer, sizeof(userChoiceBuffer), stdin);
        userChoice = strtol(userChoiceBuffer, NULL, MAXCHOICECHAR);
        
        userChoiceHandler(userChoice, &displayAgain);
    }
    return 0;
}