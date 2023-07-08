#include"syscall.h"

void main()
{
    int pingPID, pongPID, pangPID;
    PrintString("Ping-Pong test starting...\n\n");
    pingPID = Exec("./test/ping",4);
    pongPID = Exec("./test/pong",4);
    // pangPID = Exec("./test/pang",1);
    while(1) {};
}

