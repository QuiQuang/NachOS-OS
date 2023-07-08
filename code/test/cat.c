#include "syscall.h"
#include "copyright.h"
#define MaxNameLength   32

int main()
{

    char Name[MaxNameLength];
    char* buf;
    int numBytes;
    int ID;
    int i;

    PrintString("------CAT------\n");
    PrintString("=> Input file name : ");
    ReadString(Name,MaxNameLength);

    ID = Open(Name,1);
    if (ID != -1)
    {
        numBytes = Seek(-1, ID);
        Seek(0, ID);
        PrintString("- Output : \n");

        for (i = 0;i<numBytes;i++)
        {
            Read(buf,1 ,ID);
            Write(buf, 1, 1);
        }
        Close(ID);
    }
    else
        PrintString("=> Cannot open file");
    Close(ID);

    Halt();
}
