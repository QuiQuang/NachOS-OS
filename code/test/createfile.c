#include "syscall.h"
#include "copyright.h"
#define MaxNameLength 32
int main()
{
    char Str[MaxNameLength];

    PrintString("------CREATEFILE-----\n");
    PrintString("=> Input file name : ");
    ReadString(Str,MaxNameLength);

    if (CreateFile(Str) == 0)
        PrintString("===> Done");
    else PrintString("===> Unsuccessfullt create");

    Halt();
}
