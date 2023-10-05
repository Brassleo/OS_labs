#include <stdlib.h>
#include <windows.h>
#include <stdio.h>

BOOL WINAPI CtrlHandler(DWORD CtrlType)
{
    switch (CtrlType)
    {
    case CTRL_C_EVENT:
        printf("Child: Ctrl-C event\n\n");
        exit(2);

    default:
        return FALSE;
    }
}

int main(void)
{
    if (SetConsoleCtrlHandler(CtrlHandler, TRUE))
    {
        printf("\nChild: Waiting for signal...\n");

        while (1) {}
    }
    else
    {
        printf("\nChild: ERROR: Could not set control handler");
        return 1;
    }
    return 0;
}

/*
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <Windows.h>

void sigintHandler(int signal)
{
    printf("SIGINT\n");
    exit(signal);
}

int main()
{
    signal(SIGINT, sigintHandler);
    while(1) {
    }

    return 0;
}
*/
