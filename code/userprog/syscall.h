#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "copyright.h"

#define SC_Halt		    0
//Thao tac
#define SC_Exit		    1
#define SC_Exec		    2
#define SC_Join		    3
//Tep tin
#define SC_CreateFile	4
#define SC_Open		    5
#define SC_Read		    6
#define SC_Write	    7
#define SC_Close	    8
#define SC_Seek         9
#define SC_Delete       10
//Co ban
#define SC_PrintString  11
#define SC_ReadString   12
#define SC_PrintChar    13
#define SC_ReadChar     14
//Multithreading
#define SC_Fork	    	15
#define SC_Yield	    16
#define SC_Sleep        17

#ifndef IN_ASM

/* Stop Nachos, and print out performance stats */
void Halt();

//-----------------------------------------------------------------

/* Address space control operations: Exit, Exec, and Join */
/* This user program is done (status = 0 means exited normally). */
void Exit(int status);
/* A unique identifier for an executing user program (address space) */
typedef int SpaceId;
/* Run the executable, stored in the Nachos file "name", and return the
 * address space identifier
 */
SpaceId Exec(char *name,int priority);
/* Only return once the the user program "id" has finished.
 * Return the exit status.
 */
int Join(SpaceId id);
/* File system operations: Create, Open, Read, Write, Close
 * These functions are patterned after UNIX -- files represent
 * both files *and* hardware I/O devices.
 *
 * If this assignment is done before doing the file system assignment,
 * note that the Nachos file system has a stub implementation, which
 * will work for the purposes of testing out these routines.
 */
/* User-level thread operations: Fork and Yield.  To allow multiple
 * threads to run within a user program.
 */

/* Fork a thread to run a procedure ("func") in the *same* address space
 * as the current thread.
 */
void Fork(void (*func)());
/* Yield the CPU to another runnable thread, whether in this address space
 * or not.
 */
void Yield();

//-----------------------------------------------------------------

/* A unique identifier for an open Nachos file. */
typedef int OpenFileId;
#define ConsoleInput	0
#define ConsoleOutput	1

/* Create a Nachos file, with "name" */
int CreateFile(char *name);
/* Open the Nachos file "name", and return an "OpenFileId" that can
 * be used to read and write to the file.
 */
OpenFileId Open(char *name, int type);
/* Write "size" bytes from "buffer" to the open file. */
void Write(char *buffer, int size, OpenFileId id);
int Read(char *buffer, int size, OpenFileId id);
void Close(OpenFileId id);
int Delete(char*name);
int Seek(int pos, OpenFileId ID);

//-----------------------------------------------------------------

void PrintString(char buf[]);
void ReadString(char buf[], int length);
void PrintChar(char c);
void ReadChar();
void Sleep();

#endif /* IN_ASM */

#endif /* SYSCALL_H */
