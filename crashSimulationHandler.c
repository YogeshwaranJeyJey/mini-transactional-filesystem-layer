#include "common.h"
#include "crashSimulationHandler.h"

unsigned char crashMode = NO_CRASH;

void simulateCrashAfterWalWrite(){
    printf("Crash after wal write simulation is enabled!\n");
    printf("Please do a transaction!\n");
    crashMode = CRASH_AFTER_WAL;
}

void simulateCrashAfterPartialWalWrite(){
    printf("Crash after partial wal write simulation is enabled!\n");
    printf("Please do a transaction!\n");
    crashMode = CRASH_AFTER_PARTIAL_WAL;
}

void simulatCrashBeforeCommit(){
    printf("Crash before commit simulation is enabled!\n");
    printf("Please do a transaction!\n");
    crashMode = CRASH_BEFORE_COMMIT;
}

void simulatCrashAfterCommit(){
    printf("Crash after commit simulation is enabled!\n");
    printf("Please do a transaction!\n");
    crashMode = CRASH_AFTER_COMMIT;
}