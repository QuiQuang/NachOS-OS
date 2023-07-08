#include "syscall.h"
#include "copyright.h"
#define MaxNameLength   32

int main()
{
    char* filename;

    PrintString("------DELETE------\n");

    PrintString("=> Input file to delete : ");
    ReadString(filename,MaxNameLength);

    if(Delete(filename) == 0)
    {
        PrintString("===> Success");
    }

    Halt();
}
