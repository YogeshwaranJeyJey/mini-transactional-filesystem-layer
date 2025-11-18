Mini Transactional Filesystem Layer (with WAL, Undo/Redo & Crash Recovery)

This project implements a mini database-like transactional layer on top of a simple text-based file system using Write-Ahead Logging (WAL).
It supports atomic operations, crash simulation, and automatic recovery using Undo/Redo logging + Checkpoints.

Features

1.Write-Ahead Logging (WAL)
2. Atomic CREATE, UPDATE, DELETE, TRANSFER
3. Crash simulation at various stages
4. Automatic Crash Recovery on startup
5. Undo & Redo implementation
6. Checkpoint mechanism using checkPoint.txt
7. Input validation for name, mobile, mail
8. WAL cleanup (removing undone transactions)
9. Clean menu-driven user interface

Project Structure
accountDetails.txt        → Main data file  
wal.txt                   → Write-Ahead Log  
checkPoint.txt            → Last successfully applied transaction ID

main.c                    → Main program entry  
menuPage.c                → UI + menu display  
bankServices.c            → CREATE, UPDATE, DELETE, TRANSFER logic  
logHandler.c              → WAL writing functions  
crashRecovery.c           → Undo/Redo, WAL parsing, checkpoint update  
crashSimulationHandler.c  → Crash testing options  
common.c                  → Utility functions (validators, trim, isBlank)

Build Instructions
Using Makefile
make - build
./run or make run

How It Works
Transaction Workflow

Every operation follows this sequence:

Validate user input

Write WAL entry (old/new values)

Write COMMIT record

Apply changes to accountDetails.txt

If a crash happens at any stage, recovery handles it.

WAL Format
Example (UPDATE MAIL ID)
TXN ID: 1054
Operation: UPDATE

Old Account Number: 132
Old Name: gowtham
Old Account Balance: 0.00
Old MobileNumber: 9856432256
Old MailId: gow@gmail.com
Old Password: aa

New Account Number: 132
New Name: gowtham
New Account Balance: 5000.00
New MobileNumber: 9856432256
New MailId: yog@yahoo.com
New Password: aa

Commit status: Pending
End Txn

COMMIT 1054

Crash Recovery Logic

When the program starts, startRecovery() runs:

Undo Rules:

Undo is applied when a transaction is:

Not committed

Has complete WAL (End Txn present)

Undo actions:

CREATE → delete account

UPDATE → restore old values

DELETE → restore deleted info

TRANSFER → reverse balances

Redo Rules:

Redo is applied when:

WAL contains COMMIT

Data not yet written to file

txnId > checkpoint

Redo applies new values.

Checkpoint Mechanism

checkPoint.txt stores:

last successfully applied txn ID


This ensures:

Redone transactions are skipped next time

Undone transactions are removed from WAL

WAL size reduces over time

Input Validation
Name:

Max 20 characters

Alphabets + space only

Mobile Number:

Exactly 10 digits

Starts with 6/7/8/9

Mail ID:

Must end with:

@gmail.com

@yahoo.com

Crash Simulation Options

Simulate Crash After Wal Write

Simulate Crash After Partial Wal Write

Simulate Crash Before Commit

Simulate Crash After Commit

Random crash during multiple transactions

Recovery handles all cases.

Data File Format (accountDetails.txt)
AccountNumber|Name|Balance|Mobile|MailId|Password


Example:

132|gowtham|5000.00|9856432|dfg@gmail.com|aa
133|mathu|2500.00|87452|rtyu@gmail.com|sss

Limitations

No concurrency support

Single-user system

No multi-threading

Not a real DBMS (simulated behavior)

Future Enhancements

Concurrency & locking

GUI-based interface

Encrypted password storage

Better crash simulation

Support for batch transactions

Author

Yogeshwaran J J
