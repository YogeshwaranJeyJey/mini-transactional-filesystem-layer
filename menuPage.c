#include "menuPage.h" 

int doPadding(char* string){
    return (WIDTH - strlen(string)) / 2;
}

void displayProjectTitle(){
    int padding = doPadding(PROJECTTITLE);
    printf("--------------------------------------------------\n");
    printf("%*s%s%*s\n",padding, "",PROJECTTITLE, padding, "");
    printf("--------------------------------------------------\n");
}

void displayMainMenu(){
    
}

void displayTransactionalOperations(){

}

void displayDeveloperOptions(){

}

void displayOtherOptions(){

}