#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

void keepalive(int primary_pid)
{
    pid_t keepalive_pid = getpid();
    printf("Keepalive started (PID: %d). Monitoring Primary %d\n", keepalive_pid, primary_pid);

    // Write PID to file
    FILE *fp = fopen("/tmp/keepalive.pid", "w");
    if (fp)
    {
        fprintf(fp, "%d", keepalive_pid);
        fclose(fp);
    }

    while (1)
    {
        if (kill(primary_pid, 0) == -1 && errno == ESRCH)
        {
            printf("Primary dead. Restarting...\n");
            pid_t new_primary = fork();
            if (new_primary == 0)
            {
                char pid_str[20];
                snprintf(pid_str, sizeof(pid_str), "%d", keepalive_pid);
                execl("./keeper", "keeper", "primary", pid_str, NULL);
                perror("execl");
                exit(1);
            }
            primary_pid = new_primary;
            printf("New Primary PID: %d\n", primary_pid);
        }
        sleep(1);
    }
}