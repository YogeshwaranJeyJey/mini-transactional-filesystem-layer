#ifndef CRASHSIMULATIONHANDLER_H
#define CRASHSIMULATIONHANDLER_H

#define NO_CRASH 0
#define CRASH_AFTER_WAL 1
#define CRASH_AFTER_PARTIAL_WAL 2
#define CRASH_BEFORE_COMMIT 3
#define CRASH_AFTER_COMMIT 4

extern unsigned char crashMode;

void simulateCrashAfterWalWrite();
void simulateCrashAfterPartialWalWrite();
void simulatCrashBeforeCommit();
void simulatCrashAfterCommit();

#endif