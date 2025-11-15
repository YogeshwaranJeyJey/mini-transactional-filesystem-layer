#ifndef MENUPAGE_H
#define MENUPAGE_H

#define PROJECTTITLE    "MINI TRANSACTIONAL FILESYSTEM LAYER"
#define WELCOMEMSG  "Welcome to ABC Bank!"
#define MAINMENU    "Main Menu"
#define TRANSACTIONSMENU    "Transactional Operations"
#define SYSTEMOPTIONSMENU   "System Operations (Admin only)"
#define SIMULATECRASHMENU   "Crash Simulation options"
#define OTHEROPTIONS    "Other"

void displayProjectTitle();
void displayMainMenu();
void displayTransactionalOperations();
void displaySystemOperations();
void displayCrashSimulationOptions();
void displayOtherOptions();
void userChoiceHandler(int userChoice, int *displayAgain);

#endif