#include "syscall.h"
#include "copyright.h"
#define MaxLength       255

int main()
{
    int numBytes;
    char buf[MaxLength];
    int i;

    PrintString("------ECHO------\n");
    PrintString("=> Input : ");


    numBytes = Read(buf,MaxLength,0);
    PrintString("-----------------------------\n");

    if (numBytes != -1 && numBytes != -2)
    {
        PrintString("=> Output : ");
        Write(buf,numBytes,1);

        PrintString("\n");
        PrintString("-----------------------------");
    }
    else
    {
    	PrintString("===> Error");
    }

    Halt();
}
