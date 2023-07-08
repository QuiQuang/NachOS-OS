#include "syscall.h"
#include "copyright.h"
#define MaxNameLength   32

int main()
{

    char NameS[MaxNameLength];
    char NameD[MaxNameLength];

    char* buf;
    int i;
    int numBytes;

    int ID_From;
    int ID_To;


    PrintString("------COPY------\n");
    PrintString("=> Input source file : ");
    ReadString(NameS,MaxNameLength);

    PrintString("=> Input destination file : ");
    ReadString(NameD,MaxNameLength);

    ID_From = Open(NameS,0);
    ID_To = Open(NameD,0);

    if(ID_From == -1)
    {
        PrintString("- SFile is nonexistent");
        return;
    }
    if(ID_To == -1)
    {
        PrintString("- DFile is nonexistent \n");
        PrintString("===> CREATE NEW FILE \n");
    }
    CreateFile(NameD);
    Close(ID_To);
    ID_To = Open(NameD,0);
    numBytes = Seek(-1, ID_From);
    Seek(0,ID_From);

    for (i = 0;i<numBytes;i++)
    {
        Read(buf, 1,ID_From);
        Write(buf, 1, ID_To);
    }

    Close(ID_From);
    Close(ID_To);

    PrintString("===> Done");

    Halt();
}
