#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "thread.h"
#include "synch.h"
#include "addrspace.h"
#include "utility.h"
extern void StartProcess2(int id);
#define MaxNameLength   32                          //max file length
#define MaxLength       255                         //max data

void IncreasePC()
{
    int pcAfter = machine->registers[NextPCReg] + 4;
    machine->registers[PrevPCReg] = machine->registers[PCReg];
    machine->registers[PCReg] = machine->registers[NextPCReg];
    machine->registers[NextPCReg] = pcAfter;
}

char* User2System(int virtAddr,int limit)
{
    int i;                                          // index
    int oneChar;
    char* kernelBuf = NULL;
    kernelBuf = new char[limit +1];                 //need for terminal string
    if (kernelBuf == NULL)
        return kernelBuf;
    memset(kernelBuf,0,limit+1);                    //printf("\n Filename u2s:");
    for (i = 0 ; i < limit ; i++)
    {
        machine->ReadMem(virtAddr+i,1,&oneChar);
        kernelBuf[i] = (char)oneChar;               //printf("%c",kernelBuf[i]);
        if (oneChar == 0)
            break;
    }
    return kernelBuf;
}

int System2User(int virtAddr,int len,char* buffer)
{
    if (len < 0) return -1;
    if (len == 0)return len;
    int i = 0;
    int oneChar = 0 ;
    do
    {
        oneChar = (int) buffer[i];
        machine->WriteMem(virtAddr+i,1,oneChar);
        i ++;
    }
    while(i < len && oneChar != 0);
    return i;
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);               //paramter 1 -> 4 : register 4 -> 7
    switch (which)                                     //result          : register 2
    {
    case NoException:
    {
        interrupt->Halt();
        return;
    }
    case PageFaultException:
    {
        DEBUG('a', "\n No valid translation found");
        printf("\n\n No valid translation found");
        interrupt->Halt();
        break;
    }
    case ReadOnlyException:
    {
        DEBUG('A',"\n Write attempted to page marked read-only");
        printf("\n\n Write attempted to page marke read-only");
        interrupt->Halt();
        break;
    }
    case BusErrorException:
    {
        DEBUG('a', "\n Translation resulted invalid physical address");
        printf("\n\n Translation resulted invalid physical address");
        interrupt->Halt();
        break;
    }
    case AddressErrorException:
    {
        DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
        printf("\n\n Unaligned reference or one that was beyond the end of the address space");
        interrupt->Halt();
        break;
    }
    case IllegalInstrException:
    {
        DEBUG('a', "\n Unimplemented or reserved instr.");
        printf("\n\n Unimplemented or rrvedion");
        interrupt->Halt();
        break;
    }
    case NumExceptionTypes:
    {
        DEBUG('a', "\n Number exception types");
        printf("\n\n Number exception types");
        interrupt->Halt();
        break;
    }
    case SyscallException:
    {
        switch (type)
        {
        case SC_Halt:
        {
            DEBUG('a', "\n Shutdown, initiated by user program.");
            printf ("\n\n Shutdown, initiated by user program.");
            interrupt->Halt();
            return;
        }

        case SC_CreateFile:
        {
            int virtAddr;
            char* filename;

            DEBUG('a',"\n SC_Create call ...");
            DEBUG('a',"\n Reading virtual address of filename");
            virtAddr = machine->ReadRegister(4);
            DEBUG ('a',"\n Reading filename.");
            filename = User2System(virtAddr,MaxNameLength+1);           //copy memory from user to system
            if (filename == NULL)                                       //create but still null
            {
                printf("\n => Not enough memory in system");
                DEBUG('a',"\n Not enough memory in system");
                machine->WriteRegister(2,-1);
                delete filename;
                // IncreasePC();
                break;
            }
            if (filename[0] == '\0')
            {
                printf("\n ===> File name cant be empty");
                machine->WriteRegister(2,-1);
                delete filename;
                // IncreasePC();
                break;
            }
            DEBUG('a',"\n Finish reading filename");
            if (!fileSystem->Create(filename,0))                        //error
            {
                printf("\n=> Error create file '%s'",filename);
                machine->WriteRegister(2,-1);
                delete filename;
                //  IncreasePC();
                break;
            }
            if (filename[0] == '\n')
            {
                printf("\n=> File name is not empty");
                machine->WriteRegister(2,-1);
                delete filename;
                //IncreasePC();
                break;
            }
            machine->WriteRegister(2,0);
            delete filename;
            //   IncreasePC();
            break;
        }

        case SC_Open:
        {
            int virtAddr = machine->ReadRegister(4);
            int type = machine->ReadRegister(5);
            char*filename;
            filename = User2System(virtAddr,MaxNameLength);
            int Slot = fileSystem->FindFreeSlot();                          //find slot in list openf
            if (Slot != -1)
            {
                if (type == 0 || type == 1)                                 //stdin and stdout
                {
                    if ((fileSystem->openf[Slot] = fileSystem->Open(filename,type)) != NULL)
                    {
                        fileSystem->openf[Slot]->SetName(filename);         //update name to pointer for SC_Delete
                        machine->WriteRegister(2,Slot);
                        //     IncreasePC();
                        delete filename;
                        break;
                    }
                }
                else if (type == 2)
                {
                    machine->WriteRegister(2,0);
                    //    IncreasePC();
                    delete filename;
                    break;
                }
                else
                {
                    machine->WriteRegister(2,1);
                    delete filename;
                    // IncreasePC();
                    break;
                }
            }
            machine->WriteRegister(2,-1);
            delete filename;
            //  IncreasePC();
            break;
        }

        case SC_Read:
        {
            int virtAddr = machine->ReadRegister(4);
            int Count = machine->ReadRegister(5);
            int ID = machine->ReadRegister(6);

            int Bef, Aft;
            char*buf;

            if(ID < 0 || ID > 9 || fileSystem->openf[ID] == NULL)   //out of range and NULL file
            {
                printf("\n=> File is nonexistent");
                machine->WriteRegister(2,-1);
                //   IncreasePC();
                break;
            }
            else if (fileSystem->openf[ID]->type == 3)
            {
                printf("\n=> Unavailable to read stdout file");
                machine->WriteRegister(2,-1);
                //  IncreasePC();
                break;
            }
            //process read file
            Bef = fileSystem->openf[ID]->GetCurrentPos();
            buf = User2System(virtAddr,Count);

            if(fileSystem->openf[ID]->type==2)                      //type 2 : stdin
            {
                //return register 2 : numBytes                      // giong phia duoi
                int size = gSynchConsole->Read(buf,Count);
                System2User(virtAddr,size,buf);
                if(size < MaxLength)
                    machine->WriteRegister(2,size);
                else machine->WriteRegister(2,-1);
                delete[] buf;
                //  IncreasePC();
                break;
            }

            if((fileSystem->openf[ID]->Read(buf,Count)) > 0)        //<---numBytes > MaxLength => error
            {
                Aft = fileSystem->openf[ID]->GetCurrentPos();
                System2User(virtAddr,Aft-Bef,buf);
                if ((Aft - Bef) < MaxLength)
                    machine->WriteRegister(2,Aft-Bef);
                else
                    machine->WriteRegister(2,-1);
            }
            else
            {
                printf("\n=> Empty file");
                machine->WriteRegister(2,-2);
            }
            delete[] buf;
            //  IncreasePC();
            break;
        }

        case SC_Write:
        {
            int virtAddr = machine->ReadRegister(4);
            int Count = machine->ReadRegister(5);
            int ID = machine->ReadRegister(6);

            int Bef, Aft;
            char*buf;
            if(ID < 0 || ID > 9 || fileSystem->openf[ID] == NULL)
            {
                printf("\n=> File is nonexistent");
                machine->WriteRegister(2,-1);
                //   IncreasePC();
                break;
            }
            if (fileSystem->openf[ID]->type == 1 || fileSystem->openf[ID]->type == 2)   //error with type to write
            {
                printf("\n=> Unavailable to write into stdin and read - only file");
                machine->WriteRegister(2,-1);
                //IncreasePC();
                break;
            }
            Bef = fileSystem->openf[ID]->GetCurrentPos();
            buf = User2System(virtAddr, Count);

            if (fileSystem->openf[ID]->type == 0)                                        //return size
            {
                if ((fileSystem->openf[ID]->Write(buf,Count)) > 0)
                {
                    Aft = fileSystem->openf[ID]->GetCurrentPos();
                    machine->WriteRegister(2,Aft-Bef);
                    delete[] buf;
                    // IncreasePC();
                    break;
                }
            }
            if (fileSystem->openf[ID]->type == 3)
            {
                int i = 0;
                while (buf[i] != 0)                                                        //write each char
                {
                    gSynchConsole->Write(buf + i,1);
                    i++;
                }
                gSynchConsole->Write(buf + i,1);
                machine->WriteRegister(2,i-1);
                delete[] buf;
                //  IncreasePC();
                break;
            }
        }

        case SC_Close:
        {
            int ID = machine->ReadRegister(4);
            if (ID >= 2 && ID <= 9)                     //************************
            {
                if (fileSystem->openf[ID] != NULL)
                {
                    delete fileSystem->openf[ID];
                    fileSystem->openf[ID] = NULL;
                    machine->WriteRegister(2,0);
                    // IncreasePC();
                    break;
                }
            }
            machine->WriteRegister(2,-1);
            // IncreasePC();
            break;
        }

        case SC_PrintString:
        {
            int virtAddr = machine->ReadRegister(4);
            int length = 0;
            char*buf;

            buf = User2System(virtAddr,MaxNameLength+1);
            while (buf[length]!= 0 && buf[length]!='\n')
            {
                length++;
            } //calculate length
            gSynchConsole->Write(buf,length+1);                                     //write each char
            delete[] buf;
            // IncreasePC();
            break;
        }

        case SC_ReadString:
        {
            int virtAddr = machine->ReadRegister(4);
            int length = machine->ReadRegister(5);
            char* buf;

            buf = User2System(virtAddr, length);                                //copy string from user to system
            gSynchConsole->Read(buf,length);                                    //read by using method of Synchrons
            System2User(virtAddr,length,buf);                                   //copy back to user
            delete[] buf;
            // IncreasePC();
            break;
        }

        case SC_Seek:
        {
            int pos = machine->ReadRegister(4);                                 //position to point to
            int ID = machine->ReadRegister(5);
            if(ID < 0 || ID > 9 || fileSystem->openf[ID] == NULL)
            {
                printf("\n=> File is nonexistent");
                machine->WriteRegister(2,-1);
                //  IncreasePC();
                break;
            }
            if (ID == 0 && ID == 1)                                             //error with stdin and stdout
            {
                printf("\n=> Unavailble seek in console file");
                machine->WriteRegister(2,-1);
                // IncreasePC();
            }

            pos = (pos == -1) ? fileSystem->openf[ID]->Length() : pos;          //pos == -1 ? then pos = length, else skip
            if(pos > fileSystem->openf[ID]->Length() || pos < 0)
            {
                printf("\n=> Unavailable seek to this position");
                machine->WriteRegister(2,-1);
            }
            else
            {
                fileSystem->openf[ID]->Seek(pos);
                machine->WriteRegister(2,pos);
            }
            //  IncreasePC();
            break;
        }

        case SC_Delete:
        {
            int virtAddr = machine->ReadRegister(4);
            char*name;
            name = User2System(virtAddr,MaxNameLength+1);

            if(name == NULL)
            {
                printf("\n=> Not enough memory in system.");
                DEBUG('a',"\nNot enough memory in system.");
                machine->WriteRegister(2,-1);
                // IncreasePC();
                delete name;
                break;
            }

            for(int i = 2; i<10; i++)
            {
                if(fileSystem->openf[i] != NULL)
                {
                    if(strcmp(name,fileSystem->openf[i]->GetName()) == 0)
                    {
                        printf("\n=> File is opened, cannot be deleted.");
                        machine->WriteRegister(2,-1);
                        // IncreasePC();
                        delete name;
                        break;
                    }
                }
            }

            if(!fileSystem->Remove(name))
            {
                printf("=> File is nonexistent");
                machine->WriteRegister(2,-1);
                //IncreasePC();
                delete name;
                break;
            }
            machine->WriteRegister(2,0);
            //IncreasePC();
            delete name;
            break;
        }

        case SC_Exec:
        {
            //Prototype SpaceId Exec(char *name);
            int virtAddr;
            virtAddr = machine->ReadRegister(4);

            char *name;
            name = User2System(virtAddr, MaxNameLength + 1);

            int pr = machine->ReadRegister(5);
            if(pr==0)
            {
                        printf("\n\nPriority is an invalid value.\n");
                        break;
            }

            // Kiem tra ten chuong trinh
            if(name == NULL)
            {
                DEBUG('a', "\nCan not execute program with null name");
                printf("\nCan not execute program with null name");
                machine->WriteRegister(2, -1);
                //IncreasePC();
                break;
            }

            Thread *mythread;
            if((mythread = new Thread(name)) == NULL)
            {
                DEBUG('a', "\nNot enough memory for a new thread.");
                printf("\nNot enough memory for a new thread.");
                machine->WriteRegister(2,-1);
                delete[] name;
                delete mythread;
                break;
            }

            int pid; // pid cua mythread
            pid  = pageTable->Find();
            // kiem tra pagetable con cho trong hay khong
            if(pid < 0)
            {
                DEBUG('a', "\nThere is no free slot.");
                printf("\nThere is no free slot.");
                machine->WriteRegister(2,-1);
                // IncreasePC();
                delete[] name;
                delete mythread;
                break;
            }

            pageTable->Mark(pid);

            filenameTable[pid] = name;   // luu lai ten tien trinh vao filenameTable

            mythread->SetPriority(pr); // gan priority cho thread

           // currentThread->addChildThread();

            mythread->Fork(StartProcess2,pid);

            currentThread->Yield();

            machine->WriteRegister(2, pid);

            break;
        }

        case SC_PrintChar:
        {
            //prototype void PrintChar(char c);
            char ch;
            ch = (char)machine->ReadRegister(4);
            gSynchConsole->Write(&ch, 1);
            break;
        }

        case SC_Exit:
        {
            //currentThread->Yield();
            currentThread->FreeSpace();
            break;
        }
        default:
            break;
        }
        IncreasePC();
        break;
    }
    }
    return;
}
