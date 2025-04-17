#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void dummy(int primary_pid)
{
    printf("Dummy process started. Launching keepalive...\n");
    pid_t child = fork();
    if (child == 0)
    {
        char pid_str[20];
        snprintf(pid_str, sizeof(pid_str), "%d", primary_pid);
        execl("./keeper", "keeper", "keepalive", pid_str, NULL);
        perror("execl");
        exit(1);
    }
    printf("Dummy exiting.\n");
    exit(0);
}